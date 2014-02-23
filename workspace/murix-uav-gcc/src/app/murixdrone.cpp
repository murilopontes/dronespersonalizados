

#include "murixdrone.h"

/////////////////////////////////////////////////////////////////

#include <murix_pid.h>

////////////////////////////////////////////////////////////////
// I2C driver
#include <Wire.h>
// PWM Write
#include <wiring_private.h>
// GY521 driver
#include <GY521.h>

/////////////////////////////////////////////////////////////////
// SPI driver
#include <SPI.h>
// CC1101 driver
#include <cc1101.h>


////////////////////////////////////////////////////////////////////
// Bitlash

#include <bitlash.h>
#include <src/bitlash.h>  // for sp() and printInteger()

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


//new instance of GY521 on Stellaris I2C port 3
GY521 imu(3);


numvar bitlash_cmd_imu_flat_trim(void){
	//
	imu.init();
	//
	Serial.println("wait calibrating...");
	imu.flat_trim(1000);
	Serial.println("calibration done!");
	return 0;
}


numvar bitlash_cmd_imu(void){
	//
	imu.update();
	//
/*
	Serial.printf("%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|\r\n",
			//mpu6050 temperature
			imu.sensors.temp,
			//tracking interval time
			imu.tracking.imu_dt,
			//accel
			imu.sensors.ax,
			imu.sensors.ay,
			imu.sensors.az,
			//accel integrate -> speed
			imu.tracking.acc_linear_x_speed[1],
			imu.tracking.acc_linear_y_speed[1],
			imu.tracking.acc_linear_z_speed[1],
			//accel integrate2 -> position
			imu.tracking.acc_linear_x_position[1],
			imu.tracking.acc_linear_y_position[1],
			imu.tracking.acc_linear_z_position[1],
			//accel gravity
			imu.tracking.gravity_x,
			imu.tracking.gravity_y,
			imu.tracking.gravity_z,
			//accel pitch and roll
			imu.tracking.acc_pitch,
			imu.tracking.acc_roll,
			//gyro
			imu.sensors.gx,
			imu.sensors.gy,
			imu.sensors.gz,
			//gyro integrate -> pitch, roll, yaw
			imu.tracking.gyro_x_integrate,
			imu.tracking.gyro_y_integrate,
			imu.tracking.gyro_z_integrate,
			//fusion pitch, roll
			imu.tracking.fusion_pitch,
			imu.tracking.fusion_roll);

	Serial.printf("ok\r\n");
        */
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


class drone {
public:

	int pilot_throttle;
	int pilot_pitch;
	int pilot_roll;
	int pilot_yaw;
	int pilot_takeoff;
	int pilot_emergency;

	int is_live;

	int speed_height;
	int speed_pitch;
	int speed_roll;
	int speed_yaw;

	int pwm_front[2];
	int pwm_rear[2];
	int pwm_left[2];
	int pwm_right[2];



	drone(){
		pilot_throttle=0;
		pilot_pitch=0;
		pilot_roll=0;
		pilot_yaw=0;
		pilot_takeoff=0;
		pilot_emergency=0;

		is_live=0;

		speed_height=0;
		speed_pitch=0;
		speed_roll=0;
		speed_yaw=0;

		memset(pwm_front,0,sizeof(pwm_front));
		memset(pwm_rear,0,sizeof(pwm_rear));
		memset(pwm_left,0,sizeof(pwm_left));
		memset(pwm_right,0,sizeof(pwm_right));

	}

	////////////////////////////////////////////////////

	PID pid_angle_pitch;
	PID pid_rate_pitch;
	PID pid_angle_roll;
	PID pid_rate_roll;
	PID pid_angle_yaw;
	PID pid_rate_yaw;

	void pids_init(){

		pid_angle_pitch.Init();
		pid_angle_pitch.Setpoint=0;
		pid_angle_pitch.windup_min=-180;
		pid_angle_pitch.windup_max=+180;
		pid_angle_pitch.kp=0.0;
		pid_angle_pitch.ki=0.0;
		pid_angle_pitch.kd=0.0;

		pid_rate_pitch.Init();
		pid_rate_pitch.Setpoint=0;
		pid_rate_pitch.windup_min=-110;
		pid_rate_pitch.windup_max=+110;
		pid_rate_pitch.kp=0.0;
		pid_rate_pitch.ki=0.0;
		pid_rate_pitch.kd=0.0;

		pid_angle_roll.Init();
		pid_angle_roll.Setpoint=0;
		pid_angle_roll.windup_min=-180;
		pid_angle_roll.windup_max=+180;
		pid_angle_roll.kp=0.0;
		pid_angle_roll.ki=0.0;
		pid_angle_roll.kd=0.0;

		pid_rate_roll.Init();
		pid_rate_roll.Setpoint=0;
		pid_rate_roll.windup_min=-110;
		pid_rate_roll.windup_max=+110;
		pid_rate_roll.kp=0.0;
		pid_rate_roll.ki=0.0;
		pid_rate_roll.kd=0.0;

		pid_angle_yaw.Init();
		pid_angle_yaw.Setpoint=0;
		pid_angle_yaw.windup_min=-180;
		pid_angle_yaw.windup_max=+180;
		pid_angle_yaw.kp=0.0;
		pid_angle_yaw.ki=0.0;
		pid_angle_yaw.kd=0.0;

		pid_rate_yaw.Init();
		pid_rate_yaw.Setpoint=0;
		pid_rate_yaw.windup_min=-110;
		pid_rate_yaw.windup_max=+110;
		pid_rate_yaw.kp=0.0;
		pid_rate_yaw.ki=0.0;
		pid_rate_yaw.kd=0.0;
	}


	void pilot_man(){


		/////////////////////////////////
		if(pilot_takeoff){

			memset(pwm_front,0,sizeof(pwm_front));
			memset(pwm_rear,0,sizeof(pwm_rear));
			memset(pwm_left,0,sizeof(pwm_left));
			memset(pwm_right,0,sizeof(pwm_right));

			is_live=1;
		}
		////////////////////////
		if(pilot_emergency){
			is_live=0;

			memset(pwm_front,0,sizeof(pwm_front));
			memset(pwm_rear,0,sizeof(pwm_rear));
			memset(pwm_left,0,sizeof(pwm_left));
			memset(pwm_right,0,sizeof(pwm_right));

			motor_all(0);
		}

		////////////////////////////////////////////////
		if(is_live){
			//
			speed_height=pilot_throttle;
			speed_pitch=pilot_pitch;
			speed_roll=pilot_roll;
			speed_yaw=pilot_yaw;

			//backup
			pwm_front[0]=pwm_front[1];
			pwm_rear[0]=pwm_rear[1];
			pwm_left[0]=pwm_left[1];
			pwm_right[0]=pwm_right[1];

			//mix table
			pwm_front[1]=constrain(speed_height+speed_yaw-speed_pitch,130,254);
			pwm_rear[1]=constrain(speed_height+speed_yaw+speed_pitch,130,254);
			pwm_left[1]=constrain(speed_height-speed_yaw+speed_roll,130,254);
			pwm_right[1]=constrain(speed_height-speed_yaw-speed_roll,130,254);

			//diff from backup -> update
			if(pwm_front[0]!=pwm_front[1]) motor_front(pwm_front[1]);
			if(pwm_rear[0]!=pwm_rear[1]) motor_rear(pwm_rear[1]);
			if(pwm_left[0]!=pwm_left[1]) motor_left(pwm_left[1]);
			if(pwm_right[0]!=pwm_right[1]) motor_right(pwm_right[1]);

		}

	}



	void pilot_auto(){


		if(pilot_takeoff){
			is_live=1;
			pids_init();

			memset(pwm_front,0,sizeof(pwm_front));
			memset(pwm_rear,0,sizeof(pwm_rear));
			memset(pwm_left,0,sizeof(pwm_left));
			memset(pwm_right,0,sizeof(pwm_right));
		}

		if(pilot_emergency){
			is_live=0;

			memset(pwm_front,0,sizeof(pwm_front));
			memset(pwm_rear,0,sizeof(pwm_rear));
			memset(pwm_left,0,sizeof(pwm_left));
			memset(pwm_right,0,sizeof(pwm_right));

			motor_all(0);
		}

		if(is_live){

			imu.update();
			int max_angle_change=30;
			int max_rotate_speed=100;
			/////////////////////////////////////////////////////////
			//PID1 - setpoint=joystick input=angle
			pid_angle_pitch.kp = 2;
			pid_angle_pitch.Setpoint = constrain(pilot_pitch,-max_angle_change,max_angle_change);
			pid_angle_pitch.Input = imu.tracking.fusion_pitch;
			pid_angle_pitch.Compute();
			//PID2 - setpoint=PID1.out input=gyro
			pid_rate_pitch.kp = 2;
			pid_rate_pitch.Setpoint = constrain(pid_angle_pitch.Output,-max_rotate_speed,max_rotate_speed);
			pid_rate_pitch.Input = imu.sensors.gx;
			pid_rate_pitch.Compute();
			/////////////////////////////////////////////////////////
			//PID1
			//pid_angle_roll.kp = 2;
			pid_angle_roll.Setpoint=constrain(pilot_roll,-max_angle_change,max_angle_change);
			pid_angle_roll.Input = imu.tracking.fusion_roll;
			pid_angle_roll.Compute();
			//PID2
			//pid_rate_roll.kp = 2;
			pid_rate_roll.Setpoint = constrain(pid_angle_roll.Output,-max_rotate_speed,max_rotate_speed);
			pid_rate_roll.Input = imu.sensors.gy;
			pid_rate_roll.Compute();
			/////////////////////////////////////////////////////////
			//PID1
			//pid_angle_yaw.kp=2;
			pid_angle_yaw.Setpoint=constrain(pilot_yaw,-max_angle_change,max_angle_change);
			pid_angle_yaw.Input = imu.tracking.fusion_yaw;
			pid_angle_yaw.Compute();
			//PID2
			//pid_rate_yaw.kp=2;
			pid_rate_yaw.Setpoint = constrain(pid_angle_yaw.Output,-max_rotate_speed,max_rotate_speed);
			pid_rate_yaw.Input = imu.sensors.gz;
			pid_rate_yaw.Compute();


			/////////////////////////////////////////////////////////
			pilot_throttle = constrain(pilot_throttle, -10  ,  10  );


			speed_height += pilot_throttle;
			speed_pitch=pid_rate_pitch.Output;
			speed_roll=pid_rate_roll.Output;
			speed_yaw=pid_rate_yaw.Output;

			//backup
			pwm_front[0]=pwm_front[1];
			pwm_rear[0]=pwm_rear[1];
			pwm_left[0]=pwm_left[1];
			pwm_right[0]=pwm_right[1];

			//mix table
			pwm_front[1]=constrain(speed_height+speed_yaw-speed_pitch,130,254);
			pwm_rear[1]=constrain(speed_height+speed_yaw+speed_pitch,130,254);
			pwm_left[1]=constrain(speed_height-speed_yaw+speed_roll,130,254);
			pwm_right[1]=constrain(speed_height-speed_yaw-speed_roll,130,254);

			//diff from backup -> update
			if(pwm_front[0]!=pwm_front[1]) motor_front(pwm_front[1]);
			if(pwm_rear[0]!=pwm_rear[1]) motor_rear(pwm_rear[1]);
			if(pwm_left[0]!=pwm_left[1]) motor_left(pwm_left[1]);
			if(pwm_right[0]!=pwm_right[1]) motor_right(pwm_right[1]);

			delay(5);
		}




	}



	void motor_front(int duty){
		if(duty<110) duty=110;
		if(duty>254) duty=254;
		analogWrite(PC_7,duty);
	}
	void motor_rear(int duty){
		if(duty<110) duty=110;
		if(duty>254) duty=254;
		analogWrite(PD_6,duty);
	}
	void motor_right(int duty){
		if(duty<110) duty=110;
		if(duty>254) duty=254;
		analogWrite(PC_6,duty);
	}
	void motor_left(int duty){
		if(duty<110) duty=110;
		if(duty>254) duty=254;
		analogWrite(PD_7,duty);
	}
	void motor_all(int duty){
		if(duty<110) duty=110;
		if(duty>254) duty=254;
		analogWrite(PC_6,duty);
		analogWrite(PC_7,duty);
		analogWrite(PD_6,duty);
		analogWrite(PD_7,duty);
	}

	void motors_init(){
		//
		pinMode(PC_6,OUTPUT);
		pinMode(PC_7,OUTPUT);
		pinMode(PD_6,OUTPUT);
		pinMode(PD_7,OUTPUT);
		motor_all(110);
	}


};

drone drone_pilot;





numvar bitlash_fly_auto(void){
	drone_pilot.pilot_auto();
	return 0;
}
numvar bitlash_fly_man(void){
	drone_pilot.pilot_man();
	return 0;
}

numvar bitlash_cmd_motor_front(void){
	int duty = getarg(1);
	drone_pilot.motor_front(duty);
	return 0;

}
numvar bitlash_cmd_motor_rear(void){
	int duty = getarg(1);
	drone_pilot.motor_rear(duty);
	return 0;

}
numvar bitlash_cmd_motor_right(void){
	int duty = getarg(1);
	drone_pilot.motor_right(duty);
	return 0;

}
numvar bitlash_cmd_motor_left(void){
	int duty = getarg(1);
	drone_pilot.motor_left(duty);
	return 0;
}

numvar bitlash_cmd_all_motor(void){
	int duty = getarg(1);
	drone_pilot.motor_all(duty);
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


typedef struct {

	int16_t pitch;
	int16_t roll;
	int16_t yaw;
	int16_t throttle;
	uint8_t takeoff;
	uint8_t emergency;

} packet_t;

void cc1101_rx_callback(uint8_t* buf,uint8_t buflen){
	analogWrite(GREEN_LED ,30);

	if(buflen<2){
		goto rx_end;
	}

	if(buf[0]==sizeof(packet_t)){

		packet_t* px = (packet_t*) &buf[1];

		drone_pilot.pilot_pitch     =  (px->pitch/10000.0)*254;
		drone_pilot.pilot_roll      =  (px->roll/10000.0)*254;
		drone_pilot.pilot_yaw       =  (px->yaw/10000.0)*254;
		drone_pilot.pilot_throttle  =  (px->throttle/10000.0)*254;
		drone_pilot.pilot_takeoff   =  px->takeoff;
		drone_pilot.pilot_emergency =  px->emergency;

		/*
		Serial.printf("rf-pilot %d %d %d %d %d %d\r\n",
				drone_pilot.pilot_pitch,
				drone_pilot.pilot_roll,
				drone_pilot.pilot_yaw,
				drone_pilot.pilot_throttle,
				drone_pilot.pilot_takeoff,
				drone_pilot.pilot_emergency
				);
				*/

		uint8_t rf_ack[2] = { 0xca, 0xfe };
		analogWrite(BLUE_LED ,30);
		cc1101_packet_send(rf_ack,sizeof(rf_ack));
		analogWrite(BLUE_LED ,0);
	}

	rx_end:
	analogWrite(GREEN_LED ,0);
}


numvar bitlash_cmd_cc1101_send(void) {
	char* texto=(char *) getarg(1);
	Serial.println(texto);
	Serial.println(strlen(texto));
	cc1101_packet_send((uint8_t*)texto,strlen(texto));
	return 0;
}

numvar bitlash_cmd_cc1101_reset(void) {
	cc1101_init(12,0,14,cc1101_rx_callback);
	return 0;
}
numvar bitlash_cmd_cc1101_debugstate(void) {
	cc1101_debug_state();
	return 0;
}
numvar bitlash_cmd_cc1101_don(void) {
	cc1101_debug_on();
	return 0;
}
numvar bitlash_cmd_cc1101_doff(void) {
	cc1101_debug_off();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


void leds_init(){
	//
	pinMode(BLUE_LED,OUTPUT);
	pinMode(RED_LED,OUTPUT);
	pinMode(GREEN_LED,OUTPUT);
	//
	analogWrite(BLUE_LED ,0);
	analogWrite(RED_LED  ,1);
	analogWrite(GREEN_LED,0);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


void murixdrone_setup(void){

	//
	leds_init();
	//
	drone_pilot.motors_init();

	//
	initBitlash(115200);
	//
	bitlash_cmd_cc1101_reset();
	//
	imu.init();
	bitlash_cmd_imu_flat_trim();





	addBitlashFunction("flat", (bitlash_function) bitlash_cmd_imu_flat_trim);
	addBitlashFunction("imu", (bitlash_function) bitlash_cmd_imu);

	addBitlashFunction("ccsend", (bitlash_function) bitlash_cmd_cc1101_send);
	addBitlashFunction("ccreset", (bitlash_function) bitlash_cmd_cc1101_reset);
	addBitlashFunction("ccdebug", (bitlash_function) cc1101_debug_state);
	addBitlashFunction("ccdon", (bitlash_function) bitlash_cmd_cc1101_don);
	addBitlashFunction("ccdoff", (bitlash_function) bitlash_cmd_cc1101_doff);

	addBitlashFunction("motor_all", (bitlash_function) bitlash_cmd_all_motor);
	addBitlashFunction("motor_front", (bitlash_function) bitlash_cmd_motor_front);
	addBitlashFunction("motor_rear", (bitlash_function) bitlash_cmd_motor_rear);
	addBitlashFunction("motor_right", (bitlash_function) bitlash_cmd_motor_right);
	addBitlashFunction("motor_left", (bitlash_function) bitlash_cmd_motor_left);

	addBitlashFunction("fly_man", (bitlash_function) bitlash_fly_man);
	addBitlashFunction("fly_auto", (bitlash_function) bitlash_fly_auto);



}


void murixdrone_loop(void){
	runBitlash();
	//bitlash_fly_man();
	bitlash_fly_auto();
}





