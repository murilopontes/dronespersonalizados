/*
 * arduino.cpp
 *
 *  Created on: 19/01/2014
 *      Author: mpontes
 */


#include "murix_arduino.h"

#include "murix_cpp_headers.h"
#include "murix_boost_headers.h"

long millis(void){
	boost::posix_time::ptime tick_back(boost::gregorian::date(1970,boost::gregorian::Jan,1));
	boost::posix_time::ptime tick_now = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration tick_diff;
	tick_diff = tick_now - tick_back;
	return tick_diff.total_milliseconds();
}

long micros(void){
	boost::posix_time::ptime tick_back(boost::gregorian::date(1970,boost::gregorian::Jan,1));
	boost::posix_time::ptime tick_now = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration tick_diff;
	tick_diff = tick_now - tick_back;
	return tick_diff.total_microseconds();
}

long nanos(void){
	boost::posix_time::ptime tick_back(boost::gregorian::date(1970,boost::gregorian::Jan,1));
	boost::posix_time::ptime tick_now = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration tick_diff;
	tick_diff = tick_now - tick_back;
	return tick_diff.total_nanoseconds();
}

