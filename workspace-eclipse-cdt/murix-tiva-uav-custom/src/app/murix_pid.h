/*
 * murix_pid.h
 *
 *  Created on: 19/02/2014
 *      Author: mpontes
 */

#ifndef MURIX_PID_H_
#define MURIX_PID_H_




class PID {

private:

  //timers
  double now;
  double lastTime;
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


  void Init();
  void Compute();
};

#endif /* MURIX_PID_H_ */
