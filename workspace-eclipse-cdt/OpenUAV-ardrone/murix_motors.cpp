/*
 * murix_motors.cpp
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#include <stdint.h>
#include <boost/atomic.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread/thread.hpp>


#include "murix_motors.h"
#include "murix_gpio.h"
#include "murix_constraint.h"



motor_speed_t::motor_speed_t(){
	front_left=0;
	front_right=0;
	rear_left=0;
	rear_right=0;
}

void motor_speed_t::clamp(int pwm_max){
	front_left  = constraint_s16(front_left,0,pwm_max);
	front_right = constraint_s16(front_right,0,pwm_max);
	rear_left   = constraint_s16(rear_left,0,pwm_max);
	rear_right  = constraint_s16(rear_right,0,pwm_max);
}
void motor_speed_t::print(){
	printf("motors pwm -> %d %d %d %d\r\n",front_left,front_right,rear_left,rear_right);
}

boost::atomic<motor_speed_t>     atomic_motor_speed;
boost::atomic<bool> atomic_motors_ready(false);


void motors_cmd_io(boost::asio::serial_port* port,uint8_t cmd, uint8_t*reply, int replylen){

	memset(reply,0xff,replylen);

	boost::asio::write(*port, boost::asio::buffer(&cmd, 1));


	boost::system::error_code error;
	int ret = (*port).read_some(boost::asio::buffer(reply,sizeof(replylen)), error);



	bool print = false;
	if(print){
		printf("motors_cmd_io write2 -> %02x | ",cmd);
		printf("reply2 -> ");
		for(int i=0;i<ret;i++){
			printf("%02x",reply[i]);
		}
		printf("\r\n");
	}

}

void motors_set_pwm(boost::asio::serial_port* port, motor_speed_t speeds_pwm){

	//
	speeds_pwm.clamp(511);

	//
	uint16_t pwm[4];
	pwm[0]=speeds_pwm.front_left;
	pwm[1]=speeds_pwm.front_right;
	pwm[2]=speeds_pwm.rear_right;
	pwm[3]=speeds_pwm.rear_left;

	//
	const uint16_t pwm_min=0x00;
	const uint16_t pwm_max=0x1ff;

	//cmd
	uint8_t cmd[5];
	cmd[0] = 0x20 | ((pwm[0]&pwm_max)>>4);
	cmd[1] = ((pwm[0]&pwm_max)<<4) | ((pwm[1]&pwm_max)>>5);
	cmd[2] = ((pwm[1]&pwm_max)<<3) | ((pwm[2]&pwm_max)>>6);
	cmd[3] = ((pwm[2]&pwm_max)<<2) | ((pwm[3]&pwm_max)>>7);
	cmd[4] = ((pwm[3]&pwm_max)<<1);

	//


	//
	boost::asio::write(*port, boost::asio::buffer(&cmd, 5));
	//
	uint8_t reply[256];
	boost::system::error_code error;
	int ret = (*port).read_some(boost::asio::buffer(reply,sizeof(reply)), error);

	if(memcmp(&cmd,&reply,5)!=0){
		printf("motor write -> ");
		for(int i=0;i<5;i++){
			printf("%02x",cmd[i]);
		}
		printf("\r\n");
		printf("motor reply -> ");
		for(int i=0;i<ret;i++){
			printf("%02x",reply[i]);
		}
		printf("\r\n");
	}

}

void motors_thread_server(void){

	// GPIO (gpio num -d i [select])(gpio num -d ho 0 [deselect])
	// 68 - motor1
	// 69 - motor2
	// 70 - motor3
	// 71 - motor4
	// 106 - irq (input)
	// 107 - reset irq (-d ho 1 [reset])


	//
	boost::asio::io_service io;
	boost::asio::serial_port port(io);
	port.open("/dev/ttyPA1");
	port.set_option(boost::asio::serial_port_base::baud_rate(115200));

	//
	motor_cmd_init:

	printf("motors reset irq\r\n");
	gpio_set(106,GPIO_IN); //configure irq input
	gpio_set(107,GPIO_OUT_0); //reset irq -
	gpio_set(107,GPIO_OUT_1); //reset irq - change 0 to 1 for reset

	printf("motors all as inactive\r\n");
	gpio_set(68,GPIO_OUT_1);
	gpio_set(69,GPIO_OUT_1);
	gpio_set(70,GPIO_OUT_1);
	gpio_set(71,GPIO_OUT_1);

	uint8_t reply[256];
	for(int m=0;m<4;m++) {

		printf("--------------------------------\r\n");

		printf("motor[%d] active\r\n",m);
		gpio_set(68+m,GPIO_IN);

		printf("motor[%d] get status\r\n",m);
		motors_cmd_io(&port,0xe0,reply,sizeof(reply));

		printf("motor[%d] check if motor is ok\r\n",m);
		if(reply[0]!=0xe0 || reply[1]!=0x00)
		{
			printf("motors init error motor=%d cmd=0x%02x reply=0x%02x\n",m+1,(int)reply[0],(int)reply[1]);
			boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

			goto motor_cmd_init;
		}

		printf("motor[%d] command assign\r\n",m);
		motors_cmd_io(&port,m+1,reply,sizeof(reply));

		//printf("motor[%d] command version\r\n",m);
		//motors_cmd_io(&port,0x40,reply,sizeof(reply));

		//
		printf("motor[%d] inactive\r\n",m);
		gpio_set(68+m,GPIO_OUT_1);
	}

	printf("--------------------------------\r\n");
	printf("motors all as inactive\r\n");
	gpio_set(68,GPIO_IN);
	gpio_set(69,GPIO_IN);
	gpio_set(70,GPIO_IN);
	gpio_set(71,GPIO_IN);

	printf("motors multicast\r\n");
	motors_cmd_io(&port,0xa0,reply,sizeof(reply));
	motors_cmd_io(&port,0xa0,reply,sizeof(reply));
	motors_cmd_io(&port,0xa0,reply,sizeof(reply));
	motors_cmd_io(&port,0xa0,reply,sizeof(reply));
	motors_cmd_io(&port,0xa0,reply,sizeof(reply));

	printf("motors reset irq\r\n");
	gpio_set(106,GPIO_IN);
	gpio_set(107,GPIO_OUT_0);
	gpio_set(107,GPIO_OUT_1);

	printf("motors ready, wait for commands\r\n");
	atomic_motors_ready = true;

	while(true){
		//
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		//
		motor_speed_t speeds_pwm = atomic_motor_speed;
		//
		motors_set_pwm(&port,speeds_pwm);


		//reply[1]-overload
		//

		//reply[2]-status
		//0x01-idle
		//0x41-spin up
		//0x81-running
		//0xc1-crash

		//
		motors_cmd_io(&port,0x89,reply,sizeof(reply));
		if(reply[1]!=0xff){ printf("motor overload->"); speeds_pwm.print(); }
		switch(reply[2]){
		case 0x01:
		case 0x41:
		case 0x81:
			break;
		case 0xc1:
			printf("motor front-left crash ->"); speeds_pwm.print();
			printf("send recovery\r\n");
			speeds_pwm.front_left=0;
			motors_set_pwm(&port,speeds_pwm);
			break;
		default:
			printf("motor front-left bug ->"); speeds_pwm.print();
			break;
		}

		motors_cmd_io(&port,0x8a,reply,sizeof(reply));
		if(reply[1]!=0xff){ printf("motor overload->"); speeds_pwm.print(); }
		switch(reply[2]){
		case 0x01:
		case 0x41:
		case 0x81:
			break;
		case 0xc1:
			printf("motor front-right crash ->"); speeds_pwm.print();
			printf("send recovery\r\n");
			speeds_pwm.front_right=0;
			motors_set_pwm(&port,speeds_pwm);
			break;
		default:
			printf("motor front-right bug ->"); speeds_pwm.print();
			break;
		}

		motors_cmd_io(&port,0x8b,reply,sizeof(reply));
		if(reply[1]!=0xff){ printf("motor overload->"); speeds_pwm.print(); }
		switch(reply[2]){
		case 0x01:
		case 0x41:
		case 0x81:
			break;
		case 0xc1:
			printf("motor1 rear-right crash ->"); speeds_pwm.print();
			printf("send recovery\r\n");
			speeds_pwm.rear_right=0;
			motors_set_pwm(&port,speeds_pwm);
			break;
		default:
			printf("motor1 rear-right bug ->"); speeds_pwm.print();
			break;
		}

		motors_cmd_io(&port,0x8c,reply,sizeof(reply));
		if(reply[1]!=0xff){ printf("motor overload->"); speeds_pwm.print(); }
		switch(reply[2]){
		case 0x01:
		case 0x41:
		case 0x81:
			break;
		case 0xc1:
			printf("motor1 rear-left crash ->"); speeds_pwm.print();
			printf("send recovery\r\n");
			speeds_pwm.rear_left=0;
			motors_set_pwm(&port,speeds_pwm);
			break;
		default:
			printf("motor1 rear-left bug ->"); speeds_pwm.print();
			break;
		}


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





