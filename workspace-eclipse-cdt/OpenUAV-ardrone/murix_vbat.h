/*
 * vbat.h
 *
 *  Created on: 19/01/2014
 *      Author: mpontes
 */

#ifndef VBAT_H_
#define VBAT_H_


class vbat_t {
public:
	float vbat;
	float vdd0;
	float vdd1;
	float vdd2;
	float vdd3;
	float vdd4;
	float vdd0_setpoint;
	float vdd1_setpoint;
	float vdd2_setpoint;
	float vdd3_setpoint;
	float vdd4_setpoint;
} ;

extern boost::atomic<vbat_t> atomic_vbat_stats;
extern boost::atomic<bool> atomic_vbat_ready;

void vbat_thread_udp_json_server(void);
void vbat_thread_generator(void);


#endif /* VBAT_H_ */
