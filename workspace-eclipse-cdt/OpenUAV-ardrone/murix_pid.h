/*
 * murix_pid.h
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#ifndef MURIX_PID_H_
#define MURIX_PID_H_


class PID {

private:

	//timers
	boost::posix_time::ptime now;
	boost::posix_time::ptime lastTime;
	boost::posix_time::time_duration tick_diff;
	double timeChange;

	//errors
	double lastErr, errSum, error,dErr;

public:

	//process variables
	double Input, Output, Setpoint;

	//gains
	double kp, ki, kd;

	//windup guards
	double windup_min;
	double windup_max;

	//terms
	double pterm;
	double iterm;
	double dterm;

	PID();
	void Compute();
};



#endif /* MURIX_PID_H_ */
