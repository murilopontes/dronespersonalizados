/*
 * murix_navboard.h
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#ifndef MURIX_NAVBOARD_H_
#define MURIX_NAVBOARD_H_

#include "murix_boost_headers.h"
#include "murix_cpp_headers.h"

typedef struct
{
	//
	uint16_t size;
	//
	uint16_t seq;

	//Bosh BMA150
	uint16_t acc_x;
	uint16_t acc_y;
	uint16_t acc_z;

	//Invensense IDG-500 max=+-500 - 2mV g/s  / +-110 - 9.1mV g/s
	uint16_t gyro_x;
	uint16_t gyro_y;

	//Epson XV-3500CB max=100 degrees/s ---- 0.67 mV/(degree/s)
	uint16_t gyro_z;

	//Invensense IDG-500
	uint16_t gyro_110_x;
	uint16_t gyro_110_y;

	//BMA150 (120=25C, 143=48C, offset=-95)
	uint16_t acc_temp;

	//Invensense IDG-500 (1532=25C, 1572=65, offset=-1507)
	uint16_t gyro_temp;

	//Epson XV-3500CB
	uint16_t vrefEpson;

	//Invensense IDG-500
	uint16_t vrefIDG;

	//echo (bit15=signal transmitted, bit14-0=echo value)
	uint16_t us_echo;

	//
	uint16_t checksum;

	uint16_t us_echo_start;
	uint16_t us_echo_end;
	uint16_t us_association_echo;
	uint16_t us_distance_echo;
	uint16_t us_courbe_temps;
	uint16_t us_courbe_valeur;
	uint16_t us_courbe_ref;
	uint16_t us_unknown01;
	uint16_t us_unknown02;
} navboard_raw_t;


class navboard_raw_calibration_t {
public:
	double raw_acc_x_offset;
	double raw_acc_y_offset;
	double raw_acc_z_offset;
	double raw_gyro_x_offset;
	double raw_gyro_y_offset;
	double raw_gyro_z_offset;
	double raw_gyro_gain;

	navboard_raw_calibration_t();
} ;


class navboard_fusion_t {
public:
	uint8_t navboard_raw[50];
	navboard_raw_t* pkt_raw;

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

	navboard_fusion_t();
	void Compute(navboard_raw_calibration_t calibration_data,double dt);
};


extern boost::atomic<navboard_fusion_t> atomic_navboard_fusion;
extern boost::atomic<navboard_raw_calibration_t> atomic_navboard_calibration;
extern boost::atomic<bool> navboard_fusion_ready;



void navboard_generator(void);
void navboard_udp_json_server(void);

#endif /* MURIX_NAVBOARD_H_ */
