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

/*
{
    "raw_acc_x_offset": "2048.5",
    "raw_acc_y_offset": "1990.5",
    "raw_acc_z_offset": "2070",
    "raw_acc_gain": "100",
    "raw_gyro_x_offset": "1789",
    "raw_gyro_y_offset": "1665",
    "raw_gyro_z_offset": "1663",
    "raw_gyro_gain": "5.729577951308232",
    "acc_pitch_offset": "0.65",
    "acc_roll_offset": "0.75"
}

*/
	// range 0-4000 -> ~ -2g - +2g
	raw_acc_x_offset=2048.5; // 1000 samples average in ground
	raw_acc_y_offset=1990.5; // 1000 samples average in ground
	raw_acc_z_offset=2070.0; // 1000 samples average in wall
	raw_acc_gain=100.0;

	// range 0-3320
	raw_gyro_x_offset=1789;
	raw_gyro_y_offset=1665;
	raw_gyro_z_offset=1663;

	raw_gyro_gain_x = ((180.0f / M_PI) / 4.5f);
	raw_gyro_gain_y = ((180.0f / M_PI) / 4.5f);
	raw_gyro_gain_z = (180.0f / M_PI) / 10.0f;


	acc_pitch_offset= +0.65;
	acc_roll_offset= +0.75;
}


void navboard_get_calib_udp_json_server(void){

	uint32_t count=0;
	enum { max_length = 1024 };
	char data[max_length];

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 10000));

	printf("navboard_get_calib_udp_json_server ok!\r\n");
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

		boost::property_tree::ptree pt;
		//raw data for help calibration
		pt.put("raw_acc_x_offset",calib.raw_acc_x_offset);
		pt.put("raw_acc_y_offset",calib.raw_acc_y_offset);
		pt.put("raw_acc_z_offset",calib.raw_acc_z_offset);
		pt.put("raw_acc_gain",calib.raw_acc_gain);
		pt.put("raw_gyro_x_offset",calib.raw_gyro_x_offset);
		pt.put("raw_gyro_y_offset",calib.raw_gyro_y_offset);
		pt.put("raw_gyro_z_offset",calib.raw_gyro_z_offset);
		pt.put("raw_gyro_gain_x",calib.raw_gyro_gain_x);
		pt.put("raw_gyro_gain_y",calib.raw_gyro_gain_y);
		pt.put("raw_gyro_gain_z",calib.raw_gyro_gain_z);

		pt.put("acc_pitch_offset",calib.acc_pitch_offset);
		pt.put("acc_roll_offset",calib.acc_roll_offset);


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


