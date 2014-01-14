
//POSIX
#include <termios.h>
#include <unistd.h>

// i2c library
#include "i2c-dev.h"

// C library
#include <stdio.h>
#include <stdlib.h>

// C++ library
#include <iostream>

// Camera library
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

// Boost C++
#include <boost/thread.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/median.hpp>

#include "singleton.h"
#include "daemonize.h"

namespace murix_ardrone {

class PID {
public:
	/*working variables*/
	boost::posix_time::ptime lastTime;
	double Input, Output, Setpoint;
	double errSum, lastErr;
	double kp, ki, kd;

	PID(){
		lastTime = boost::posix_time::microsec_clock::local_time();
		Input=0;
		Output=0;
		Setpoint=0;
		errSum=0;
		lastErr=0;
		kp=0;
		ki=0;
		kd=0;
	}

	void Compute()
	{
		/*How long since we last calculated*/
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration tick_diff = now - lastTime;
		double timeChange = tick_diff.total_microseconds() / 1000000.0f;

		/*Compute all the working error variables*/
		double error = Setpoint - Input;
		errSum += (error * timeChange);
		double dErr = (error - lastErr) / timeChange;

		/*Compute PID Output*/
		Output = kp * error + ki * errSum + kd * dErr;

		/*Remember some variables for next time*/
		lastErr = error;
		lastTime = now;
	}

	void SetTunings(double Kp, double Ki, double Kd)
	{
		kp = Kp;
		ki = Ki;
		kd = Kd;
	}
};


typedef struct  {
	float vbat;
	float vdd0;
	float vdd1;
	float vdd2;
	float vdd3;
	float vdd4;
	float vdd0_setpoint;
	float vdd1_setpoint;
	float vdd2_setpoint;
	float vdd3_setpoint;
	float vdd4_setpoint;
} vbat_t;

boost::lockfree::queue<vbat_t> vbat_queue(1);


float vbat_get(unsigned char channel,int fd)
{
	if(channel>9) return -1;
	unsigned lower = i2c_smbus_read_byte_data(fd, 0x34 + (channel<<1));
	unsigned upper = i2c_smbus_read_byte_data(fd, 0x33 + (channel<<1));
	unsigned value = upper<<2 | lower;
	//VREF Reference Voltage Internally connected to VDDC pin. 1.8V +/- 0.05V
	//Measured Input Scaling Factor External inputs ANA{0,1,2,3}  0.25  V/V  	ch 0-3
	//Measured Input Scaling Factor VDD{0,1,2,3,4} inputs 0.4  V/V 				ch 4-8
	//Measured Input Scaling Factor VINSYS input 0.25 V/V 						ch 9
	float factor;
	if(channel<4) factor=0.031;
	else if(channel<9) factor=1.8/0.4/1023;
	else factor=1.8/0.25/1023;
	float v = value * factor;
	//printf("Channel=%d Vbat=%.2fVolt RawValue=%d RawHiByte=0x%x RawLiByte=0x%x\n",(int)channel,v,value,upper,lower);
	return v;
}

int vbat_read(vbat_t *vbat,int fd)
{
	vbat->vbat=vbat_get(0,fd);
	vbat->vdd0=vbat_get(4,fd);
	vbat->vdd1=vbat_get(5,fd);
	vbat->vdd2=vbat_get(6,fd);
	vbat->vdd3=vbat_get(7,fd);
	vbat->vdd4=vbat_get(8,fd);
	return 0;
}

void thread_vbat(void){

	const int VBAT_ADDRESS=0x49;


	int fd = open( "/dev/i2c-0", O_RDWR );

	if( ioctl( fd, I2C_SLAVE, VBAT_ADDRESS ) < 0 )
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
	}

	//ADC_CTRL
	if( i2c_smbus_write_byte_data( fd, 0x30, 0xc7))   {
		fprintf( stderr, "Failed to write to I2C device\n" );
	}
	//ADC_MUX_1
	if( i2c_smbus_write_byte_data( fd, 0x31, 0x5f)){
		fprintf( stderr, "Failed to write to I2C device\n" );
	}
	//ADC_MUX_2
	if( i2c_smbus_write_byte_data( fd, 0x32, 0x0f))  {
		fprintf( stderr, "Failed to write to I2C device\n" );
	}

	vbat_t vbat;

	//Setpoint
	unsigned char v;

	v = i2c_smbus_read_byte_data(fd, 0x06);
	vbat.vdd0_setpoint = (v&0x80 ? (v & 0x3f) * 0.05 + 0.80 : 0);

	v = i2c_smbus_read_byte_data(fd, 0x07);
	vbat.vdd1_setpoint = (v&0x80 ? (v & 0x3f) * 0.05 + 0.80 : 0);

	v = i2c_smbus_read_byte_data(fd, 0x08);
	vbat.vdd2_setpoint = (v&0x80 ? (v & 0x3f) * 0.05 + 0.80 : 0);

	v = i2c_smbus_read_byte_data(fd, 0x09);
	vbat.vdd3_setpoint = (v&0x80 ? (v & 0x3f) * 0.05 + 2.70 : 0);

