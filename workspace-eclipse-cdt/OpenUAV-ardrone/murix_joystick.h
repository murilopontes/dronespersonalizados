/*
 * murix_joystick.h
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#ifndef MURIX_JOYSTICK_H_
#define MURIX_JOYSTICK_H_


class joystick_cmd_t {
public:

	joystick_cmd_t();

	int16_t emergency;
	int16_t takeoff;

	int16_t height_speed;
	int16_t pitch_speed;
	int16_t roll_speed;
	int16_t yaw_speed;

};

extern boost::atomic<joystick_cmd_t> atomic_joystick_cmd;

void joystick_thread_udp_json_server(void);

#endif /* MURIX_JOYSTICK_H_ */