void navboard_set_calib_udp_json_server(void){

	uint32_t count=0;
	enum { max_length = 1024 };
	char data[max_length];

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 11000));

	printf("navboard_set_calib_udp_json_server ok!\r\n");
	while(true)
	{
		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data, max_length), sender_endpoint);
		data[length]=0;
		count++;

		navboard_raw_calibration_t calib;

		std::stringstream input;
		input.write(data,length);

		boost::property_tree::ptree ptr;
		boost::property_tree::json_parser::read_json(input, ptr);

		calib.raw_acc_x_offset= ptr.get<double>("raw_acc_x_offset",0);
		calib.raw_acc_y_offset= ptr.get<double>("raw_acc_y_offset",0);
		calib.raw_acc_z_offset= ptr.get<double>("raw_acc_z_offset",0);
		calib.raw_acc_gain=     ptr.get<double>("raw_acc_gain",1);
		calib.raw_gyro_x_offset=ptr.get<double>("raw_gyro_x_offset",0);
		calib.raw_gyro_y_offset=ptr.get<double>("raw_gyro_y_offset",0);
		calib.raw_gyro_z_offset=ptr.get<double>("raw_gyro_z_offset",0);
		calib.raw_gyro_gain_x=    ptr.get<double>("raw_gyro_gain_x",1);
		calib.raw_gyro_gain_y=    ptr.get<double>("raw_gyro_gain_y",1);
		calib.raw_gyro_gain_z=    ptr.get<double>("raw_gyro_gain_z",1);
		calib.acc_pitch_offset= ptr.get<double>("acc_pitch_offset",1);
		calib.acc_roll_offset=  ptr.get<double>("acc_roll_offset",1);


		printf("navboard_set_calib_udp_json_server RX json ok! count=%d\r\n",count);

		atomic_navboard_raw_calibration = calib;
		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////


		boost::property_tree::ptree pt;
		//raw data for help calibration
		pt.put("raw_acc_x_offset",calib.raw_acc_x_offset);
		pt.put("raw_acc_y_offset",calib.raw_acc_y_offset);
		pt.put("raw_acc_z_offset",calib.raw_acc_z_offset);
		pt.put("raw_acc_gain",calib.raw_acc_gain);
		pt.put("raw_gyro_x_offset",calib.raw_gyro_x_offset);
		pt.put("raw_gyro_y_offset",calib.raw_gyro_y_offset);
		pt.put("raw_gyro_z_offset",calib.raw_gyro_z_offset);
		pt.put("raw_gyro_gain_x",calib.raw_gyro_gain_x);
		pt.put("raw_gyro_gain_y",calib.raw_gyro_gain_y);
		pt.put("raw_gyro_gain_z",calib.raw_gyro_gain_z);
		pt.put("acc_pitch_offset",calib.acc_pitch_offset);
		pt.put("acc_roll_offset",calib.acc_roll_offset);

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
	this->acc_x= (this->pkt_raw->acc_x - calib.raw_acc_x_offset)/calib.raw_acc_gain;
	this->acc_y= (this->pkt_raw->acc_y - calib.raw_acc_y_offset)/calib.raw_acc_gain;
	this->acc_z= (this->pkt_raw->acc_z - calib.raw_acc_z_offset)/calib.raw_acc_gain;

	//
	this->gyro_x = (this->pkt_raw->gyro_110_x - calib.raw_gyro_x_offset)/calib.raw_gyro_gain_x;
	this->gyro_y = (this->pkt_raw->gyro_110_y - calib.raw_gyro_y_offset)/calib.raw_gyro_gain_y;
	this->gyro_z = (this->pkt_raw->gyro_z - calib.raw_gyro_z_offset)/calib.raw_gyro_gain_z;

	///////////////////////////////////////////////////////////////////////////////////

	//double pitch_tmp = sqrt((this->acc_y*this->acc_y)+(this->acc_z*this->acc_z));

	//
	this->acc_pitch =  radian2degree(  atan2( -this->acc_x,this->acc_z)  ) + calib.acc_pitch_offset;

	//
	this->acc_roll =   radian2degree(  atan2(this->acc_y,this->acc_z)  ) + calib.acc_roll_offset;;


	///////////////////////////////////////////////////////////////////////////////
	//prepare to integrate
	this->gyro_x_dt= this->gyro_x*dt;
	this->gyro_y_dt= this->gyro_y*dt;
	this->gyro_z_dt= this->gyro_z*dt;

	//integrate
	this->gyro_x_integrate+= this->gyro_x_dt;
	this->gyro_y_integrate+= this->gyro_y_dt;
	this->gyro_z_integrate -= this->gyro_z_dt;


	//clamp integrate
	this->gyro_x_integrate=constraint_double(this->gyro_x_integrate,-180,180);
	this->gyro_y_integrate=constraint_double(this->gyro_y_integrate,-180,180);
	this->gyro_z_integrate=constraint_double(this->gyro_z_integrate,-180,180);

	//////////////////////////////////////////////////////////////////////////////
	//use The complementary filter for pitch and roll

	this->fusion_pitch= 0.98*(this->fusion_pitch+this->gyro_x_dt)+(0.02*this->acc_pitch);
	this->fusion_roll = 0.98*(this->fusion_roll+this->gyro_y_dt)+(0.02*this->acc_roll);

	this->fusion_pitch=constraint_double(this->fusion_pitch,-180,180);
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

	navboard_fusion_t fusion = navboard_fusion_t();

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
				for(int i=0;i<navboard_size;i++){
					fusion.navboard_raw[i]=cb[i];
				}
				////////////////////////////////////////////////////////////////
				navboard_raw_calibration_t calib = atomic_navboard_raw_calibration;
				fusion.Compute(calib,dt);
				////////////////////////////////////////////////////////////////
				//snapshot fusion to atomic
				atomic_navboard_fusion=fusion;
				navboard_fusion_ready=true;
				////////////////////////////////////////////////////////////////
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


		navboard_fusion_t fusion;
		fusion = atomic_navboard_fusion;
		boost::property_tree::ptree pt;
		//raw data for help calibration
		pt.put("raw_ac_x",fusion.pkt_raw->acc_x);
		pt.put("raw_ac_y",fusion.pkt_raw->acc_y);
		pt.put("raw_ac_z",fusion.pkt_raw->acc_z);
		pt.put("raw_gy_x",fusion.pkt_raw->gyro_110_x);
		pt.put("raw_gy_y",fusion.pkt_raw->gyro_110_y);
		pt.put("raw_gy_z",fusion.pkt_raw->gyro_z);
		pt.put("raw_us_echo",fusion.pkt_raw->us_echo);

		pt.put("acc_x",fusion.acc_x);
		pt.put("acc_y",fusion.acc_y);
		pt.put("acc_z",fusion.acc_z);

		pt.put("gyro_x",fusion.gyro_x);
		pt.put("gyro_y",fusion.gyro_y);
		pt.put("gyro_z",fusion.gyro_z);

		pt.put("gyro_x_integrate",fusion.gyro_x_integrate);
		pt.put("gyro_y_integrate",fusion.gyro_y_integrate);
		pt.put("gyro_z_integrate",fusion.gyro_z_integrate);



		pt.put("acc_pitch",fusion.acc_pitch);
		pt.put("acc_roll",fusion.acc_roll);

		pt.put("fusion_pitch",fusion.fusion_pitch);
		pt.put("fusion_roll",fusion.fusion_roll);

		pt.put("height",fusion.height);
		pt.put("dt",fusion.dt);

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