	v = i2c_smbus_read_byte_data(fd, 0x0a);
	vbat.vdd4_setpoint = (v&0x80 ? (v & 0x3f) * 0.05 + 2.70 : 0);

	//printf("Setpoints %f %f %f %f\n",vbat->vdd0_setpoint,vbat->vdd1_setpoint,vbat->vdd2_setpoint,vbat->vdd3_setpoint);
	while(1) {
		vbat_read(&vbat,fd);
		vbat_queue.push(vbat);
	}


}

void vbat_show(void){
	vbat_t vbat;

	for(;;){
		if(vbat_queue.pop(vbat)){
			printf("set|%4.2f|%4.2f|%4.2f|%4.2f|%4.2f|\r\n",
					vbat.vdd0_setpoint,
					vbat.vdd1_setpoint,
					vbat.vdd2_setpoint,
					vbat.vdd3_setpoint,
					vbat.vdd4_setpoint);
			printf("get|%4.2f|%4.2f|%4.2f|%4.2f|%4.2f|%5.2f|\r\n",
					vbat.vdd0,
					vbat.vdd1,
					vbat.vdd2,
					vbat.vdd3,
					vbat.vdd4,
					vbat.vbat);
		}
	}

}

boost::mutex mutex_xioctl;

int xioctl(int fh, int request, void *arg)
{
	boost::mutex::scoped_lock lock(mutex_xioctl);

	int r;
	do {
		r = ioctl(fh, request, arg);
	} while (-1 == r && EINTR == errno);
	return r;
}

static void errno_exit(const char *s)
{
	fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
}


struct buffer {
	void   *start;
	size_t  length;
};

#define CLEAR(x) memset(&(x), 0, sizeof(x))



typedef struct {
   uint8_t buf[614400];
} camera_image_horizontal_t;

typedef struct {
   uint8_t buf[50688];
} camera_image_vertical_t;


boost::mutex mutex_h;
boost::mutex mutex_v;

camera_image_horizontal_t img_h;
camera_image_vertical_t img_v;

void process_image(const void *p, int size)
{
	printf("%s: called! size=%d\n", __func__,size);


	if(size==614400){
		boost::mutex::scoped_lock lock_h(mutex_h, boost::try_to_lock);
		if(lock_h){
			memcpy(&img_h.buf,p,size);
		} else {
			printf("fail to lock img_h\r\n");
		}
	}
	if(size==50688){
		boost::mutex::scoped_lock lock_v(mutex_v, boost::try_to_lock);
		if(lock_v){
			memcpy(&img_v.buf,p,size);
		} else {
			printf("fail to lock img_v\r\n");
		}
	}


	//	printf("sendto udp camera-horizontal \n");
		/*
		pFile = fopen ("/tmp/horizontal.yuv", "wb");
		fwrite (p , 1, size, pFile);
		fclose (pFile);
*/
		//system("/data/video/udp_broadcast_file 7070 /tmp/horizontal.yuv");
	//}

	//if(size==50688){
		//printf("sendto udp camera-vertical \n");
		/*
		pFile = fopen ("/tmp/vertical.yuv", "wb");
		fwrite (p , 1, size, pFile);
		fclose (pFile);
*/
		//system("/data/video/udp_broadcast_file 6969 /tmp/vertical.yuv");
	//}



	//if (out_buf)
	//	fwrite(p, size, 1, stdout);


/*
	fflush(stderr);
	fprintf(stderr, ".");
	fflush(stdout);
*/

}


int read_frame(int fd,struct buffer *buffers,unsigned int n_buffers)
{
	struct v4l2_buffer buf;

	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
		switch (errno) {
		case EAGAIN:
			return 0;
		case EIO:
			/* Could ignore EIO, see spec. */
			/* fall through */
		default:
			errno_exit("VIDIOC_DQBUF");
		}
	}

	//assert(buf.index < n_buffers);
	process_image(buffers[buf.index].start, buf.bytesused);

	if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
		errno_exit("VIDIOC_QBUF");


	return 1;
}


