/*
 * murix_pid.cpp
 *
 *  Created on: 19/02/2014
 *      Author: mpontes
 */

#include "Arduino.h"
#include "murix_pid.h"


void PID::Init(){

	//process variables
	Input=0;
	Output=0;
	Setpoint=0;

	//gains
	kp=0;
	ki=0;
	kd=0;

	//windup guards
	windup_min=0;
	windup_max=0;

	//timers
	lastTime = micros();
	now = micros();
	timeChange=(now - lastTime) / 1000000.0f;

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
	now = micros();
	timeChange = (now - lastTime) / 1000000.0;

	/*Compute all the working error variables*/
	error = Setpoint - Input;
	errSum += (error * timeChange);
	dErr = (error - lastErr) / timeChange;

	/* windup guard */
	errSum = constrain(errSum,windup_min,windup_max);

	/*Compute PID Output*/
	pterm= kp * error;
	iterm= ki * errSum;
	dterm= kd * dErr;

	Output = pterm + iterm + dterm;

	/*Remember some variables for next time*/
	lastErr = error;
	lastTime = now;
}




