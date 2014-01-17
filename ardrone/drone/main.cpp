
//POSIX
#include <termios.h>
#include <unistd.h>
#include <signal.h>

// i2c library
#include "i2c-dev.h"

// C library
#include <stdio.h>
#include <stdlib.h>

// C++ library
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Camera library
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

// Boost C++

//
#include <boost/bind.hpp>

//
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

//
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>

//
#include <boost/circular_buffer.hpp>

//
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>

//
#include <boost/atomic.hpp>

//
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/median.hpp>

//
#include <boost/date_time/gregorian/gregorian.hpp>



#include "singleton.h"
#include "daemonize.h"

int global_run_foverer=1;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void ctrlchandler(int) {
	printf("ctrlchandler\r\n");
	global_run_foverer=0;
}
void killhandler(int) {
	printf("killhandler\r\n");
	global_run_foverer=0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

long millis(void){
	boost::posix_time::ptime tick_back(boost::gregorian::date(1970,boost::gregorian::Jan,1));
	boost::posix_time::ptime tick_now = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration tick_diff;
	tick_diff = tick_now - tick_back;
	return tick_diff.total_milliseconds();
}

long micros(void){
	boost::posix_time::ptime tick_back(boost::gregorian::date(1970,boost::gregorian::Jan,1));
	boost::posix_time::ptime tick_now = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration tick_diff;
	tick_diff = tick_now - tick_back;
	return tick_diff.total_microseconds();
}

long nanos(void){
	boost::posix_time::ptime tick_back(boost::gregorian::date(1970,boost::gregorian::Jan,1));
	boost::posix_time::ptime tick_now = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration tick_diff;
	tick_diff = tick_now - tick_back;
	return tick_diff.total_nanoseconds();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



class PID {
public:
	/*working variables*/
	boost::posix_time::ptime lastTime;
	double Input, Output, Setpoint;
	double errSum, lastErr;
	double kp, ki, kd;

	//
	double autotune_ku;
	double autotune_pu;
	double autotune_input_back;
	double autotune_input_oscillation;

	PID(){
		//
		Input=0;
		Output=0;
		Setpoint=0;
		//
		lastTime = boost::posix_time::microsec_clock::local_time();
		errSum=0;
		lastErr=0;
		//
		kp=0;
		ki=0;
		kd=0;
		//
		autotune_ku=0;
		autotune_pu=0;
		autotune_input_back=Input;
	}



	void online_autotune(){

		autotune_input_oscillation=Input-autotune_input_back;

		//oscillation is not sufficient
		if(abs(autotune_input_oscillation)<1){
			//increase ku
			printf("ku=%.2f\r\n",autotune_ku);
			autotune_ku+=1;
		}
		//oscillation is ok
		else {
			//calcute tunning
			kp = 0.6*autotune_ku;
			ki = 2*kp/autotune_pu;
			kd = kp*autotune_pu/8.0f;
			printf("kp=%.2f ki=%.2f kd=%.2f\r\n",kp,ki,kd);
		}

	}

	void Compute(double input_now)
	{
		//
		autotune_input_back = Input;
		Input = input_now;
		online_autotune();


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

	/*
	void SetTunings(double Kp, double Ki, double Kd)
	{
		kp = Kp;
		ki = Ki;
		kd = Kd;
	}
	 */
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

boost::atomic<bool> motors_ready(false);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class vbat_t {
public:
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
} ;





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

boost::atomic<vbat_t> atomic_vbat;
boost::atomic<bool> vbat_ready(false);

void thread_vbat(void){

	/*
	/dev/i2c-0
	0×49 Atmel AT73C246 Power Management and Analog Companions (PMAAC)
	0×50 24C32WI eeprom
	0x5d Bottom camera

	/dev/i2c-1
	 0×21 Horizontal Camera – OmniVision ov7725
	 */

	printf("vbat init!\r\n");
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


	//Setpoint
	unsigned char v;
	vbat_t vbat;

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
	printf("vbat working!\r\n");
	while(global_run_foverer) {
		vbat_read(&vbat,fd);
		atomic_vbat = vbat;
		vbat_ready = true;
	}


}



void vbat_show(void){

	vbat_t vbat;

	while(!vbat_ready){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}

	while(global_run_foverer){

		vbat = atomic_vbat;

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





//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

typedef struct {
	uint8_t buf[614400];
} camera_image_horizontal_t;

typedef struct {
	uint8_t buf[50688];
} camera_image_vertical_t;

boost::mutex camera_mutex_h;
boost::mutex camera_mutex_v;
camera_image_horizontal_t camera_img_h;
camera_image_vertical_t camera_img_v;

void camera_process_image(const void *p, int size)
{
	if(size==614400){
		boost::mutex::scoped_lock lock_h(camera_mutex_h, boost::try_to_lock);
		if(lock_h){
			memcpy(&camera_img_h.buf,p,size);
		} else {
			printf("fail to lock img_h\r\n");
		}
	}
	if(size==50688){
		boost::mutex::scoped_lock lock_v(camera_mutex_v, boost::try_to_lock);
		if(lock_v){
			memcpy(&camera_img_v.buf,p,size);
		} else {
			printf("fail to lock img_v\r\n");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct  {
	void   *start;
	size_t  length;
} camera_buffer_t;

typedef struct {
	char* dev_name;
	int width;
	int height;
	//////////////////////////
	int fd;
	camera_buffer_t *buffers;
	int n_buffers;
} camera_context_t;

int camera_xioctl(int fh, int request, void *arg,const char *name)
{
	int r = ioctl(fh, request, arg);
	if(r==-1){
		printf("ioctl fd=%d name=%s \033[1;31mFAIL\033[m\r\n",fh,name);
	} else {
		printf("ioctl fd=%d name=%s \033[1;32mOK\033[m\r\n",fh,name);
	}
	return r;
}


void camera_init(camera_context_t* ctx){

	///////////////////////////////////////////////////////////////////////////
	////////////////// ------ OPEN DEVICE

	//| O_NONBLOCK
	ctx->fd = open(ctx->dev_name, O_RDWR , 0);
	if(ctx->fd<0){
		printf("error cannot open device\r\n");
	}

	////////////////////////////////////////////////////////////////
	///////////////// ---------- DEVICE CAPS ----------------------

	struct v4l2_capability cap;
	memset(&cap,0,sizeof(cap));
	camera_xioctl(ctx->fd, VIDIOC_QUERYCAP, &cap,"VIDIOC_QUERYCAP");

	//////////////////////////////////////////////////////////////////////
	///////////////////--------- SET FORMAT

	struct v4l2_format fmt;
	memset(&fmt,0,sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
	fmt.fmt.pix.width=ctx->width;
	fmt.fmt.pix.height=ctx->height;
	camera_xioctl(ctx->fd, VIDIOC_S_FMT, &fmt,"VIDIOC_S_FMT");

	/////////////////////////////////////////////////////////////////
	/////////////// ----------------  REQUEST BUFFER ---------///////

	struct v4l2_requestbuffers req;
	memset(&req,0,sizeof(req));
	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	camera_xioctl(ctx->fd, VIDIOC_REQBUFS, &req,"VIDIOC_REQBUFS");

	/////////////////////////////////////////////////////////////////////////////
	////////////////--------------- MMAP INIT

	ctx->n_buffers=req.count;
	ctx->buffers = (camera_buffer_t*) calloc(ctx->n_buffers, sizeof(camera_buffer_t));

	for (int i = 0; i < ctx->n_buffers; ++i) {
		struct v4l2_buffer buf;
		memset(&buf,0,sizeof(buf));
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;
		camera_xioctl(ctx->fd, VIDIOC_QUERYBUF, &buf,"VIDIOC_QUERYBUF");
		ctx->buffers[i].length = buf.length;
		ctx->buffers[i].start = mmap(NULL,buf.length,PROT_READ | PROT_WRITE ,MAP_SHARED, ctx->fd, buf.m.offset);
	}

	/////////////////////////////////////////////////////////////////////
	////////////---------------- enqueue request

	for (int i = 0; i < ctx->n_buffers; ++i) {

		struct v4l2_buffer buf;
		memset(&buf,0,sizeof(buf));

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		camera_xioctl(ctx->fd, VIDIOC_QBUF, &buf,"VIDIOC_QBUF");
	}

	/////////////////////////////////////////////////////////////////////
	////////////---------------- START CAPTURE

	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	camera_xioctl(ctx->fd, VIDIOC_STREAMON, &type,"VIDIOC_STREAMON");
}

int camera_loop_check(camera_context_t* ctx){

	fd_set fds;
	struct timeval tv;
	int r;
	FD_ZERO(&fds);
	FD_SET(ctx->fd, &fds);
	/* Timeout. */
	tv.tv_sec = 2;
	tv.tv_usec = 0;

	r = select(ctx->fd + 1, &fds, NULL, NULL, &tv);

	if (-1 == r) {
		return -1;
	}

	if (0 == r) {
		return 0;
	}
	return 1;
}

void camera_loop_frame(camera_context_t* ctx){

	///////////////////////////////////////////////////
	////////////////////////////---------- DEQUEUE
	struct v4l2_buffer buf;
	memset(&buf,0,sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	if(camera_xioctl(ctx->fd, VIDIOC_DQBUF, &buf,"VIDIOC_DQBUF")!=-1){
		////////////////////////////////////////////////--- USE
		camera_process_image(ctx->buffers[buf.index].start, buf.bytesused);
		//////////////////////////////////////////////----RE-ENQUEUE
		camera_xioctl(ctx->fd, VIDIOC_QBUF, &buf,"VIDIOC_QBUF");
	}

}



void camera_close(camera_context_t* ctx){

	////////////////////////////////////////////////////////////////////////
	//////////////////////////-------- STOP CAPTURE

	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	camera_xioctl(ctx->fd, VIDIOC_STREAMOFF, &type,"VIDIOC_STREAMOFF");

	////////////////////////////////////////////////////////////////////////
	//////////////////////////-------- deinit mmap

	for (int i = 0; i < ctx->n_buffers; ++i){
		munmap(ctx->buffers[i].start, ctx->buffers[i].length);
	}
	free(ctx->buffers);

	////////////////////////////////////////////////////////////////////////
	//////////////////////////-------- CLOSE DEVICE

	close(ctx->fd);
}

void camera_h(void){
	camera_context_t ctx_h;
	ctx_h.dev_name = (char*)"/dev/video0";
	ctx_h.width=640;
	ctx_h.height=480;


	camera_init(&ctx_h);
	while(global_run_foverer){
		camera_loop_frame(&ctx_h);
	}
	camera_close(&ctx_h);

}

void camera_v(void){

	camera_context_t ctx_v;
	ctx_v.dev_name = (char*)"/dev/video1";
	ctx_v.width=176;
	ctx_v.height=144;

	camera_init(&ctx_v);
	while(global_run_foverer){
		camera_loop_frame(&ctx_v);
	}
	camera_close(&ctx_v);
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


class navboard_fusion_t {
public:


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

};

class navboard_calibration_t {
public:
	double acc_x_offset;
	double acc_y_offset;
	double acc_z_offset;
	double gyro_x_offset;
	double gyro_y_offset;
	double gyro_z_offset;
} ;

class motor_set_speed_t {
public:
	motor_set_speed_t(){
		front_left=0;
		front_right=0;
		rear_left=0;
		rear_right=0;
	}
	int16_t front_left;
	int16_t front_right;
	int16_t rear_left;
	int16_t rear_right;
} ;



boost::atomic<navboard_packet_t> atomic_navboard_raw;
boost::atomic<navboard_fusion_t> atomic_fusion;
boost::atomic<motor_set_speed_t> atomic_motor_speed;


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


boost::atomic<bool> navboard_ready(false);

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
	while(global_run_foverer){
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

				atomic_navboard_raw = packet;
				navboard_ready = true;

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


	//fusion->acc_pitch =  ( -atan2(fusion->acc_x,fusion->acc_z) * radian2degree );
	//fusion->acc_roll =   (  atan2(fusion->acc_y,fusion->acc_z) * radian2degree );

	double pitch_tmp = sqrt((fusion->acc_y*fusion->acc_y)+(fusion->acc_z*fusion->acc_z));

	//
	fusion->acc_pitch =  (  atan2(fusion->acc_x,pitch_tmp) * radian2degree );

	//
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
	fusion->gyro_x_integrate=constraint_double(fusion->gyro_x_integrate,-90,90);
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

	fusion->fusion_pitch=constraint_double(fusion->fusion_pitch,-90,90);
	fusion->fusion_roll=constraint_double(fusion->fusion_roll,-180,180);
	fusion->fusion_yaw=constraint_double(fusion->fusion_yaw,-180,180);

	/////////////////////////////////////////////////////////////////////////////

	if(packet->us_echo / 37.5f < 650){
		fusion->height=packet->us_echo / 37.5f;
	}

}

double calibration_ref(double value,double ref,double error){
	if(value<ref-error || value>ref+error){
		return ref;
	}
	return value;
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

	while(!navboard_ready){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}

	while(global_run_foverer){
		packet = atomic_navboard_raw;

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
	////////////////////////////////////////////////////////

	double tx = median(acc_raw_x);
	double ty = median(acc_raw_y);
	double tz = median(acc_raw_z);

	double gravitation = tz - ((tx+ty)/2.0f) ;

	calibration->acc_x_offset = tx;
	calibration->acc_y_offset = ty;
	calibration->acc_z_offset = tz - gravitation;
	///////////////////////////////////////////////////////
	calibration->gyro_x_offset = median(gyro_raw_x);
	calibration->gyro_y_offset = median(gyro_raw_y);
	calibration->gyro_z_offset = median(gyro_raw_z);
	//////////////////////////////////////////////////////

	printf("navboard_calibration before ao=(%.2f,%.2f,%.2f) go=(%.2f,%.2f,%.2f)\r\n",
			calibration->acc_x_offset,
			calibration->acc_y_offset,
			calibration->acc_z_offset,
			calibration->gyro_x_offset,
			calibration->gyro_y_offset,
			calibration->gyro_z_offset
	);

	//ao=(2063.16,1976.81,2019.99) go=(1658.00,1669.98,1663.00)
	double ref_ao_error = 5.0;
	double ref_ao_x = 2063.16;
	double ref_ao_y = 1976.81;
	double ref_ao_z = 2019.99;

	double ref_go_error = 5.0;
	double ref_go_x = 1658.00;
	double ref_go_y = 1669.98;
	double ref_go_z = 1663.00;

	calibration->acc_x_offset = calibration_ref(calibration->acc_x_offset,ref_ao_x,ref_ao_error);
	calibration->acc_y_offset = calibration_ref(calibration->acc_y_offset,ref_ao_y,ref_ao_error);
	calibration->acc_z_offset = calibration_ref(calibration->acc_z_offset,ref_ao_z,ref_ao_error);
	calibration->gyro_x_offset = calibration_ref(calibration->gyro_x_offset,ref_go_x,ref_go_error);
	calibration->gyro_y_offset = calibration_ref(calibration->gyro_y_offset,ref_go_y,ref_go_error);
	calibration->gyro_z_offset = calibration_ref(calibration->gyro_z_offset,ref_go_z,ref_go_error);


	printf("navboard_calibration after  ao=(%.2f,%.2f,%.2f) go=(%.2f,%.2f,%.2f)\r\n",
			calibration->acc_x_offset,
			calibration->acc_y_offset,
			calibration->acc_z_offset,
			calibration->gyro_x_offset,
			calibration->gyro_y_offset,
			calibration->gyro_z_offset
	);

}

boost::atomic<bool> fusion_ready(false);

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
	fusion.height=24;

	//
	while(!navboard_ready){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}

	//
	printf("navboard calibration wait...\r\n");
	navboard_calibration(&calibration);


	printf("navboard working!\r\n");
	while(global_run_foverer){

		packet = atomic_navboard_raw;

		///////////////////////////////////////////////////////////////
		tick_back = tick_now;
		tick_now = boost::posix_time::microsec_clock::local_time();
		tick_diff = tick_now - tick_back;
		double dt = tick_diff.total_nanoseconds() / 1000000000.0f;
		//////////////////////////////////////////////////////////////
		navboard_sensor_fusion(&packet,&fusion,dt,&calibration);

		atomic_fusion = fusion;
		fusion_ready = true;
		///////////////////////////////////////////////////////////////

	}
}

void navboard_and_vbat_udp_server(void){

	navboard_fusion_t fusion;
	vbat_t vbat;

	uint32_t count=0;
	enum { max_length = 1024 };
	char data[max_length];

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 4000));


	//
	while(!fusion_ready || !vbat_ready){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}

	printf("navboard_udp_show ok!\r\n");
	while(global_run_foverer)
	{

		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data, max_length), sender_endpoint);
		data[length]=0;
		count++;

		fusion = atomic_fusion;
		vbat = atomic_vbat;

		length=sprintf(data,"%f|%f|%f|%f",fusion.fusion_pitch,fusion.fusion_roll,fusion.fusion_yaw,vbat.vbat);
		//printf("navboard_udp_show=%d data=%s\r\n",count,data);

		sock.send_to(boost::asio::buffer(data, length), sender_endpoint);
	}


}

void navboard_show(void){

	navboard_fusion_t fusion;

	//
	while(!fusion_ready){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}


	while(global_run_foverer){
		fusion = atomic_fusion;

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




void motorboard_cmd(boost::asio::serial_port* port,uint8_t cmd, uint8_t*reply, int replylen){
	boost::asio::write(*port, boost::asio::buffer(&cmd, 1));
	boost::asio::read(*port, boost::asio::buffer(reply,replylen));
}




void drone_motors(void){



	//
	boost::asio::io_service io;
	boost::asio::serial_port port(io);
	port.open("/dev/ttyPA1");
	port.set_option(boost::asio::serial_port_base::baud_rate(115200));

	//
	motor_cmd_init:
	printf("motors init!\r\n");

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


	for(int m=0;m<4;m++) {

		//
		gpio_set(68+m,-1);

		//
		motorboard_cmd(&port,0xe0,reply,2);

		//
		if(reply[0]!=0xe0 || reply[1]!=0x00)
		{
			printf("motors init error motor=%d cmd=0x%02x reply=0x%02x\n",m+1,(int)reply[0],(int)reply[1]);
			boost::this_thread::sleep(boost::posix_time::milliseconds(50));
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

	atomic_motor_speed = speeds_pwm;


	printf("motors ready, wait for commands\r\n");

	while(global_run_foverer){

		speeds_pwm = atomic_motor_speed;

		//clamp
		speeds_pwm.front_left =constraint_s16(speeds_pwm.front_left,pwm_min,pwm_max);
		speeds_pwm.front_right=constraint_s16(speeds_pwm.front_right,pwm_min,pwm_max);
		speeds_pwm.rear_left  =constraint_s16(speeds_pwm.rear_left,pwm_min,pwm_max);
		speeds_pwm.rear_right =constraint_s16(speeds_pwm.rear_right,pwm_min,pwm_max);

		//map
		uint16_t pwm[4];
		pwm[0]=speeds_pwm.front_left;
		pwm[1]=speeds_pwm.front_right;
		pwm[2]=speeds_pwm.rear_right;
		pwm[3]=speeds_pwm.rear_left;

		//cmd
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

		motors_ready=true;
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
	while(global_run_foverer){

		printf("-------------- ROUND %d ---------------- \r\n",round);
		round++;

		printf("front_left\r\n");
		speeds_pwm.front_left=test_speed;
		speeds_pwm.front_right=0;
		speeds_pwm.rear_left=0;
		speeds_pwm.rear_right=0;
		atomic_motor_speed = speeds_pwm;
		boost::this_thread::sleep(boost::posix_time::milliseconds(time_active));

		printf("front_right\r\n");
		speeds_pwm.front_left=0;
		speeds_pwm.front_right=test_speed;
		speeds_pwm.rear_left=0;
		speeds_pwm.rear_right=0;
		atomic_motor_speed = speeds_pwm;
		boost::this_thread::sleep(boost::posix_time::milliseconds(time_active));

		printf("rear_right\r\n");
		speeds_pwm.front_left=0;
		speeds_pwm.front_right=0;
		speeds_pwm.rear_left=0;
		speeds_pwm.rear_right=test_speed;
		atomic_motor_speed = speeds_pwm;
		boost::this_thread::sleep(boost::posix_time::milliseconds(time_active));

		printf("rear_left\r\n");
		speeds_pwm.front_left=0;
		speeds_pwm.front_right=0;
		speeds_pwm.rear_left=test_speed;
		speeds_pwm.rear_right=0;
		atomic_motor_speed = speeds_pwm;
		boost::this_thread::sleep(boost::posix_time::milliseconds(time_active));

	}

}

void motor_stop(void){
	motor_set_speed_t speeds_pwm;
	speeds_pwm.front_left=0;
	speeds_pwm.front_right=0;
	speeds_pwm.rear_left=0;
	speeds_pwm.rear_right=0;
	atomic_motor_speed = speeds_pwm;
}




void drone_tcp_session(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
	try
	{
		for (;;)
		{
			char data[1024];
			boost::system::error_code error;
			size_t length = sock->read_some(boost::asio::buffer(data), error);

			if (error == boost::asio::error::eof){
				// Connection closed cleanly by peer.
				printf("connection drop eof\r\n");
				break;
			}
			else if (error){
				// Some other error.
				throw boost::system::system_error(error);
			}
			data[length]=0;
			printf("tcp msg=%s\n",data);
			boost::asio::write(*sock, boost::asio::buffer(data, length));
		}

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void drone_tcp_server(void){
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::acceptor a(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3000));
	while(global_run_foverer)
	{
		boost::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(io_service));
		a.accept(*sock);
		boost::thread t(boost::bind(drone_tcp_session, sock));
	}
}

class drone_rc_command {
public:

	drone_rc_command(){
		height_speed=0;
		pitch_speed=0;
		roll_speed=0;
		yaw_speed=0;
		emergency=0;
		takeoff=0;
	}

	int16_t emergency;
	int16_t takeoff;

	int16_t height_speed;
	int16_t pitch_speed;
	int16_t roll_speed;
	int16_t yaw_speed;

};

boost::atomic<drone_rc_command> atomic_cmd;


void drone_udp_server_parser(char* string){

	const char delimiters[] = "|";
	char *running;
	char *token;

	running = string;

	drone_rc_command cmd;

	int i=0;
	while(true){
		token = strsep (&running, delimiters);    /* token => "words" */
		if(token==NULL) break;
		//printf("token[%d]=%s len=%d\r\n",i,token,strlen(token));

		if(i==0) cmd.height_speed=atoi(token);
		if(i==1) cmd.pitch_speed=atoi(token);
		if(i==2) cmd.roll_speed=atoi(token);
		if(i==3) cmd.yaw_speed=atoi(token);
		if(i==4) cmd.takeoff=atoi(token);
		if(i==5) cmd.emergency=atoi(token);
		i++;
	}

	atomic_cmd = cmd;
}


void joystick_receive_udp_server(void){

	uint32_t count=0;
	enum { max_length = 1024 };
	char data[max_length];

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 3000));
	while(global_run_foverer)
	{

		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data, max_length), sender_endpoint);
		data[length]=0;
		count++;

		//printf("udp [%d] msg=%s\n",count,data);
		drone_udp_server_parser(data);

		sock.send_to(boost::asio::buffer(data, length), sender_endpoint);
	}

}











































void pilot_using_joystick_only(void){

	//
	int16_t height_speed=0;
	int16_t pitch_speed=0;
	int16_t roll_speed=0;
	int16_t yaw_speed=0;

	//
	motor_set_speed_t speeds_user;
	speeds_user.front_left=0;
	speeds_user.front_right=0;
	speeds_user.rear_left=0;
	speeds_user.rear_right=0;

	int takeoff=0;

	//
	drone_rc_command cmd;
	while(global_run_foverer){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));

		cmd = atomic_cmd;

			if(cmd.takeoff){
				takeoff=1;
				printf("takeoff\r\n");
			}

			if(cmd.emergency){
				printf("emergency\r\n");
				//
				height_speed=0;
				pitch_speed=0;
				roll_speed=0;
				yaw_speed=0;
				takeoff=0;
			}


			if(takeoff){
				//
				pitch_speed=constraint_s16(cmd.pitch_speed,-200,200);
				roll_speed=constraint_s16(cmd.roll_speed,-200,200);
				yaw_speed=constraint_s16(cmd.yaw_speed,-1,1);
				height_speed+=constraint_s16(cmd.height_speed,-3,3);

				//clamp
				height_speed=constraint_s16(height_speed,      50  ,  511  );
				pitch_speed =constraint_s16(pitch_speed ,    -511  ,  511  );
				roll_speed  =constraint_s16(roll_speed  ,    -511  ,  511  );
				yaw_speed   =constraint_s16(yaw_speed   ,    -511  ,  511  );
			}



			//mix table
			speeds_user.front_left  = height_speed +constraint_s16((-pitch_speed + roll_speed - yaw_speed),0,511);
			speeds_user.front_right = height_speed +constraint_s16((-pitch_speed - roll_speed + yaw_speed),0,511);
			speeds_user.rear_left   = height_speed +constraint_s16((+pitch_speed + roll_speed + yaw_speed),0,511);
			speeds_user.rear_right  = height_speed +constraint_s16((+pitch_speed - roll_speed - yaw_speed),0,511);
			//send to motors


			atomic_motor_speed=speeds_user;
			printf("udp pilot > %d %d %d %d\r\n",height_speed,pitch_speed,roll_speed,yaw_speed);



	}
}


void pilot_using_keyboard_only(void){

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
			//
			height_speed=0;
			pitch_speed=0;
			roll_speed=0;
			yaw_speed=0;
		} else {
			//clamp
			height_speed=constraint_s16(height_speed,      10  ,  511  );
			pitch_speed =constraint_s16(pitch_speed ,    -511  ,  511  );
			roll_speed  =constraint_s16(roll_speed  ,    -511  ,  511  );
			yaw_speed   =constraint_s16(yaw_speed   ,    -511  ,  511  );
		}

		//mix table
		speeds_user.front_left  = height_speed +constraint_s16((-pitch_speed + roll_speed - yaw_speed),0,511);
		speeds_user.front_right = height_speed +constraint_s16((-pitch_speed - roll_speed + yaw_speed),0,511);
		speeds_user.rear_left   = height_speed +constraint_s16((+pitch_speed + roll_speed + yaw_speed),0,511);
		speeds_user.rear_right  = height_speed +constraint_s16((+pitch_speed - roll_speed - yaw_speed),0,511);
		//send to motors
		atomic_motor_speed=speeds_user;

		printf(" > %d %d %d %d\r\n",height_speed,pitch_speed,roll_speed,yaw_speed);

	}




}


void pilot_using_joystick_with_stabilizer(void){

	navboard_fusion_t fusion;

	//
	while(!fusion_ready || !motors_ready){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}

	//
	int16_t height_speed=0;
	int16_t pitch_speed=0;
	int16_t roll_speed=0;
	int16_t yaw_speed=0;

	//
	motor_set_speed_t speeds_user;
	speeds_user.front_left=0;
	speeds_user.front_right=0;
	speeds_user.rear_left=0;
	speeds_user.rear_right=0;

	double roll_error = 0;

	while(global_run_foverer){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));

		fusion = atomic_fusion;

		roll_error = fusion.fusion_roll - 0;

		if(roll_error>1){
			roll_speed-= 1;
		}
		if(roll_error<1){
			roll_speed+= 1;
		}

		//clamp
		height_speed=constraint_s16(height_speed,      50  ,  511  );
		pitch_speed =constraint_s16(pitch_speed ,    -511  ,  511  );
		roll_speed  =constraint_s16(roll_speed  ,    -190  ,  190  );
		yaw_speed   =constraint_s16(yaw_speed   ,    -511  ,  511  );


		//mix table
		speeds_user.front_left  = height_speed +constraint_s16((-pitch_speed + roll_speed - yaw_speed),0,511);
		speeds_user.front_right = height_speed +constraint_s16((-pitch_speed - roll_speed + yaw_speed),0,511);
		speeds_user.rear_left   = height_speed +constraint_s16((+pitch_speed + roll_speed + yaw_speed),0,511);
		speeds_user.rear_right  = height_speed +constraint_s16((+pitch_speed - roll_speed - yaw_speed),0,511);

		//send to motors
		atomic_motor_speed = speeds_user;

		printf("navboard pilot > %d %d %d %d (%f)\r\n",height_speed,pitch_speed,roll_speed,yaw_speed,roll_error);


	}
}





/*
# program.elf
posix init start build on : Aug 20 2012 14:37:42
Use ctrl+\ (SIGQUIT) to end the application
plog drop: thread RT_THRESHOLD=19, SUP_THREAD_MINPRIORITY=31
plog drop: RR time quantum 0s 10000000ns
plog drop: thread guard size : default 4096
plog drop: thread guard size : set to 65536
plog drop: thread stack size : default 32768, minimal 16384, (system default 8388608)
plog drop: disable smp

 */

int main(int argc, char *argv[]) {

	//
	system("killall -9 program.elf");
	system("sysctl -w kernel.panic=0");
	system("sysctl -w kernel.panic_on_oops=0");

	//
	murix_utils::singleton(argv[0]);

	//
	//signal(SIGINT, ctrlchandler);
	//signal(SIGTERM, killhandler);

	//
	boost::thread_group ardrone_threads;


	////------------- cameras ----------------------
	//ardrone_threads.create_thread(camera_v);
	//ardrone_threads.create_thread(camera_h);

	////-------------- vbat ----------------------
	ardrone_threads.create_thread(thread_vbat);
	//ardrone_threads.create_thread(vbat_show);

	////--------------- navboard --------------------------------
	ardrone_threads.create_thread(thread_navboard_read_raw);
	ardrone_threads.create_thread(thread_navboard_decoder);
	//ardrone_threads.create_thread(navboard_show);

	////--------------- motors ----------------------------------
	ardrone_threads.create_thread(drone_motors);
	//ardrone_threads.create_thread(motor_test);

	////---------------- UDP network -------------------------------
	//ardrone_threads.create_thread(drone_tcp_server);
	ardrone_threads.create_thread(joystick_receive_udp_server);
	ardrone_threads.create_thread(navboard_and_vbat_udp_server);

	////----------------- Pilots -----------------------------------
	//ardrone_threads.create_thread(pilot_using_keyboard_only);
	ardrone_threads.create_thread(pilot_using_joystick_only);
	//ardrone_threads.create_thread(pilot_using_joystick_with_stabilizer);



	ardrone_threads.join_all();
	printf("all threads clean exit! :)\r\n");

	return 0;
}