void camera_service(char* dev_name){

	struct buffer          *buffers;
	static unsigned int     n_buffers = 4;

	///////////////////////////////////////////////////////////////////////////
	//////////////////

	int fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);
	if (-1 == fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n", dev_name, errno, strerror(errno));
	}

	////////////////////////////////////////////////////////////////
	///////////////// ---------- DEVICE INIT ----------------------

	struct v4l2_capability cap;

	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_QUERYCAP");
		}
	}

	printf("\tdriver: %s\n"
			"\tcard: %s \n"
			"\tbus_info: %s\n",
			cap.driver, cap.card, cap.bus_info);
	printf("\tversion: %u.%u.%u\n",
			(cap.version >> 16) & 0xFF,
			(cap.version >> 8) & 0xFF,
			cap.version & 0xFF);
	printf("\tcapabilities: 0x%08x\n", cap.capabilities);

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is no video capture device\n",
				dev_name);
		exit(EXIT_FAILURE);
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "%s does not support streaming i/o\n", dev_name);
		exit(EXIT_FAILURE);
	}

	//////////////////////////////////////////////////////////////////////
	///////////////////--------- SET FORMAT

	struct v4l2_format fmt;
	CLEAR(fmt);

	if(strstr("p6_camif.0",(const char *)cap.driver)){
		printf("camera-horizontal\n");
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
		fmt.fmt.pix.width=640;
		fmt.fmt.pix.height=480;
	} else {
		printf("camera-vertical\n");
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
		fmt.fmt.pix.width=176;
		fmt.fmt.pix.height=144;
	}

	printf("\tfmt.fmt.pix.pixelformat: %c,%c,%c,%c\n",
                            fmt.fmt.pix.pixelformat & 0xFF,
                            (fmt.fmt.pix.pixelformat >> 8) & 0xFF,
                            (fmt.fmt.pix.pixelformat >> 16) & 0xFF,
                            (fmt.fmt.pix.pixelformat >> 24) & 0xFF
                            );
	printf("\n");

            if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
                    errno_exit("VIDIOC_S_FMT");


	/////////////////////////////////////////////////////////////////
	/////////////// ---------------- MMAP INIT ---------//////////////

	struct v4l2_requestbuffers req;

	printf("%s: called!\n", __func__);

	CLEAR(req);

	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support "
					"memory mapping\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}
	printf("\treq.count: %d\n", req.count);
	printf("\treq.type: %d\n", req.type);
	printf("\treq.memory: %d\n", req.memory);
	printf("\n");


	if (req.count < 2) {
		fprintf(stderr, "Insufficient buffer memory on %s\n",
				dev_name);
		exit(EXIT_FAILURE);
	}

	buffers = (struct buffer*) calloc(req.count, sizeof(struct buffer));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;

		if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");

		printf("\tbuf.index: %d\n", buf.index);
		printf("\tbuf.type: %d\n", buf.type);
		printf("\tbuf.bytesused: %d\n", buf.bytesused);
		printf("\tbuf.flags: %d\n", buf.flags);
		printf("\tbuf.field: %d\n", buf.field);
		printf("\tbuf.timestamp.tv_sec: %ld\n", (long) buf.timestamp.tv_sec);
		printf("\tbuf.timestamp.tv_usec: %ld\n", (long) buf.timestamp.tv_usec);
		printf("\tbuf.timecode.type: %d\n", buf.timecode.type);
		printf("\tbuf.timecode.flags: %d\n", buf.timecode.flags);
		printf("\tbuf.timecode.frames: %d\n", buf.timecode.frames);
		printf("\tbuf.timecode.seconds: %d\n", buf.timecode.seconds);
		printf("\tbuf.timecode.minutes: %d\n", buf.timecode.minutes);
		printf("\tbuf.timecode.hours: %d\n", buf.timecode.hours);
		printf("\tbuf.timecode.userbits: %d,%d,%d,%d\n",
				buf.timecode.userbits[0],
				buf.timecode.userbits[1],
				buf.timecode.userbits[2],
				buf.timecode.userbits[3]);
		printf("\tbuf.sequence: %d\n", buf.sequence);
		printf("\tbuf.memory: %d\n", buf.memory);
		printf("\tbuf.m.offset: %d\n", buf.m.offset);
		printf("\tbuf.length: %d\n", buf.length);
		printf("\tbuf.input: %d\n", buf.input);
		printf("\n");

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start =
				mmap(NULL /* start anywhere */,
						buf.length,
						PROT_READ | PROT_WRITE /* required */,
						MAP_SHARED /* recommended */,
						fd, buf.m.offset);

		if (MAP_FAILED == buffers[n_buffers].start)
			errno_exit("mmap");
	}

	/////////////////////////////////////////////////////////////////////
	////////////---------------- START CAPTURE

	unsigned int i;
	enum v4l2_buf_type type;
	int err;

	for (i = 0; i < n_buffers; ++i) {
		struct v4l2_buffer buf;

		printf("\ti: %d\n", i);

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		printf("\tbuf.index: %d\n", buf.index);

		err == xioctl(fd, VIDIOC_QBUF, &buf);
		printf("\terr: %d\n", err);

		if (-1 == err)
			errno_exit("VIDIOC_QBUF");

		printf("\tbuffer queued!\n");
	}

	printf("Before STREAMON\n");
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
		errno_exit("VIDIOC_STREAMON");
	printf("After STREAMON\n");

	//////////////////////////////////////////////////////////////


	while (1) {

		for (;;) {
			fd_set fds;
			struct timeval tv;
			int r;

			FD_ZERO(&fds);
			FD_SET(fd, &fds);

			/* Timeout. */
			tv.tv_sec = 2;
			tv.tv_usec = 0;

			r = select(fd + 1, &fds, NULL, NULL, &tv);

			if (-1 == r) {
				if (EINTR == errno)
					continue;
				errno_exit("select");
			}

			if (0 == r) {
				fprintf(stderr, "select timeout\n");
				//exit(EXIT_FAILURE);
				continue;
			}

			if (read_frame(fd,buffers,n_buffers))
				break;
			/* EAGAIN - continue select loop. */
		}
	}

	////////////////////////////////////////////////////////////////////////
	//////////////////////////-------- STOP CAPTURE

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
		errno_exit("VIDIOC_STREAMOFF");

	////////////////////////////////////////////////////////////////////////
	//////////////////////////-------- UNINIT DEVICE

	for (i = 0; i < n_buffers; ++i)
		if (-1 == munmap(buffers[i].start, buffers[i].length))
			errno_exit("munmap");

	free(buffers);

	////////////////////////////////////////////////////////////////////////
	//////////////////////////-------- CLOSE DEVICE

	if (-1 == close(fd))
		errno_exit("close");

}

