/*
 * murix_pilot.cpp
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#include "murix_pilot.h"

#include "murix_boost_headers.h"
#include "murix_cpp_headers.h"

#include "murix_pid.h"
#include "murix_motors.h"
#include "murix_constraint.h"
#include "murix_joystick.h"
#include "murix_navboard.h"

//POSIX
#include <termios.h>
#include <unistd.h>



void pilot_using_keyboard_only(void){

	printf("build %s %s\r\n",__DATE__,__TIME__);
	printf("|-----------------------|\r\n");
	printf("|  Q = quit             |\r\n");
	printf("|  <space> = min speed  |\r\n");
	printf("|                       |\r\n");
	printf("|     ^          ^      |\r\n");
	printf("|     E          I      |\r\n");
	printf("| < S   D >  < J   K >  |\r\n");
	printf("|     X          M      |\r\n");
	printf("|    \\_/        \\_/     |\r\n");
	printf("|-----------------------|\r\n");

	int16_t height_speed=0;
	int16_t pitch_speed=0;
	int16_t roll_speed=0;
	int16_t yaw_speed=0;

	motor_speed_t speeds_user;
	speeds_user.front_left=0;
	speeds_user.front_right=0;
	speeds_user.rear_left=0;
	speeds_user.rear_right=0;


	// POSIX functions to turn off canonical mode on the input stream
	termios tos;
	tcgetattr(0, &tos);
	tos.c_lflag &= ~ICANON; // or cfmakeraw(&tos) but it's not POSIX
	tcsetattr(0, TCSANOW, &tos);

	char c;
	while(std::cin.get(c))
	{
		if(c=='e') pitch_speed+=1;
		if(c=='x') pitch_speed-=1;

		if(c=='s') roll_speed-=1;
		if(c=='d') roll_speed+=1;

		if(c=='j') yaw_speed-=1;
		if(c=='k') yaw_speed+=1;

		if(c=='i') height_speed+=1;
		if(c=='m') height_speed-=1;

		if(c==' '){
			//
			height_speed=0;
			pitch_speed=0;
			roll_speed=0;
			yaw_speed=0;
		} else {
			//clamp
			height_speed=constraint_s16(height_speed,     100  ,  511  );
			pitch_speed =constraint_s16(pitch_speed ,    -511  ,  511  );
			roll_speed  =constraint_s16(roll_speed  ,    -511  ,  511  );
			yaw_speed   =constraint_s16(yaw_speed   ,    -511  ,  511  );
		}

		//mix table
		speeds_user.front_left  = height_speed +constraint_s16((-pitch_speed + roll_speed - yaw_speed),0,511);
		speeds_user.front_right = height_speed +constraint_s16((-pitch_speed - roll_speed + yaw_speed),0,511);
		speeds_user.rear_left   = height_speed +constraint_s16((+pitch_speed + roll_speed + yaw_speed),0,511);
		speeds_user.rear_right  = height_speed +constraint_s16((+pitch_speed - roll_speed - yaw_speed),0,511);
		//send to motors
		atomic_motor_speed=speeds_user;

		printf(" > %d %d %d %d\r\n",height_speed,pitch_speed,roll_speed,yaw_speed);

	}




}




void pilot_using_joystick_only(void){

	//
	int16_t height_speed=0;
	int16_t pitch_speed=0;
	int16_t roll_speed=0;
	int16_t yaw_speed=0;

	//
	motor_speed_t speeds_user;
	speeds_user.front_left=0;
	speeds_user.front_right=0;
	speeds_user.rear_left=0;
	speeds_user.rear_right=0;

	int takeoff=0;

	//
	joystick_cmd_t cmd;
	while(true){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));

		cmd = atomic_joystick_cmd;

		if(cmd.takeoff){
			takeoff=1;
			printf("takeoff\r\n");
		}

		if(cmd.emergency){
			printf("emergency\r\n");
			//
			height_speed=0;
			pitch_speed=0;
			roll_speed=0;
			yaw_speed=0;
			takeoff=0;
		}


		if(takeoff){
			//clam from joystick
			pitch_speed=constraint_s16(cmd.pitch_speed,-511,511);
			roll_speed=constraint_s16(cmd.roll_speed,-511,511);
			yaw_speed=constraint_s16(cmd.yaw_speed,-511,511);
			height_speed=constraint_s16(cmd.height_speed,-511,511);

			//clamp for safety
			height_speed=constraint_s16(height_speed,     100  ,  511  );
			pitch_speed =constraint_s16(pitch_speed ,    -511  ,  511  );
			roll_speed  =constraint_s16(roll_speed  ,    -511  ,  511  );
			yaw_speed   =constraint_s16(yaw_speed   ,    -511  ,  511  );
		}



		//mix table
		speeds_user.front_left  = height_speed +constraint_s16((-pitch_speed + roll_speed - yaw_speed),0,511);
		speeds_user.front_right = height_speed +constraint_s16((-pitch_speed - roll_speed + yaw_speed),0,511);
		speeds_user.rear_left   = height_speed +constraint_s16((+pitch_speed + roll_speed + yaw_speed),0,511);
		speeds_user.rear_right  = height_speed +constraint_s16((+pitch_speed - roll_speed - yaw_speed),0,511);



		//clamp
		speeds_user.clamp(511);

		//send to motors
		atomic_motor_speed=speeds_user;

		//printf("udp pilot > %d %d %d %d\r\n",height_speed,pitch_speed,roll_speed,yaw_speed);

	}
}


double* neural(double roll_delta, double roll_error,double motors[4]){


	return motors;
}

void pilot_using_neural_network(void){

	double time_now = 0;

	double speed_of_motor1 = 1;
	double speed_of_motor2 = 1;
	double speed_of_motor3 = 1;
	double speed_of_motor4 = 1;

	double roll_sensor=0;
	double roll_sensor_back=0;
	double roll_joystick=0;
	double roll_delta = roll_sensor - roll_joystick;
	double roll_error = 0;

	double pitch_delta = roll_sensor - roll_joystick;
	double pitch_error = 0;

	double yaw_delta = roll_sensor - roll_joystick;
	double yaw_error = 0;

	while(1){
		roll_error = roll_joystick - roll_sensor_back;
		roll_delta = roll_sensor - roll_joystick;

		pitch_error = roll_joystick - roll_sensor_back;
		pitch_delta = roll_sensor - roll_joystick;

		yaw_error = roll_joystick - roll_sensor_back;
		yaw_delta = roll_sensor - roll_joystick;

		//run neural

		//set motors with neural output
	}



}

void pilot_using_joystick_with_stabilizer(void){

	//
	while(!navboard_fusion_ready || !atomic_motors_ready){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
		printf("wait navboard_fusion and motors\r\n");
	}


	//
	int16_t height_speed=0;
	int16_t pitch_speed=0;
	int16_t roll_speed=0;
	int16_t yaw_speed=0;

	//
	motor_speed_t speeds_user;
	speeds_user.front_left=0;
	speeds_user.front_right=0;
	speeds_user.rear_left=0;
	speeds_user.rear_right=0;

	int takeoff=0;

	//
	joystick_cmd_t cmd;
	navboard_fusion_t fusion;


	////////////////////////////////////////////////////

	PID pid_angle_pitch;
	pid_angle_pitch.Setpoint=0;
	pid_angle_pitch.windup_min=-180;
	pid_angle_pitch.windup_max=+180;
	pid_angle_pitch.kp=0.0;
	pid_angle_pitch.ki=0.0;
	pid_angle_pitch.kd=0.0;

	PID pid_rate_pitch;
	pid_rate_pitch.Setpoint=0;
	pid_rate_pitch.windup_min=-110;
	pid_rate_pitch.windup_max=+110;
	pid_rate_pitch.kp=0.0;
	pid_rate_pitch.ki=0.0;
	pid_rate_pitch.kd=0.0;

	////////////////////////////////////////////////////

	PID pid_angle_roll;
	pid_angle_roll.Setpoint=0;
	pid_angle_roll.windup_min=-180;
	pid_angle_roll.windup_max=+180;
	pid_angle_roll.kp=0.0;
	pid_angle_roll.ki=0.0;
	pid_angle_roll.kd=0.0;

	PID pid_rate_roll;
	pid_rate_roll.Setpoint=0;
	pid_rate_roll.windup_min=-110;
	pid_rate_roll.windup_max=+110;
	pid_rate_roll.kp=0.0;
	pid_rate_roll.ki=0.0;
	pid_rate_roll.kd=0.0;

	////////////////////////////////////////////////////

	PID pid_angle_yaw;
	pid_angle_yaw.Setpoint=0;
	pid_angle_yaw.windup_min=-180;
	pid_angle_yaw.windup_max=+180;
	pid_angle_yaw.kp=0.0;
	pid_angle_yaw.ki=0.0;
	pid_angle_yaw.kd=0.0;

	PID pid_rate_yaw;
	pid_rate_yaw.Setpoint=0;
	pid_rate_yaw.windup_min=-110;
	pid_rate_yaw.windup_max=+110;
	pid_rate_yaw.kp=0.0;
	pid_rate_yaw.ki=0.0;
	pid_rate_yaw.kd=0.0;

	////////////////////////////////////////////////////


	while(true){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));

		cmd = atomic_joystick_cmd;
		fusion = atomic_navboard_fusion;

		if(cmd.takeoff){
			takeoff=1;
			printf("takeoff\r\n");
		}

		if(cmd.emergency){
			printf("emergency\r\n");
			//
			height_speed=0;
			pitch_speed=0;
			roll_speed=0;
			yaw_speed=0;
			takeoff=0;
		}


		if(takeoff){


			/////////////////////////////////////////////////////////

#define MAX_PITCH_ROLL_ANGLE 30
#define MAX_YAW_ANGLE 180

			pid_angle_pitch.kp = 2;
			pid_angle_pitch.Setpoint = constraint_s16(cmd.pitch_speed,-MAX_PITCH_ROLL_ANGLE,MAX_PITCH_ROLL_ANGLE);
			pid_angle_pitch.Input = fusion.acc_pitch;
			pid_angle_pitch.Compute();

			pid_angle_roll.kp = 2;
			pid_angle_roll.Setpoint=constraint_s16(cmd.roll_speed,-MAX_PITCH_ROLL_ANGLE,MAX_PITCH_ROLL_ANGLE);
			pid_angle_roll.Input = fusion.acc_roll;
			pid_angle_roll.Compute();

			/*
			pid_angle_yaw.kp = 2;
			pid_angle_yaw.Setpoint=constraint_s16(cmd.yaw_speed,-MAX_YAW_ANGLE,MAX_YAW_ANGLE);
			pid_angle_yaw.Input = fusion.gyro_z_integrate;
			pid_angle_yaw.Compute();
*/

			////////////////////////////////////////////////////////////////////

