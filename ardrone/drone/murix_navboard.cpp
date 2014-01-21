/*
 * murix_navboard.cpp
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#include "murix_navboard.h"

#include "murix_boost_headers.h"
#include "murix_cpp_headers.h"

#include "murix_gpio.h"
#include "murix_constraint.h"

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

boost::atomic<navboard_fusion_t> atomic_navboard_fusion;
boost::atomic<navboard_raw_calibration_t> atomic_navboard_raw_calibration;
boost::atomic<bool> navboard_fusion_ready(false);

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

double radian2degree(double radian){
	return radian * (180.0f / M_PI);
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

navboard_raw_calibration_t::navboard_raw_calibration_t(){

	// range 0-4000 -> ~ -2g - +2g
	raw_acc_x_offset=2050; // 1000 samples average in ground
	raw_acc_y_offset=1990; // 1000 samples average in ground
	raw_acc_z_offset=2070; // 1000 samples average in wall

	// range 0-3320
	raw_gyro_x_offset=1661.0;
	raw_gyro_y_offset=1660.0;
	raw_gyro_z_offset=1663.5;

	raw_gyro_gain = (180.0f / M_PI) / 10.0f;
}

//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

navboard_fusion_t::navboard_fusion_t(){

	//
	memset(&navboard_raw,0,sizeof(navboard_raw));

	//
	pkt_raw = (navboard_raw_t*)& navboard_raw;

	//
	acc_x=0;
	acc_y=0;
	acc_z=0;

	//
	gyro_x=0;
	gyro_y=0;
	gyro_z=0;

	//
	gyro_x_dt=0;
	gyro_y_dt=0;
	gyro_z_dt=0;

	//
	gyro_x_integrate=0;
	gyro_y_integrate=0;
	gyro_z_integrate=0;

	//
	acc_pitch=0;
	acc_roll=0;

	//
	fusion_pitch=0;
	fusion_roll=0;

	//
	height=0;

	//
	dt=0;
	seq=0;
}

void navboard_fusion_t::Compute(navboard_raw_calibration_t calib,double dt){


	this->dt = dt;
	this->seq = this->pkt_raw->seq;

	//offset and scale
	this->acc_x= (this->pkt_raw->acc_x - calib.raw_acc_x_offset);
	this->acc_y= (this->pkt_raw->acc_y - calib.raw_acc_y_offset);
	this->acc_z= (this->pkt_raw->acc_z - calib.raw_acc_z_offset);

	//
	this->gyro_x = (this->pkt_raw->gyro_x - calib.raw_gyro_x_offset)/calib.raw_gyro_gain;
	this->gyro_y = (this->pkt_raw->gyro_y - calib.raw_gyro_y_offset)/calib.raw_gyro_gain;
	this->gyro_z = (this->pkt_raw->gyro_z - calib.raw_gyro_z_offset)/calib.raw_gyro_gain;

	///////////////////////////////////////////////////////////////////////////////////

	//double pitch_tmp = sqrt((this->acc_y*this->acc_y)+(this->acc_z*this->acc_z));

	//
	this->acc_pitch =  radian2degree(  atan2(this->acc_x,this->acc_z)  );

	//
	this->acc_roll =   radian2degree(  atan2(this->acc_y,this->acc_z)  );


	///////////////////////////////////////////////////////////////////////////////
	//prepare to integrate
	this->gyro_x_dt= this->gyro_x*dt;
	this->gyro_y_dt= this->gyro_y*dt;
	this->gyro_z_dt= this->gyro_z*dt;

	//integrate
	this->gyro_x_integrate+= this->gyro_x_dt;
	this->gyro_y_integrate+= this->gyro_y_dt;
	this->gyro_z_integrate+= this->gyro_z_dt;

	//clamp integrate
	this->gyro_x_integrate=constraint_double(this->gyro_x_integrate,-90,90);
	this->gyro_y_integrate=constraint_double(this->gyro_y_integrate,-180,180);
	this->gyro_z_integrate=constraint_double(this->gyro_z_integrate,-180,180);

	//////////////////////////////////////////////////////////////////////////////
	//use The complementary filter for pitch and roll

	this->fusion_pitch= 0.98*(this->fusion_pitch+this->gyro_x_dt)+(0.02*this->acc_pitch);
	this->fusion_roll = 0.98*(this->fusion_roll+this->gyro_y_dt)+(0.02*this->acc_roll);

	this->fusion_pitch=constraint_double(this->fusion_pitch,-90,90);
	this->fusion_roll=constraint_double(this->fusion_roll,-180,180);

	/////////////////////////////////////////////////////////////////////////////

	// 1cm = 37.0f

	if(this->pkt_raw->us_echo / 37.0f < (300.0f) ){
		//if distance less than 300cm
		this->height=this->pkt_raw->us_echo / 37.0f;
	} else {
		//
		this->height=-1;
	}


}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


void navboard_generator(void)
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

	//set /MCLR pin -> reset navboard
	gpio_set(132,GPIO_OUT_0);
	gpio_set(132,GPIO_OUT_1);


	//start acquisition
	uint8_t cmd=0x01;
	boost::asio::write(port, boost::asio::buffer(&cmd, 1));

	//
	if(sizeof(navboard_raw_t)!=navboard_size){
		printf("%d!=%d\r\n",sizeof(navboard_raw_t),navboard_size);
		printf("error %s:%d\r\n",__FILE__,__LINE__);
		exit(1);
	}


	//
	boost::posix_time::ptime tick_back;
	boost::posix_time::ptime tick_now;
	boost::posix_time::time_duration tick_diff;
	//
	tick_back = boost::posix_time::microsec_clock::local_time();
	tick_now = boost::posix_time::microsec_clock::local_time();
	//
	printf("navboard reader working\r\n");
	while(true){
		//read
		boost::asio::read(port, boost::asio::buffer(&c,1));
		//put in circular buffer
		cb.push_back(c);
		//buffer is full
		if(cb.size()==navboard_size){
			//header of buffer is navboard signature
			if(cb[0]==navboard_size && cb[1]==0x0 && cb[31]==0x0 && cb[30]==0x1){
				////////////////////////////////////////////////////////////////
				tick_back = tick_now;
				tick_now = boost::posix_time::microsec_clock::local_time();
				tick_diff = tick_now - tick_back;
				double dt = tick_diff.total_nanoseconds() / 1000000000.0f;
				///////////////////////////////////////////////////////////////
				navboard_fusion_t fusion = navboard_fusion_t();
				for(int i=0;i<navboard_size;i++){
					fusion.navboard_raw[i]=cb[i];
				}
				////////////////////////////////////////////////////////////////
				navboard_raw_calibration_t calib = atomic_navboard_raw_calibration;
				fusion.Compute(calib,dt);
				////////////////////////////////////////////////////////////////
				atomic_navboard_fusion=fusion;
				navboard_fusion_ready=true;
			}
		}
	}
	//
	port.close();
}



void navboard_udp_json_server(void){

	uint32_t count=0;
	enum { max_length = 1024 };
	char data[max_length];

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 5000));

	//
	while(!navboard_fusion_ready ){
		printf("navboard_udp_json_server wait navboard_fusion_ready\r\n");
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}

	printf("navboard_udp_json_server ok!\r\n");
	while(true)
	{
		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data, max_length), sender_endpoint);
		data[length]=0;
		count++;
		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////

		navboard_raw_calibration_t calib;
		calib = atomic_navboard_raw_calibration;



		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////


		navboard_fusion_t fusion;
		fusion = atomic_navboard_fusion;
		boost::property_tree::ptree pt;
		//raw data for help calibration
		pt.put("raw_ac_x",fusion.pkt_raw->acc_x);
		pt.put("raw_ac_y",fusion.pkt_raw->acc_y);
		pt.put("raw_ac_z",fusion.pkt_raw->acc_z);
		pt.put("raw_gy_x",fusion.pkt_raw->gyro_x);
		pt.put("raw_gy_y",fusion.pkt_raw->gyro_y);
		pt.put("raw_gy_z",fusion.pkt_raw->gyro_z);
		pt.put("raw_us_echo",fusion.pkt_raw->us_echo);


		std::stringstream ss;
	    boost::property_tree::json_parser::write_json(ss, pt);

		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////

		sock.send_to(boost::asio::buffer(ss.str().c_str(), ss.str().length()), sender_endpoint);


		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////

	}


}