void camera_horizontal(void){
	camera_service("/dev/video0");
}
void camera_vertical(void){
	camera_service("/dev/video1");
}



//#pragma pack(push,1)
typedef struct
{
	uint16_t size;                // +0x00 Size of the following data (always 0x2C)
	uint16_t seq;                 // +0x02 Sequence number, increases every update
	uint16_t acc_x;              // +0x04 Raw data (10-bit) of the accelerometers multiplied by 4
	uint16_t acc_y;              // +0x04 Raw data (10-bit) of the accelerometers multiplied by 4
	uint16_t acc_z;              // +0x04 Raw data (10-bit) of the accelerometers multiplied by 4
	uint16_t gyro_x;             // +0x0A Raw data for the gyros, 12-bit A/D converted voltage of the gyros. X,Y=IDG, Z=Epson
	uint16_t gyro_y;             // +0x0A Raw data for the gyros, 12-bit A/D converted voltage of the gyros. X,Y=IDG, Z=Epson
	uint16_t gyro_z;             // +0x0A Raw data for the gyros, 12-bit A/D converted voltage of the gyros. X,Y=IDG, Z=Epson
	uint16_t gyro_110_x;         // +0x10 4.5x Raw data (IDG), gyro values with another resolution (see IDG-500 datasheet)
	uint16_t gyro_110_y;         // +0x10 4.5x Raw data (IDG), gyro values with another resolution (see IDG-500 datasheet)
	uint16_t acc_temp;            // +0x14 Accs temperature -- startup value 120 @ 25C, rising to 143
	uint16_t gyro_temp;           // +0x16 XYGyro temperature (IDG), 12-bit A/D converted voltage of IDG's temperature sensor -- startup value 1532 @ 25C, rising to 1572
	uint16_t vrefEpson;           // +0x18 ZGyro v_ref (Epson), 12-bit A/D converted reference voltage of the Epson sensor
	uint16_t vrefIDG;             // +0x1A XYGyro v_ref (IDG), 12-bit A/D converted reference voltage of the IDG sensor
	uint16_t us_echo;             // +0x1C bit15=1 echo pulse transmitted, bit14-0 first echo. Value 30 = 1cm. min value: 784 = 26cm
	uint16_t checksum;            // +0x1E Checksum = sum of all values except checksum (22 values)
	uint16_t us_echo_start;       // +0x20 Array with starts of echos (8 array values @ 25Hz, 9 values @ 22.22Hz)
	uint16_t us_echo_end;         // +0x22 Array with ends of echos   (8 array values @ 25Hz, 9 values @ 22.22Hz)
	uint16_t us_association_echo; // +0x24 Ultrasonic parameter -- echo number starting with 0. max value 3758. examples: 0,1,2,3,4,5,6,7  ; 0,1,2,3,4,86,6,9
	uint16_t us_distance_echo;    // +0x26 Ultrasonic parameter -- no clear pattern
	uint16_t us_courbe_temps;     // +0x28 Ultrasonic parameter -- counts up from 0 to approx 24346 in 192 sample cycles of which 12 cylces have value 0
	uint16_t us_courbe_valeur;    // +0x2A Ultrasonic parameter -- value between 0 and 4000, no clear pattern. 192 sample cycles of which 12 cylces have value 0
	uint16_t us_courbe_ref;       // +0x2C Ultrasonic parameter -- coutns down from 4000 to 0 in 192 sample cycles of which 12 cylces have value 0
	uint16_t uk01;
	uint16_t uk02;
} navboard_packet_t;
//#pragma pack(pop)


typedef struct {

	double acc_x;
	double acc_y;
	double acc_z;

	double gyro_x;
	double gyro_y;
	double gyro_z;

	double gyro_x_dt;
	double gyro_y_dt;
	double gyro_z_dt;

	double gyro_x_integrate;
	double gyro_y_integrate;
	double gyro_z_integrate;

	double acc_pitch;
	double acc_roll;

	double fusion_pitch;
	double fusion_roll;
	double fusion_yaw;

	double height;

	double dt;
	double seq;

} navboard_fusion_t;

typedef struct {
	double acc_x_offset;
	double acc_y_offset;
	double acc_z_offset;
	double gyro_x_offset;
	double gyro_y_offset;
	double gyro_z_offset;
} navboard_calibration_t;

typedef struct {
	int16_t front_left;
	int16_t front_right;
	int16_t rear_left;
	int16_t rear_right;
} motor_set_speed_t;


/*
boost::atomic_int producer_count(0);
boost::atomic_int consumer_count(0);
boost::atomic<bool> done (false);
 */