#define MAX_PITH_ROLL_YAW_SPEED 150

			pid_rate_pitch.kp = 2;
			pid_rate_pitch.Setpoint = constraint_s16(pid_angle_pitch.Output,-MAX_PITH_ROLL_YAW_SPEED,MAX_PITH_ROLL_YAW_SPEED);
			pid_rate_pitch.Input = fusion.gyro_x;
			pid_rate_pitch.Compute();

			pid_rate_roll.kp = 2;
			pid_rate_roll.Setpoint = constraint_s16(pid_angle_roll.Output,-MAX_PITH_ROLL_YAW_SPEED,MAX_PITH_ROLL_YAW_SPEED);
			pid_rate_roll.Input = fusion.gyro_y;
			pid_rate_roll.Compute();

			pid_rate_yaw.kp = 2;
			pid_rate_yaw.Setpoint = constraint_s16(cmd.yaw_speed,-MAX_PITH_ROLL_YAW_SPEED,MAX_PITH_ROLL_YAW_SPEED);
			pid_rate_yaw.Input = fusion.gyro_z;
			pid_rate_yaw.Compute();

			/////////////////////////////////////////////////////////

			pitch_speed=pid_rate_pitch.Output;
			roll_speed=pid_rate_roll.Output;
			yaw_speed=pid_rate_yaw.Output;

			height_speed+=cmd.height_speed;

			//clamp
			height_speed=constraint_s16(height_speed,      50    ,  300  );
			pitch_speed =constraint_s16(pitch_speed ,      -100  ,  100  );
			roll_speed  =constraint_s16(roll_speed  ,      -100  ,  100  );
			yaw_speed   =constraint_s16(yaw_speed   ,      -100  ,  100  );
		}

		//mix table
		speeds_user.front_left  = height_speed +constraint_s16((-pitch_speed + roll_speed - yaw_speed),0,511);
		speeds_user.front_right = height_speed +constraint_s16((-pitch_speed - roll_speed + yaw_speed),0,511);
		speeds_user.rear_left   = height_speed +constraint_s16((+pitch_speed + roll_speed + yaw_speed),0,511);
		speeds_user.rear_right  = height_speed +constraint_s16((+pitch_speed - roll_speed - yaw_speed),0,511);

		//send to motors
		atomic_motor_speed=speeds_user;



	}
}
