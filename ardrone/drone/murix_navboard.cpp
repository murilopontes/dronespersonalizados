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


double radian2degree(double radian){
	return radian * (180.0f / M_PI);
}






navboard_raw_calibration_t::navboard_raw_calibration_t(){

	acc_x_offset=2060; // 1000 samples average in ground
	acc_y_offset=1963; // 1000 samples average in ground
	acc_z_offset=2041; // 1000 samples average in wall

	gyro_x_offset=1659; // 1000 samples average in ground
	gyro_y_offset=1664; // 1000 samples average in ground
	gyro_z_offset=1662; // 1000 samples average in ground

	gyro_gain = (180.0f / M_PI) / 10.0f;
}



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
	while(true){
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
	while(true){
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
	navboard_raw_calibration_t calib;


	uint32_t count=0;
	enum { max_length = 1024 };
	char data[max_length];

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 4000));


	//
	while(!navboard_fusion_ready ){
		printf("wait navboard_fusion_ready & vbat_ready");
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	}

	printf("navboard_udp_show ok!\r\n");
	while(true)
	{

		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data, max_length), sender_endpoint);
		data[length]=0;
		count++;

		fusion = atomic_navboard_fusion;
		calib = atomic_navboard_raw_calibration;



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
	while(true){
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