boost::lockfree::queue<navboard_packet_t> navboard_queue(1);
boost::lockfree::queue<navboard_fusion_t> fusion_queue(1);
boost::lockfree::queue<motor_set_speed_t> motor_speed_queue(1);

//val=0 -> set gpio output lo
//val=1 -> set gpio output hi
//val=-1 -> set gpio as input (output hi-Z)
int gpio_set(int nr,int val)
{
	//printf("gpio_set %d = %d\r\n",nr,val);
	char cmdline[200];
	if(val<0) sprintf(cmdline,"/usr/sbin/gpio %d -d i",nr);
	else if(val>0) sprintf(cmdline,"/usr/sbin/gpio %d -d ho 1",nr);
	else sprintf(cmdline,"/usr/sbin/gpio %d -d ho 0",nr);
	return system(cmdline);
}

void thread_navboard_read_raw(void)
{
	printf("navboard reader init\r\n");

	boost::asio::io_service io;
	boost::asio::serial_port port(io);


	port.open("/dev/ttyPA2");
	port.set_option(boost::asio::serial_port_base::baud_rate(460800));

	uint8_t c;

	uint8_t navboard_size=0x32;
	uint8_t navboard8[navboard_size];

	uint16_t* p_navboard16;
	p_navboard16 = (uint16_t*)& navboard8;

	navboard_packet_t packet;

	boost::circular_buffer<uint8_t> cb(navboard_size);

	//set /MCLR pin -> reset navboard
	gpio_set(132,1);

	//start acquisition
	uint8_t cmd=0x01;
	boost::asio::write(port, boost::asio::buffer(&cmd, 1));

	if(sizeof(navboard_packet_t)!=sizeof(navboard8)){
		printf("%d!=%d\r\n",sizeof(navboard_packet_t),sizeof(navboard8));
		printf("error %s:%d\r\n",__FILE__,__LINE__);
		exit(1);
	}

	printf("navboard reader working\r\n");
	while(1){
		//read
		boost::asio::read(port, boost::asio::buffer(&c,1));
		//put in circular buffer
		cb.push_back(c);
		//buffer is full
		if(cb.size()==navboard_size){
			//header of buffer is navboard signature
			if(cb[0]==navboard_size && cb[1]==0x0 &&
					cb[31]==0x0 && cb[30]==0x1
			){

				//copy packet to linear buffer
				for(int i=0;i<navboard_size;i++){
					navboard8[i]=cb[i];
				}
				memcpy(&packet,&navboard8,sizeof(packet));
				navboard_queue.push(packet);

				/*
				for(int i=0;i<navboard_size/2;i++){
					printf("%5d|",p_navboard16[i]);
				}
				printf("\r\n");
				 */

			}
		}
	}
	port.close();

	//boost::this_thread::sleep(boost::posix_time::milliseconds(500));
}


double constraint_double(double value,double min,double max){
	double out=value;
	if(out<min)out=min;
	if(out>max)out=max;
	return out;
}
int16_t constraint_s16(int16_t value,int16_t min,int16_t max){
	int16_t out=value;
	if(out<min)out=min;
	if(out>max)out=max;
	return out;
}

void navboard_sensor_fusion(navboard_packet_t* packet,navboard_fusion_t* fusion,double dt,navboard_calibration_t* calibration){

	fusion->dt = dt;
	fusion->seq = packet->seq;

	//
	double radian2degree = 180.0f / M_PI;
	double gyro_gain = radian2degree / 10.0f;

	//offset and scale
	fusion->acc_x= (packet->acc_x - calibration->acc_x_offset);
	fusion->acc_y= (packet->acc_y - calibration->acc_y_offset);
	fusion->acc_z= (packet->acc_z - calibration->acc_z_offset);

	fusion->gyro_x = (packet->gyro_x - calibration->gyro_x_offset)/gyro_gain;
	fusion->gyro_y = (packet->gyro_y - calibration->gyro_y_offset)/gyro_gain;
	fusion->gyro_z = (packet->gyro_z - calibration->gyro_z_offset)/gyro_gain;

	///////////////////////////////////////////////////////////////////////////////////
	fusion->acc_pitch =  ( -atan2(fusion->acc_x,fusion->acc_z) * radian2degree );
	fusion->acc_roll =   (  atan2(fusion->acc_y,fusion->acc_z) * radian2degree );

	///////////////////////////////////////////////////////////////////////////////
	//prepare to integrate
	fusion->gyro_x_dt= fusion->gyro_x*dt;
	fusion->gyro_y_dt= fusion->gyro_y*dt;
	fusion->gyro_z_dt= fusion->gyro_z*dt;

	//integrate
	fusion->gyro_x_integrate+= fusion->gyro_x_dt;
	fusion->gyro_y_integrate+= fusion->gyro_y_dt;
	fusion->gyro_z_integrate+= fusion->gyro_z_dt;

	//clamp integrate
	fusion->gyro_x_integrate=constraint_double(fusion->gyro_x_integrate,-180,180);
	fusion->gyro_y_integrate=constraint_double(fusion->gyro_y_integrate,-180,180);
	fusion->gyro_z_integrate=constraint_double(fusion->gyro_z_integrate,-180,180);

	//////////////////////////////////////////////////////////////////////////////
	//use The complementary filter for pitch and roll
	fusion->fusion_pitch= 0.98*(fusion->fusion_pitch+fusion->gyro_x_dt)+(0.02*fusion->acc_pitch);
	fusion->fusion_roll = 0.98*(fusion->fusion_roll+fusion->gyro_y_dt)+(0.02*fusion->acc_roll);

	//use integrative for yaw
	fusion->fusion_yaw =  fusion->gyro_z_integrate;

	//////////////////////////////////////////////////////////////////////////////
	//clamp fusion

	fusion->fusion_pitch=constraint_double(fusion->fusion_pitch,-180,180);
	fusion->fusion_roll=constraint_double(fusion->fusion_roll,-180,180);
	fusion->fusion_yaw=constraint_double(fusion->fusion_yaw,-180,180);

	/////////////////////////////////////////////////////////////////////////////

	if(packet->us_echo / 37.5f < 650){
		fusion->height=packet->us_echo / 37.5f;
	}

}


