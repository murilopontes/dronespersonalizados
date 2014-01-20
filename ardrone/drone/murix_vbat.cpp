/*
 * vbat.cpp
 *
 *  Created on: 19/01/2014
 *      Author: mpontes
 */

#include "murix_vbat.h"

#include "murix_cpp_headers.h"
#include "murix_boost_headers.h"

// i2c library
#include "i2c-dev.h"




boost::atomic<vbat_t> atomic_vbat_stats;
boost::atomic<bool> atomic_vbat_ready(false);

float vbat_get_channel(unsigned char channel,int fd)
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

int vbat_get_all_channels(vbat_t *vbat,int fd)
{
	vbat->vbat=vbat_get_channel(0,fd);
	vbat->vdd0=vbat_get_channel(4,fd);
	vbat->vdd1=vbat_get_channel(5,fd);
	vbat->vdd2=vbat_get_channel(6,fd);
	vbat->vdd3=vbat_get_channel(7,fd);
	vbat->vdd4=vbat_get_channel(8,fd);
	return 0;
}

void vbat_thread_generator(void){

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
	while(true) {
		vbat_get_all_channels(&vbat,fd);
		atomic_vbat_stats = vbat;
		atomic_vbat_ready = true;
	}


}

void vbat_thread_udp_json_server(void){

	vbat_t vbat;


	///---------------------------
	while(!atomic_vbat_ready){
		printf("vbat_thread_udp_json_server wait atomic_vbat_ready\r\n");
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}



	///-------------------------------
	enum { max_length = 1024 };
	char data[max_length];
	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 4000));
	while(true){
		boost::asio::ip::udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data, max_length), sender_endpoint);
		data[length]=0;

		vbat = atomic_vbat_stats;



		boost::property_tree::ptree pt;
		pt.put("vbat",vbat.vbat);
		pt.put("vdd0",vbat.vdd0);
		pt.put("vdd1",vbat.vdd1);
		pt.put("vdd2",vbat.vdd2);
		pt.put("vdd3",vbat.vdd3);
		pt.put("vdd4",vbat.vdd4);
		pt.put("vdd0_setpoint",vbat.vdd0_setpoint);
		pt.put("vdd1_setpoint",vbat.vdd1_setpoint);
		pt.put("vdd2_setpoint",vbat.vdd2_setpoint);
		pt.put("vdd3_setpoint",vbat.vdd3_setpoint);
		pt.put("vdd4_setpoint",vbat.vdd4_setpoint);

		std::stringstream ss;
	    boost::property_tree::json_parser::write_json(ss, pt);

		sock.send_to(boost::asio::buffer(ss.str().c_str(), ss.str().length()), sender_endpoint);

	}

}


