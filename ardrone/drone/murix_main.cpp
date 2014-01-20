/*
 * murix_main.cpp
 *
 *  Created on: 02/12/2013
 *      Author: mpontes
 */

#include "murix_cpp_headers.h"
#include "murix_boost_headers.h"
#include "murix_singleton.h"
#include "murix_daemonize.h"
#include "murix_camera.h"
#include "murix_arduino.h"
#include "murix_vbat.h"
#include "murix_http_server.h"
#include "murix_pid.h"
#include "murix_constraint.h"
#include "murix_gpio.h"
#include "murix_joystick.h"
#include "murix_motors.h"
#include "murix_navboard.h"
#include "murix_pilot.h"


void handler_abrt(int) {
	printf("handler_abrt\r\n");
}

void handler_int(int) {
	printf("handler_int\r\n");
}

void handler_term(int) {
	printf("handler_term\r\n");
}

int main(int argc, char *argv[]) {
	//
	system("killall -9 program.elf");
	system("sysctl -w kernel.panic=0");
	system("sysctl -w kernel.panic_on_oops=0");
	//
	singleton(argv[0]);
	//
	signal(SIGINT,  handler_int);
	signal(SIGTERM, handler_term);
	signal(SIGABRT, handler_abrt);
	//
	boost::thread_group ardrone_threads;

	////------------- http and cameras --------------------
	ardrone_threads.create_thread(http_thread_server);

	////-------------- vbat ----------------------
	ardrone_threads.create_thread(vbat_thread_generator);
	ardrone_threads.create_thread(vbat_thread_udp_json_server);

	////---------------- joystick -------------------------------
	ardrone_threads.create_thread(joystick_thread_udp_json_server);

	////--------------- motors ----------------------------------
	ardrone_threads.create_thread(motors_thread_server);
	ardrone_threads.create_thread(motors_thread_udp_json_status_server);

	////--------------- navboard --------------------------------
	//ardrone_threads.create_thread(navboard_read_raw);
	//ardrone_threads.create_thread(navboard_consumer_raw_procude_fusion);
	//ardrone_threads.create_thread(navboard_consumer_raw_procude_fusion);
	//ardrone_threads.create_thread(navboard_consumer_show_in_console);
	//ardrone_threads.create_thread(navboard_and_motors_and_vbat_udp_server);

	////----------------- Pilots -----------------------------------
	//ardrone_threads.create_thread(pilot_using_keyboard_only);
	//ardrone_threads.create_thread(pilot_using_joystick_only);
	//ardrone_threads.create_thread(pilot_using_joystick_with_stabilizer);

	printf("---------------- BUILD %s %s ------------\r\n",__DATE__,__TIME__);

	ardrone_threads.join_all();
	printf("all threads clean exit! :)\r\n");

	return 0;
}