void navboard_calibration(navboard_calibration_t* calibration){
	navboard_packet_t packet;

	using namespace boost::accumulators;
	accumulator_set<double, stats<tag::median(with_p_square_quantile) > > acc_raw_x;
	accumulator_set<double, stats<tag::median(with_p_square_quantile) > > acc_raw_y;
	accumulator_set<double, stats<tag::median(with_p_square_quantile) > > acc_raw_z;
	accumulator_set<double, stats<tag::median(with_p_square_quantile) > > gyro_raw_x;
	accumulator_set<double, stats<tag::median(with_p_square_quantile) > > gyro_raw_y;
	accumulator_set<double, stats<tag::median(with_p_square_quantile) > > gyro_raw_z;

	int calibration_samples=0;
	for(;;){
		if(navboard_queue.pop(packet)){
			/////////////////////////////////////////////////////////
			acc_raw_x(packet.acc_x);
			acc_raw_y(packet.acc_y);
			acc_raw_z(packet.acc_z);
			gyro_raw_x(packet.gyro_x);
			gyro_raw_y(packet.gyro_y);
			gyro_raw_z(packet.gyro_z);
			calibration_samples++;
			if(calibration_samples>100){
				break;
			}
		}
	}
	////////////////////////////////////////////////////////
	calibration->acc_x_offset = median(acc_raw_x);
	calibration->acc_y_offset = median(acc_raw_y);
	double gravitation = median(acc_raw_z)-((median(acc_raw_x)+median(acc_raw_y))/2.0f);
	calibration->acc_z_offset = median(acc_raw_z) - gravitation;
	///////////////////////////////////////////////////////
	calibration->gyro_x_offset = median(gyro_raw_x);
	calibration->gyro_y_offset = median(gyro_raw_y);
	calibration->gyro_z_offset = median(gyro_raw_z);
	//////////////////////////////////////////////////////

}

void thread_navboard_decoder(void){

	//
	navboard_packet_t packet;
	navboard_fusion_t fusion;
	navboard_calibration_t calibration;

	//
	boost::posix_time::ptime tick_back;
	boost::posix_time::ptime tick_now;
	boost::posix_time::time_duration tick_diff;

	//
	tick_back = boost::posix_time::microsec_clock::local_time();
	tick_now = boost::posix_time::microsec_clock::local_time();

	//
	printf("navboard calibration wait...\r\n");
	navboard_calibration(&calibration);

	//
	fusion.height=24;

	printf("navboard working!\r\n");
	for(;;){
		if(navboard_queue.pop(packet)){
			///////////////////////////////////////////////////////////////
			tick_back = tick_now;
			tick_now = boost::posix_time::microsec_clock::local_time();
			tick_diff = tick_now - tick_back;
			double dt = tick_diff.total_microseconds() / 1000000.0f;
			//////////////////////////////////////////////////////////////
			navboard_sensor_fusion(&packet,&fusion,dt,&calibration);
			fusion_queue.push(fusion);
			///////////////////////////////////////////////////////////////
		}
	}
}

void thread_stabilizer(void){
	navboard_fusion_t fusion;
	for(;;){
		if(fusion_queue.pop(fusion)){

			printf("dt=%.6f seq=%.0f a(%.2f|%.2f) g(%.2f|%.2f|%.2f) gi(%.2f|%.2f|%.2f) f(%.2f|%.2f|%.2f) h=%.2f\r\n",
					fusion.dt,
					fusion.seq,
					fusion.acc_pitch,
					fusion.acc_roll,
					fusion.gyro_x,
					fusion.gyro_y,
					fusion.gyro_z,
					fusion.gyro_x_integrate,
					fusion.gyro_y_integrate,
					fusion.gyro_z_integrate,
					fusion.fusion_pitch,
					fusion.fusion_roll,
					fusion.fusion_yaw,
					fusion.height
			);
		}
	}
}



void motorboard_cmd(boost::asio::serial_port* port,uint8_t cmd, uint8_t*reply, int replylen){
	boost::asio::write(*port, boost::asio::buffer(&cmd, 1));
	boost::asio::read(*port, boost::asio::buffer(reply,replylen));
}



