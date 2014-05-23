/*
 * murix_joystick.cpp
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#include <stdint.h>
#include <boost/atomic.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "murix_joystick.h"



joystick_cmd_t::joystick_cmd_t(){
	height_speed=0;
	pitch_speed=0;
	roll_speed=0;
	yaw_speed=0;
	emergency=0;
	takeoff=0;
}


boost::atomic<joystick_cmd_t> atomic_joystick_cmd;


void joystick_thread_udp_json_server(void)
{


	joystick_cmd_t cmd;

	////////////////////////////////////////////////////////////////
	uint32_t count=0;
	enum { max_length = 1024 };
	char data[max_length];
	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 3000));
	//////////////////////////////////////////////////////////////
	while(true)
	{


		///////////////////////////////////////////////////////////////
		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data, max_length), sender_endpoint);
		data[length]=0;
		count++;
		//printf("joystick_thread_udp_server[[%s]]\r\n",data);
		////////////////////////////////////////////////////////////////

		cmd = joystick_cmd_t();


		std::stringstream input;
		input.write(data,length);

		boost::property_tree::ptree pt;
		boost::property_tree::json_parser::read_json(input, pt);

		cmd.height_speed=pt.get("throttle",0);
		cmd.pitch_speed=pt.get("pitch",0);
		cmd.roll_speed=pt.get("roll",0);
		cmd.yaw_speed=pt.get("yaw",0);
		cmd.emergency=pt.get("emergency",0);
		cmd.takeoff=pt.get("takeoff",0);

		atomic_joystick_cmd = cmd;

		/////////////////////////////////////////////////////////////////////
		sock.send_to(boost::asio::buffer(data, length), sender_endpoint);
		/////////////////////////////////////////////////////////////////////
	}

}

