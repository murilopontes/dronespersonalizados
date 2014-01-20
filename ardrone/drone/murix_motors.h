/*
 * murix_motors.h
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#ifndef MURIX_MOTORS_H_
#define MURIX_MOTORS_H_

#include "murix_boost_headers.h"
#include "murix_cpp_headers.h"

class motor_speed_t {
public:
	motor_speed_t();
	void clamp();
	int16_t front_left;
	int16_t front_right;
	int16_t rear_left;
	int16_t rear_right;
} ;

extern boost::atomic<motor_speed_t> atomic_motor_speed;
extern boost::atomic<bool>          atomic_motors_ready;

void motors_thread_server(void);
void motors_thread_udp_json_status_server(void);

#endif /* MURIX_MOTORS_H_ */