void drone_motors(void){

	printf("motors init!\r\n");
	//
	boost::asio::io_service io;
	boost::asio::serial_port port(io);
	port.open("/dev/ttyPA1");
	port.set_option(boost::asio::serial_port_base::baud_rate(115200));

	//reset IRQ flipflop - on error 106 read 1, this code resets 106 to 0
	gpio_set(106,-1);
	gpio_set(107,0);
	gpio_set(107,1);
	//all select lines inactive
	gpio_set(68,1);
	gpio_set(69,1);
	gpio_set(70,1);
	gpio_set(71,1);

	uint8_t reply[256];

	motor_cmd_init:
	for(int m=0;m<4;m++) {

		//
		gpio_set(68+m,-1);

		//
		motorboard_cmd(&port,0xe0,reply,2);

		//
		if(reply[0]!=0xe0 || reply[1]!=0x00)
		{
			printf("error motor%d cmd=0x%02x reply=0x%02x\n",m+1,(int)reply[0],(int)reply[1]);
			goto motor_cmd_init;
		}

		//
		motorboard_cmd(&port,m+1,reply,1);

		//
		gpio_set(68+m,1);
	}

	//all select lines active
	gpio_set(68,-1);
	gpio_set(69,-1);
	gpio_set(70,-1);
	gpio_set(71,-1);

	//start multicast
	motorboard_cmd(&port,0xa0,reply,1);
	motorboard_cmd(&port,0xa0,reply,1);
	motorboard_cmd(&port,0xa0,reply,1);
	motorboard_cmd(&port,0xa0,reply,1);
	motorboard_cmd(&port,0xa0,reply,1);

	//reset IRQ flipflop - on error 106 read 1, this code resets 106 to 0
	gpio_set(106,-1);
	gpio_set(107,0);
	gpio_set(107,1);



	const uint16_t pwm_min=0x00;
	const uint16_t pwm_max=0x1ff;

	//initialize motors speeds
	motor_set_speed_t speeds_pwm;
	speeds_pwm.front_left=0;
	speeds_pwm.front_right=0;
	speeds_pwm.rear_left=0;
	speeds_pwm.rear_right=0;
	motor_speed_queue.push(speeds_pwm);

	printf("motors ready, wait for commands\r\n");

	for(;;){
		if(motor_speed_queue.pop(speeds_pwm)){
			//clamp
			speeds_pwm.front_left =constraint_s16(speeds_pwm.front_left,pwm_min,pwm_max);
			speeds_pwm.front_right=constraint_s16(speeds_pwm.front_right,pwm_min,pwm_max);
			speeds_pwm.rear_left  =constraint_s16(speeds_pwm.rear_left,pwm_min,pwm_max);
			speeds_pwm.rear_right =constraint_s16(speeds_pwm.rear_right,pwm_min,pwm_max);
		}
		//
		uint16_t pwm[4];
		pwm[0]=speeds_pwm.front_left;
		pwm[1]=speeds_pwm.front_right;
		pwm[2]=speeds_pwm.rear_right;
		pwm[3]=speeds_pwm.rear_left;
		//
		uint8_t cmd[5];
		cmd[0] = 0x20 | ((pwm[0]&pwm_max)>>4);
		cmd[1] = ((pwm[0]&pwm_max)<<4) | ((pwm[1]&pwm_max)>>5);
		cmd[2] = ((pwm[1]&pwm_max)<<3) | ((pwm[2]&pwm_max)>>6);
		cmd[3] = ((pwm[2]&pwm_max)<<2) | ((pwm[3]&pwm_max)>>7);
		cmd[4] = ((pwm[3]&pwm_max)<<1);
		//
		boost::asio::write(port, boost::asio::buffer(&cmd, 5));
		//boost::this_thread::sleep(boost::posix_time::milliseconds(50));
		boost::system::error_code error;
		//int ret =
		port.read_some(boost::asio::buffer(reply,sizeof(reply)), error);
		/*
		printf("ret=%d [",ret);
		for(int i=0;i<ret;i++){
			printf("%02x",reply[i]);
		}
		printf("]\r\n");
		 */
	}
}

void motor_test(void){

	int time_active = 2000;
	motor_set_speed_t speeds_pwm;
	uint16_t test_speed = 15;

	uint32_t round=0;
	for(;;){

		printf("-------------- ROUND %d ---------------- \r\n",round);
		round++;

		printf("front_left\r\n");
		speeds_pwm.front_left=test_speed;
		speeds_pwm.front_right=0;
		speeds_pwm.rear_left=0;
		speeds_pwm.rear_right=0;
		motor_speed_queue.push(speeds_pwm);
		boost::this_thread::sleep(boost::posix_time::milliseconds(time_active));

		printf("front_right\r\n");
		speeds_pwm.front_left=0;
		speeds_pwm.front_right=test_speed;
		speeds_pwm.rear_left=0;
		speeds_pwm.rear_right=0;
		motor_speed_queue.push(speeds_pwm);
		boost::this_thread::sleep(boost::posix_time::milliseconds(time_active));

		printf("rear_right\r\n");
		speeds_pwm.front_left=0;
		speeds_pwm.front_right=0;
		speeds_pwm.rear_left=0;
		speeds_pwm.rear_right=test_speed;
		motor_speed_queue.push(speeds_pwm);
		boost::this_thread::sleep(boost::posix_time::milliseconds(time_active));

		printf("rear_left\r\n");
		speeds_pwm.front_left=0;
		speeds_pwm.front_right=0;
		speeds_pwm.rear_left=test_speed;
		speeds_pwm.rear_right=0;
		motor_speed_queue.push(speeds_pwm);
		boost::this_thread::sleep(boost::posix_time::milliseconds(time_active));

	}

}

