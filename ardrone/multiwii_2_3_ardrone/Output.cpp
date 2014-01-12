#include "Arduino.h"
#include "config.h"
#include "def.h"
#include "types.h"
#include "MultiWii.h"
#include "Alarms.h"


/**************************************************************************************/
/***************   Writes the Servos values to the needed format   ********************/
/**************************************************************************************/
void writeServos() {
	printf("writeServos\r\n");
}

/**************************************************************************************/
/************  Writes the Motors values to the PWM compare register  ******************/
/**************************************************************************************/
void writeMotors() { // [1000;2000] => [125;250]
	printf("writeMotors %f %f %f %f\r\n", motor[0], motor[1], motor[2], motor[3]);

}

/**************************************************************************************/
/************          Writes the mincommand to all Motors           ******************/
/**************************************************************************************/
void writeAllMotors(int16_t mc) {   // Sends commands to all motors
	printf("writeAllMotors\r\n");
  for (uint8_t i =0;i<NUMBER_MOTOR;i++) {
    motor[i]=mc;
  }
  writeMotors();
}

/**************************************************************************************/
/************        Initialize the PWM Timers and Registers         ******************/
/**************************************************************************************/
void initOutput() {
	printf("motor init\r\n");
}


/**************************************************************************************/
/********** Mixes the Computed stabilize values to the Motors & Servos  ***************/
/**************************************************************************************/

// get servo middle point from Config or from RC-Data
int16_t get_middle(uint8_t nr) {
  return (conf.servoConf[nr].middle < RC_CHANS) ? rcData[conf.servoConf[nr].middle] : conf.servoConf[nr].middle;
}



void mixTable() {
  double maxMotor;
  uint8_t i;

  printf("mixTable() -> motors PIDMIX\r\n");

  #define PIDMIX(X,Y,Z) rcCommand[THROTTLE] + axisPID[ROLL]*X + axisPID[PITCH]*Y + YAW_DIRECTION * axisPID[YAW]*Z

  /****************                   main Mix Table                ******************/
  #if defined( QUADP )
    motor[0] = PIDMIX( 0,+1,-1); //REAR
    motor[1] = PIDMIX(-1, 0,+1); //RIGHT
    motor[2] = PIDMIX(+1, 0,+1); //LEFT
    motor[3] = PIDMIX( 0,-1,-1); //FRONT
  #elif defined( QUADX )
    motor[0] = PIDMIX(-1,+1,-1); //REAR_R
    motor[1] = PIDMIX(-1,-1,+1); //FRONT_R
    motor[2] = PIDMIX(+1,+1,+1); //REAR_L
    motor[3] = PIDMIX(+1,-1,-1); //FRONT_L
  #else
    #error "missing coptertype mixtable entry. Either you forgot to define a copter type or the mixing table is lacking neccessary code"
  #endif 

  printf("mixTable() -> after mix -> %.2f %.2f %.2f %.2f\r\n", motor[0], motor[1], motor[2], motor[3]);

  /****************                normalize the Motors values                ******************/

    printf("mixTable() -> normalize the Motors values\r\n");
    maxMotor=motor[0];
	for (i = 1; i< NUMBER_MOTOR; i++){
		if (motor[i]>maxMotor){
			maxMotor = motor[i];
		}
	}

    for(i=0; i< NUMBER_MOTOR; i++) {

		//
		if (maxMotor > MAXTHROTTLE){
			// this is a way to still have good gyro corrections if at least one motor reaches its max.
			motor[i] -= maxMotor - MAXTHROTTLE;
		}

		//apply constrain
		motor[i] = constrain(motor[i], conf.minthrottle, MAXTHROTTLE);
      
		//if not barometer present, check for minthrottle
		if ((rcData[THROTTLE] < MINCHECK) && !f.BARO_MODE){
			motor[i] = conf.minthrottle;
		}

		//if not armed use mincommand to stop motor
		if (!f.ARMED){
			motor[i] = MINCOMMAND;
		}
    }
	printf("mixTable() -> after normalize -> %.2f %.2f %.2f %.2f\r\n", motor[0], motor[1], motor[2], motor[3]);

}
