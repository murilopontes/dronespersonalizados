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

void motor_speed_t::clamp(){
	front_left  = constraint_s16(front_left,0,511);
	front_right = constraint_s16(front_right,0,511);
	rear_left   = constraint_s16(rear_left,0,511);
	rear_right  = constraint_s16(rear_right,0,511);
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
	boost::this_thread::sleep(boost::posix_time::milliseconds(250));

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



	printf("motors ready, wait for commands\r\n");

	atomic_motors_ready = true;

	while(true){
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));

		motor_speed_t speeds_pwm = atomic_motor_speed;

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
		boost::system::error_code error;
		port.read_some(boost::asio::buffer(reply,sizeof(reply)), error);

	}

}

void motors_thread_udp_json_status_server(void){


	motor_speed_t speeds;


	///////////////////////////////////////
	while(!atomic_motors_ready){
		printf("motors_thread_udp_json_status_server wait atomic_motors_ready\r\n");
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); // 1 / 30Hz = 33,33ms
	}


	////////////////////////////////////
	enum { max_length = 1024 };
	char data[max_length];
	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 6000));
	/////////////////////////////////////

	while(true){

		/////////////////////////////////////////////////////////////////////
		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data, max_length), sender_endpoint);
		data[length]=0;
		////////////////////////////////////////////////////////

		speeds = atomic_motor_speed;
		boost::property_tree::ptree pt;
		pt.put("front_left",speeds.front_left);
		pt.put("front_right",speeds.front_right);
		pt.put("rear_left",speeds.rear_left);
		pt.put("rear_right",speeds.rear_right);

		////////////////////////////////////////////////////////////
		std::stringstream ss;
		boost::property_tree::json_parser::write_json(ss, pt);
		sock.send_to(boost::asio::buffer(ss.str().c_str(), ss.str().length()), sender_endpoint);
		///////////////////////////////////////////////////////////////

	}



}





