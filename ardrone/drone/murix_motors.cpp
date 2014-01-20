/*
 * murix_motors.cpp
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#include "murix_motors.h"

#include "murix_boost_headers.h"
#include "murix_cpp_headers.h"

#include "murix_gpio.h"
#include "murix_constraint.h"



motor_speed_t::motor_speed_t(){
		front_left=0;
		front_right=0;
		rear_left=0;
		rear_right=0;
	}


boost::atomic<motor_speed_t>     atomic_motor_speed;
boost::atomic<bool> atomic_motors_ready(false);

void motors_cmd_io(boost::asio::serial_port* port,uint8_t cmd, uint8_t*reply, int replylen){
	boost::asio::write(*port, boost::asio::buffer(&cmd, 1));
	boost::asio::read(*port, boost::asio::buffer(reply,replylen));
}

void motors_thread_server(void){



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

	while(true){

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

		atomic_motors_ready=true;
		/*
		printf("ret=%d [",ret);
		for(int i=0;i<ret;i++){
			printf("%02x",reply[i]);
		}
		printf("]\r\n");
		 */
	}

}



