/*
 * murix_main.cpp
 *
 *  Created on: 02/12/2013
 *      Author: mpontes
 */


#include "murix_cpp_headers.h"
#include "murix_boost_headers.h"


//POSIX
#include <termios.h>
#include <unistd.h>



//
#include "murix_singleton.h"
#include "murix_daemonize.h"
#include "murix_camera.h"
#include "murix_arduino.h"
#include "murix_vbat.h"
#include "murix_http_server.h"
#include "murix_pid.h"
#include "murix_constraint.h"
#include "murix_gpio.h"
#include "murix_joystick.h"


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int global_run_foverer=1;

void handler_abrt(int) {
	printf("handler_abrt\r\n");
	global_run_foverer=0;
}

void handler_int(int) {
	printf("handler_int\r\n");
	global_run_foverer=0;
}
void handler_term(int) {
	printf("handler_term\r\n");
	global_run_foverer=0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

double radian2degree(double radian){
	return radian * (180.0f / M_PI);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////






class motor_speed_t {
public:
	motor_speed_t(){
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

boost::atomic<motor_speed_t>     atomic_motor_speed;
boost::atomic<bool> motors_ready(false);

void motors_cmd_io(boost::asio::serial_port* port,uint8_t cmd, uint8_t*reply, int replylen){
	boost::asio::write(*port, boost::asio::buffer(&cmd, 1));
	boost::asio::read(*port, boost::asio::buffer(reply,replylen));
}

void motors_cmd_consumer(void){



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
		motors_cmd_io(&port,0xe0,reply,2);

		//
		if(reply[0]!=0xe0 || reply[1]!=0x00)
		{
			printf("motors init error motor=%d cmd=0x%02x reply=0x%02x\n",m+1,(int)reply[0],(int)reply[1]);
			boost::this_thread::sleep(boost::posix_time::milliseconds(50));
			goto motor_cmd_init;
		}

		//
		motors_cmd_io(&port,m+1,reply,1);

		//
		gpio_set(68+m,1);
	}

	//all select lines active
	gpio_set(68,-1);
	gpio_set(69,-1);
	gpio_set(70,-1);
	gpio_set(71,-1);

	//start multicast
	motors_cmd_io(&port,0xa0,reply,1);
	motors_cmd_io(&port,0xa0,reply,1);
	motors_cmd_io(&port,0xa0,reply,1);
	motors_cmd_io(&port,0xa0,reply,1);
	motors_cmd_io(&port,0xa0,reply,1);

	//reset IRQ flipflop - on error 106 read 1, this code resets 106 to 0
	gpio_set(106,-1);
	gpio_set(107,0);
	gpio_set(107,1);



	const uint16_t pwm_min=0x00;
	const uint16_t pwm_max=0x1ff;

	//initialize motors speeds
	motor_speed_t speeds_pwm;
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

void motors_sequential_test(void){

	int time_active = 2000;
	motor_speed_t speeds_pwm;
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

void motors_stop_all(void){
	motor_speed_t speeds_pwm;
	speeds_pwm.front_left=0;
	speeds_pwm.front_right=0;
	speeds_pwm.rear_left=0;
	speeds_pwm.rear_right=0;
	atomic_motor_speed = speeds_pwm;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

typedef struct
{
	//
	uint16_t size;                // +0x00 Size of the following data (always 0x2C)
	uint16_t seq;                 // +0x02 Sequence number, increases every update

	//Bosh BMA150
	uint16_t acc_x;              // +0x04 Raw data (10-bit) of the accelerometers multiplied by 4
	uint16_t acc_y;              // +0x04 Raw data (10-bit) of the accelerometers multiplied by 4
	uint16_t acc_z;              // +0x04 Raw data (10-bit) of the accelerometers multiplied by 4

	//Invensense IDG-500 max=+-500 - 2mV g/s  / +-110 - 9.1mV g/s
	uint16_t gyro_x;             // +0x0A Raw data for the gyros, 12-bit A/D converted voltage of the gyros. X,Y=IDG, Z=Epson
	uint16_t gyro_y;             // +0x0A Raw data for the gyros, 12-bit A/D converted voltage of the gyros. X,Y=IDG, Z=Epson

	//Epson XV-3500CB max=100 degrees/s ---- 0.67 mV/(degree/s)
	uint16_t gyro_z;

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
} navboard_raw_t;

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


	double height;

	double dt;
	double seq;

};

class navboard_raw_calibration_t {
public:
	double acc_x_offset;
	double acc_y_offset;
	double acc_z_offset;
	double gyro_x_offset;
	double gyro_y_offset;
	double gyro_z_offset;
	double gyro_gain;

	navboard_raw_calibration_t(){

		acc_x_offset=2060; // 1000 samples average in ground
		acc_y_offset=1963; // 1000 samples average in ground
		acc_z_offset=2041; // 1000 samples average in wall

		gyro_x_offset=1659; // 1000 samples average in ground
		gyro_y_offset=1664; // 1000 samples average in ground
		gyro_z_offset=1662; // 1000 samples average in ground

		gyro_gain = (180.0f / M_PI) / 10.0f;
	}
} ;




boost::atomic<navboard_raw_calibration_t> atomic_navboard_raw_calibration;
boost::atomic<navboard_raw_t> atomic_navboard_raw;
boost::atomic<navboard_fusion_t> atomic_navboard_fusion;
boost::atomic<bool> navboard_raw_ready(false);
boost::atomic<bool> navboard_fusion_ready(false);



void navboard_read_raw(void)
{


	printf("navboard reader init\r\n");


	//
	boost::asio::io_service io;
	boost::asio::serial_port port(io);
	port.open("/dev/ttyPA2");
	port.set_option(boost::asio::serial_port_base::baud_rate(460800));


	//
	uint8_t navboard_size=0x32;
	uint8_t c;
	boost::circular_buffer<uint8_t> cb(navboard_size);
	//
	navboard_raw_t packet;
	uint8_t navboard8[navboard_size];
	uint16_t* p_navboard16;
	p_navboard16 = (uint16_t*)& navboard8;

	//set /MCLR pin -> reset navboard
	gpio_set(132,1);

	//start acquisition
	uint8_t cmd=0x01;
	boost::asio::write(port, boost::asio::buffer(&cmd, 1));

	//
	if(sizeof(navboard_raw_t)!=sizeof(navboard8)){
		printf("%d!=%d\r\n",sizeof(navboard_raw_t),sizeof(navboard8));
		printf("error %s:%d\r\n",__FILE__,__LINE__);
		exit(1);
	}

	//
	printf("navboard reader working\r\n");
	while(global_run_foverer){
		//read
		boost::asio::read(port, boost::asio::buffer(&c,1));
		//put in circular buffer
		cb.push_back(c);
		//buffer is full
		if(cb.size()==navboard_size){
			//header of buffer is navboard signature
			if(cb[0]==navboard_size && cb[1]==0x0 && cb[31]==0x0 && cb[30]==0x1
			){
				//copy packet to linear buffer
				for(int i=0;i<navboard_size;i++){
					navboard8[i]=cb[i];
				}
				memcpy(&packet,&navboard8,sizeof(packet));
				atomic_navboard_raw = packet;
				navboard_raw_ready = true;
			}
		}
	}
	//
	port.close();
}





void navboard_raw_to_fusion(navboard_raw_t* packet,navboard_fusion_t* fusion,double dt){

	//
	navboard_raw_calibration_t navboard_calibration;
	navboard_calibration = atomic_navboard_raw_calibration;

	fusion->dt = dt;
	fusion->seq = packet->seq;

	//offset and scale
	fusion->acc_x= (packet->acc_x - navboard_calibration.acc_x_offset);
	fusion->acc_y= (packet->acc_y - navboard_calibration.acc_y_offset);
	fusion->acc_z= (packet->acc_z - navboard_calibration.acc_z_offset);

	//
	fusion->gyro_x = (packet->gyro_x - navboard_calibration.gyro_x_offset)/navboard_calibration.gyro_gain;
	fusion->gyro_y = (packet->gyro_y - navboard_calibration.gyro_y_offset)/navboard_calibration.gyro_gain;
	fusion->gyro_z = (packet->gyro_z - navboard_calibration.gyro_z_offset)/navboard_calibration.gyro_gain;

	///////////////////////////////////////////////////////////////////////////////////

	double pitch_tmp = sqrt((fusion->acc_y*fusion->acc_y)+(fusion->acc_z*fusion->acc_z));

	//
	fusion->acc_pitch =  radian2degree(  atan2(fusion->acc_x,pitch_tmp)  );

	//
	fusion->acc_roll =   radian2degree(  atan2(fusion->acc_y,fusion->acc_z)  );


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

	fusion->fusion_pitch=constraint_double(fusion->fusion_pitch,-90,90);
	fusion->fusion_roll=constraint_double(fusion->fusion_roll,-180,180);

	/////////////////////////////////////////////////////////////////////////////

	if(packet->us_echo / 37.5f < 650){
		fusion->height=packet->us_echo / 37.5f;
	}

}



void navboard_consumer_raw_procude_fusion(void){

	//
	navboard_raw_t navboard_raw;
	navboard_fusion_t navboard_fusion;

	//
	boost::posix_time::ptime tick_back;
	boost::posix_time::ptime tick_now;
	boost::posix_time::time_duration tick_diff;

	//
	tick_back = boost::posix_time::microsec_clock::local_time();
	tick_now = boost::posix_time::microsec_clock::local_time();

	//
	navboard_fusion.height=24;

	//
	while(!navboard_raw_ready){
		printf("wait navboard_raw_ready\r\n");
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}

	printf("navboard working!\r\n");
	while(global_run_foverer){
		///////////////////////////////////////////////////////////////
		navboard_raw = atomic_navboard_raw;
		///////////////////////////////////////////////////////////////
		tick_back = tick_now;
		tick_now = boost::posix_time::microsec_clock::local_time();
		tick_diff = tick_now - tick_back;
		double dt = tick_diff.total_nanoseconds() / 1000000000.0f;
		//////////////////////////////////////////////////////////////
		navboard_raw_to_fusion(&navboard_raw,&navboard_fusion,dt);
		atomic_navboard_fusion = navboard_fusion;
		navboard_fusion_ready = true;
		///////////////////////////////////////////////////////////////
	}
}


void navboard_and_motors_and_vbat_udp_server(void){

	navboard_fusion_t fusion;
	vbat_t vbat;
	motor_speed_t motor_speed;
	navboard_raw_calibration_t calib;


	uint32_t count=0;
	enum { max_length = 1024 };
	char data[max_length];

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 4000));


	//
	while(!navboard_fusion_ready || !atomic_vbat_ready){
		printf("wait navboard_fusion_ready & vbat_ready");
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}

	printf("navboard_udp_show ok!\r\n");
	while(global_run_foverer)
	{

		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data, max_length), sender_endpoint);
		data[length]=0;
		count++;

		fusion = atomic_navboard_fusion;
		vbat = atomic_vbat_stats;
		motor_speed = atomic_motor_speed;
		calib = atomic_navboard_raw_calibration;

		length=sprintf(data,"%f|%f|%f|%f|%f|%f|%f|%f|%d|%d|%d|%d",
				fusion.fusion_pitch,fusion.fusion_roll,
				fusion.acc_pitch,fusion.acc_roll,
				fusion.gyro_x,fusion.gyro_y,fusion.gyro_z,
				vbat.vbat,
				motor_speed.front_left,motor_speed.front_right,motor_speed.rear_left,motor_speed.rear_right
		);
		//printf("navboard_udp_show=%d data=%s\r\n",count,data);

		sock.send_to(boost::asio::buffer(data, length), sender_endpoint);
	}


}

void navboard_consumer_show_in_console(void){
	navboard_fusion_t fusion;
	//
	while(!navboard_fusion_ready){
		printf("wait navboard_fusion\r\n");
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}
	while(global_run_foverer){
		fusion = atomic_navboard_fusion;
		printf("dt=%.6f seq=%.0f a(%.2f|%.2f) g(%.2f|%.2f|%.2f) gi(%.2f|%.2f|%.2f) f(%.2f|%.2f) h=%.2f\r\n",
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
				fusion.height
		);
	}
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

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

	motor_speed_t speeds_user;
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

void pilot_using_joystick_only(void){

	//
	int16_t height_speed=0;
	int16_t pitch_speed=0;
	int16_t roll_speed=0;
	int16_t yaw_speed=0;

	//
	motor_speed_t speeds_user;
	speeds_user.front_left=0;
	speeds_user.front_right=0;
	speeds_user.rear_left=0;
	speeds_user.rear_right=0;

	int takeoff=0;

	//
	joystick_cmd_t cmd;
	while(global_run_foverer){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));

		cmd = atomic_joystick_cmd;

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

void pilot_using_joystick_with_stabilizer(void){

	//
	while(!navboard_fusion_ready || !motors_ready){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
		printf("wait navboard_fusion and motors\r\n");
	}


	//
	int16_t height_speed=0;
	int16_t pitch_speed=0;
	int16_t roll_speed=0;
	int16_t yaw_speed=0;

	//
	motor_speed_t speeds_user;
	speeds_user.front_left=0;
	speeds_user.front_right=0;
	speeds_user.rear_left=0;
	speeds_user.rear_right=0;

	int takeoff=0;

	//
	joystick_cmd_t cmd;
	navboard_fusion_t fusion;

	////////////////////////////////////////////////////

	PID pid_angle_pitch;
	pid_angle_pitch.Setpoint=0;
	pid_angle_pitch.windup_min=-90;
	pid_angle_pitch.windup_max=+90;
	pid_angle_pitch.kp=0.0;
	pid_angle_pitch.ki=0.0;
	pid_angle_pitch.kd=0.0;

	PID pid_rate_pitch;
	pid_rate_pitch.Setpoint=0;
	pid_rate_pitch.windup_min=-250;
	pid_rate_pitch.windup_max=+250;
	pid_rate_pitch.kp=0.0;
	pid_rate_pitch.ki=0.0;
	pid_rate_pitch.kd=0.0;

	////////////////////////////////////////////////////

	PID pid_angle_roll;
	pid_angle_roll.Setpoint=0;
	pid_angle_roll.windup_min=-180;
	pid_angle_roll.windup_max=+180;
	pid_angle_roll.kp=0.0;
	pid_angle_roll.ki=0.0;
	pid_angle_roll.kd=0.0;

	PID pid_rate_roll;
	pid_rate_roll.Setpoint=0;
	pid_rate_roll.windup_min=-250;
	pid_rate_roll.windup_max=+250;
	pid_rate_roll.kp=0.0;
	pid_rate_roll.ki=0.0;
	pid_rate_roll.kd=0.0;

	////////////////////////////////////////////////////

	PID pid_angle_yaw;
	pid_angle_yaw.Setpoint=0;
	pid_angle_yaw.windup_min=-180;
	pid_angle_yaw.windup_max=+180;
	pid_angle_yaw.kp=0.0;
	pid_angle_yaw.ki=0.0;
	pid_angle_yaw.kd=0.0;

	PID pid_rate_yaw;
	pid_rate_yaw.Setpoint=0;
	pid_rate_yaw.windup_min=-250;
	pid_rate_yaw.windup_max=+250;
	pid_rate_yaw.kp=0.0;
	pid_rate_yaw.ki=0.0;
	pid_rate_yaw.kd=0.0;

	////////////////////////////////////////////////////


	while(global_run_foverer){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));

		cmd = atomic_joystick_cmd;
		fusion = atomic_navboard_fusion;

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


			/////////////////////////////////////////////////////////

			//PID1 - setpoint=joystick input=angle
			pid_angle_pitch.Setpoint=cmd.pitch_speed;
			pid_angle_pitch.Input = fusion.fusion_pitch;
			pid_angle_pitch.Compute();
			//PID2 - setpoint=PID1.out input=gyro
			pid_rate_pitch.Setpoint = pid_angle_pitch.Output;
			pid_rate_pitch.Input = fusion.gyro_x;
			pid_rate_pitch.Compute();

			/////////////////////////////////////////////////////////

			//PID1
			pid_angle_roll.Setpoint=cmd.roll_speed;
			pid_angle_roll.Input = fusion.fusion_roll;
			pid_angle_roll.Compute();
			//PID2
			pid_rate_roll.Setpoint = pid_angle_roll.Output;
			pid_rate_roll.Input = fusion.gyro_y;
			pid_rate_roll.Compute();

			/////////////////////////////////////////////////////////

			//PID1
			pid_angle_yaw.Setpoint=cmd.yaw_speed;
			pid_angle_yaw.Input = fusion.gyro_z_integrate;
			pid_angle_yaw.Compute();
			//PID2
			pid_rate_yaw.Setpoint = pid_angle_yaw.Output;
			pid_rate_yaw.Input = fusion.gyro_y;
			pid_rate_yaw.Compute();

			/////////////////////////////////////////////////////////

			//constraint_s16(cmd.pitch_speed,-200,200)+
			pitch_speed=pid_rate_pitch.Output;
			//constraint_s16(cmd.roll_speed,-200,200)+
			roll_speed=pid_rate_roll.Output;
			//constraint_s16(cmd.yaw_speed,-1,1);
			yaw_speed=pid_rate_yaw.Output;
			//constraint_s16(cmd.height_speed,-3,3);
			height_speed+=cmd.height_speed;

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
		//printf("stable pilot > %d %d %d %d\r\n",height_speed,pitch_speed,roll_speed,yaw_speed);



	}
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[]) {



	//
	system("killall -9 program.elf");
	system("sysctl -w kernel.panic=0");
	system("sysctl -w kernel.panic_on_oops=0");

	//
	singleton(argv[0]);

	//
	signal(SIGINT,  handler_int);
	signal(SIGTERM, handler_term);
	signal(SIGABRT, handler_abrt);


	//
	boost::thread_group ardrone_threads;

	////------------- http and cameras --------------------
	ardrone_threads.create_thread(http_thread_server);

	////-------------- vbat ----------------------
	ardrone_threads.create_thread(vbat_thread_generator);
	ardrone_threads.create_thread(vbat_thread_udp_json_server);

	////---------------- joystick -------------------------------
	ardrone_threads.create_thread(joystick_thread_udp_json_server);


	////--------------- navboard --------------------------------
	//ardrone_threads.create_thread(navboard_read_raw);
	//ardrone_threads.create_thread(navboard_consumer_raw_procude_fusion);
	//ardrone_threads.create_thread(navboard_consumer_raw_procude_fusion);
	//ardrone_threads.create_thread(navboard_consumer_show_in_console);
	//ardrone_threads.create_thread(navboard_and_motors_and_vbat_udp_server);

	////--------------- motors ----------------------------------
	//ardrone_threads.create_thread(motors_cmd_consumer);
	//ardrone_threads.create_thread(motor_test);


	////----------------- Pilots -----------------------------------
	//ardrone_threads.create_thread(pilot_using_keyboard_only);
	//ardrone_threads.create_thread(pilot_using_joystick_only);
	//ardrone_threads.create_thread(pilot_using_joystick_with_stabilizer);

	printf("---------------- BUILD %s %s ------------\r\n",__DATE__,__TIME__);

	ardrone_threads.join_all();
	printf("all threads clean exit! :)\r\n");

	return 0;
}
