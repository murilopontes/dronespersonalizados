

#include <stdio.h>
#include <stdlib.h>
#include <boost/thread.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <boost/atomic.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/median.hpp>

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



/*
boost::atomic_int producer_count(0);
boost::atomic_int consumer_count(0);
boost::atomic<bool> done (false);
*/

boost::lockfree::queue<navboard_packet_t> navboard_queue(1);


//val=0 -> set gpio output lo
//val=1 -> set gpio output hi
//val=-1 -> set gpio as input (output hi-Z)
int gpio_set(int nr,int val)
{
	char cmdline[200];
	if(val<0) sprintf(cmdline,"/usr/sbin/gpio %d -d i",nr);
	else if(val>0) sprintf(cmdline,"/usr/sbin/gpio %d -d ho 1",nr);
	else sprintf(cmdline,"/usr/sbin/gpio %d -d ho 0",nr);
	return system(cmdline);
}

void navboard_producer(void)
{
	boost::asio::io_service io;
	boost::asio::serial_port port(io);


	port.open("/dev/ttyPA2");
	port.set_option(boost::asio::serial_port_base::baud_rate(460800));

	uint8_t c;

	int navboard_size=0x32;
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

} navboard_fusion_t;

typedef struct {
	double acc_x_offset;
	double acc_y_offset;
	double acc_z_offset;
	double gyro_x_offset;
	double gyro_y_offset;
	double gyro_z_offset;
} navboard_calibration_t;

double navboard_constraint(double value,double min,double max){
	double out=value;
	if(out<min)out=min;
	if(out>max)out=max;
	return out;
}

void navboard_sensor_fusion(navboard_packet_t* packet,navboard_fusion_t* fusion,double dt,navboard_calibration_t* calibration){

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
	fusion->gyro_x_integrate=navboard_constraint(fusion->gyro_x_integrate,-180,180);
	fusion->gyro_y_integrate=navboard_constraint(fusion->gyro_y_integrate,-180,180);
	fusion->gyro_z_integrate=navboard_constraint(fusion->gyro_z_integrate,-180,180);

	//////////////////////////////////////////////////////////////////////////////
	//use The complementary filter for pitch and roll
	fusion->fusion_pitch= 0.98*(fusion->fusion_pitch+fusion->gyro_x_dt)+(0.02*fusion->acc_pitch);
	fusion->fusion_roll = 0.98*(fusion->fusion_roll+fusion->gyro_y_dt)+(0.02*fusion->acc_roll);

	//use integrative for yaw
	fusion->fusion_yaw =  fusion->gyro_z_integrate;

	//////////////////////////////////////////////////////////////////////////////
	//clamp fusion

	fusion->fusion_pitch=navboard_constraint(fusion->fusion_pitch,-180,180);
	fusion->fusion_roll=navboard_constraint(fusion->fusion_roll,-180,180);
	fusion->fusion_yaw=navboard_constraint(fusion->fusion_yaw,-180,180);

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

void navboard_consumer(void){

	navboard_packet_t packet;
	navboard_fusion_t fusion;
	navboard_calibration_t calibration;

	boost::posix_time::ptime tick_back;
	boost::posix_time::ptime tick_now;
	boost::posix_time::time_duration tick_diff;

	tick_back = boost::posix_time::microsec_clock::local_time();
	tick_now = boost::posix_time::microsec_clock::local_time();

	//
	navboard_calibration(&calibration);

	for(;;){
		if(navboard_queue.pop(packet)){

			///////////////////////////////////////////////////////////////
			tick_back = tick_now;
			tick_now = boost::posix_time::microsec_clock::local_time();
			tick_diff = tick_now - tick_back;
			double dt = tick_diff.total_microseconds() / 1000000.0f;
			//////////////////////////////////////////////////////////////

			navboard_sensor_fusion(&packet,&fusion,dt,&calibration);

			printf("dt=%.6f seq=%d pitch=%.2f roll=%.2f g(%.2f|%.2f|%.2f) gi(%.2f|%.2f|%.2f) f(%.2f|%.2f|%.2f)\r\n",
					dt,
					packet.seq,
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
					fusion.fusion_yaw
			);





		}
	}

}


int main(int argc, char *argv[]) {

	system("killall -9 program.elf");
	system("sysctl -w kernel.panic=0");
	system("sysctl -w kernel.panic_on_oops=0");


	//threads group
	boost::thread_group ardrone_threads;

	//create threads
	ardrone_threads.create_thread(navboard_producer);
	ardrone_threads.create_thread(navboard_consumer);


	//wait all threads
	ardrone_threads.join_all();



	return 0;
}