void motor_stop(void){
	motor_set_speed_t speeds_pwm;
	speeds_pwm.front_left=0;
	speeds_pwm.front_right=0;
	speeds_pwm.rear_left=0;
	speeds_pwm.rear_right=0;
	motor_speed_queue.push(speeds_pwm);
}



void drone_console_pilot(void){

	printf("build %s %s\r\n",__DATE__,__TIME__);
	printf("|-----------------------|\r\n");
	printf("|  Q = quit             |\r\n");
	printf("|  <space> = min speed  |\r\n");
	printf("|                       |\r\n");
	printf("|     ^          ^      |\r\n");
	printf("|     E          I      |\r\n");
	printf("| < S   D >  < J   K >  |\r\n");
	printf("|     X          M      |\r\n");
	printf("|    \\_/        \\_/     |\r\n");
	printf("|-----------------------|\r\n");

	int16_t height_speed=0;
	int16_t pitch_speed=0;
	int16_t roll_speed=0;
	int16_t yaw_speed=0;

	motor_set_speed_t speeds_user;
	speeds_user.front_left=0;
	speeds_user.front_right=0;
	speeds_user.rear_left=0;
	speeds_user.rear_right=0;


	// POSIX functions to turn off canonical mode on the input stream
	termios tos;
	tcgetattr(0, &tos);
	tos.c_lflag &= ~ICANON; // or cfmakeraw(&tos) but it's not POSIX
	tcsetattr(0, TCSANOW, &tos);

	char c;
	while(std::cin.get(c))
	{
		if(c=='e') pitch_speed+=1;
		if(c=='x') pitch_speed-=1;

		if(c=='s') roll_speed-=1;
		if(c=='d') roll_speed+=1;

		if(c=='j') yaw_speed-=1;
		if(c=='k') yaw_speed+=1;

		if(c=='i') height_speed+=1;
		if(c=='m') height_speed-=1;

		if(c==' '){
			height_speed=0;
			pitch_speed=0;
			roll_speed=0;
			yaw_speed=0;
		}

		//clamp
		height_speed=constraint_s16(height_speed,    -511  ,  511  );
		pitch_speed =constraint_s16(pitch_speed ,    -511  ,  511  );
		roll_speed  =constraint_s16(roll_speed  ,    -511  ,  511  );
		yaw_speed   =constraint_s16(yaw_speed   ,    -511  ,  511  );

		printf(" > %d %d %d %d\r\n",height_speed,pitch_speed,roll_speed,yaw_speed);

		//mix table
		speeds_user.front_left  = height_speed -pitch_speed + roll_speed - yaw_speed;
		speeds_user.front_right = height_speed -pitch_speed - roll_speed + yaw_speed;
		speeds_user.rear_left   = height_speed +pitch_speed + roll_speed + yaw_speed;
		speeds_user.rear_right  = height_speed +pitch_speed - roll_speed - yaw_speed;

		//send to motors
		motor_speed_queue.push(speeds_user);

	}




}







void murix_drone_start(void){

	boost::thread::attributes attrs;
	attrs.set_stack_size(4096*10);
	//boost::thread th(attrs, find_the_question, 42);

	//threads group
	boost::thread_group ardrone_threads;
	//create threads
	//ardrone_threads.create_thread(thread_navboard_read_raw);
	//ardrone_threads.create_thread(thread_navboard_decoder);
	//	ardrone_threads.create_thread(thread_stabilizer);
	//ardrone_threads.create_thread(drone_motors);
	//ardrone_threads.create_thread(motor_test);
	//ardrone_threads.create_thread(drone_console_pilot);
	//ardrone_threads.create_thread(thread_vbat);
	//ardrone_threads.create_thread(vbat_show);

	boost::thread_group cam_h;
	cam_h.create_thread(camera_horizontal);

	//boost::this_thread::sleep(boost::posix_time::milliseconds(5000));

	boost::thread_group cam_v;
	cam_v.create_thread(camera_vertical);



	//wait all threads
	ardrone_threads.join_all();
	cam_h.join_all();
	cam_v.join_all();
}

}


int main(int argc, char *argv[]) {

	murix_utils::singleton(argv[0]);

	system("killall -9 program.elf");
	system("sysctl -w kernel.panic=0");
	system("sysctl -w kernel.panic_on_oops=0");

	//murix_utils::daemonize();

	murix_ardrone::murix_drone_start();

	return 0;
}
