/*
 * murix_pid.cpp
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#include "boost/date_time/posix_time/posix_time.hpp"

#include "murix_pid.h"


#include "murix_constraint.h"

PID::PID(){

	//process variables
	Input=0;
	Output=0;
	Setpoint=0;

	//gains
	kp=1;
	ki=1;
	kd=1;

	//windup guards
	windup_min=0;
	windup_max=0;

	//timers
	lastTime = boost::posix_time::microsec_clock::local_time();
	now = boost::posix_time::microsec_clock::local_time();
	tick_diff = now - lastTime;
	timeChange=tick_diff.total_microseconds() / 1000000.0f;

	//errors
	lastErr=0;
	errSum=0;
	error=0;
	dErr=0;

	//terms
	pterm=0;
	iterm=0;
	dterm=0;

}

void PID::Compute()
{
	/*How long since we last calculated*/
	now = boost::posix_time::microsec_clock::local_time();
	tick_diff = now - lastTime;
	timeChange = tick_diff.total_microseconds() / 1000000.0f;

	/*Compute all the working error variables*/
	error = Setpoint - Input;
	errSum += (error * timeChange);
	dErr = (error - lastErr) / timeChange;

	/* windup guard */
	errSum = constraint_double(errSum,windup_min,windup_max);

	/*Compute PID Output*/
	pterm= kp * error;
	iterm= ki * errSum;
	dterm= kd * dErr;

	Output = pterm + iterm + dterm;

	/*Remember some variables for next time*/
	lastErr = error;
	lastTime = now;
}





