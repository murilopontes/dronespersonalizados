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

	navboard_raw_calibration_t();
} ;


extern boost::atomic<navboard_raw_calibration_t> atomic_navboard_raw_calibration;
extern boost::atomic<navboard_raw_t> atomic_navboard_raw;
extern boost::atomic<navboard_fusion_t> atomic_navboard_fusion;
extern boost::atomic<bool> navboard_raw_ready;
extern boost::atomic<bool> navboard_fusion_ready;


#endif /* MURIX_NAVBOARD_H_ */
