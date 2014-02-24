
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


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

	//mpu6050 temperature
	Serial.print(imu.sensors.temp,2);
	Serial.print("|");
	//tracking interval time
	Serial.print(imu.tracking.imu_dt,2);
	Serial.print("|");
	//accel
	Serial.print(imu.sensors.ax,2);
	Serial.print("|");
	Serial.print(imu.sensors.ay,2);
	Serial.print("|");
	Serial.print(imu.sensors.az,2);
	Serial.print("|");
	//accel integrate -> speed
	Serial.print(imu.tracking.acc_linear_x_speed[1],2);
	Serial.print("|");
	Serial.print(imu.tracking.acc_linear_y_speed[1],2);
	Serial.print("|");
	Serial.print(imu.tracking.acc_linear_z_speed[1],2);
	Serial.print("|");
	//accel integrate2 -> position
	Serial.print(imu.tracking.acc_linear_x_position[1],2);
	Serial.print("|");
	Serial.print(imu.tracking.acc_linear_y_position[1],2);
	Serial.print("|");
	Serial.print(imu.tracking.acc_linear_z_position[1],2);
	Serial.print("|");
	//accel gravity
	Serial.print(imu.tracking.gravity_x,2);
	Serial.print("|");
	Serial.print(imu.tracking.gravity_y,2);
	Serial.print("|");
	Serial.print(imu.tracking.gravity_z,2);
	Serial.print("|");
	//accel pitch and roll
	Serial.print(imu.tracking.acc_pitch,2);
	Serial.print("|");
	Serial.print(imu.tracking.acc_roll,2);
	Serial.print("|");
	//gyro
	Serial.print(imu.sensors.gx,2);
	Serial.print("|");
	Serial.print(imu.sensors.gy,2);
	Serial.print("|");
	Serial.print(imu.sensors.gz,2);
	Serial.print("|");
	//gyro integrate -> pitch, roll, yaw
	Serial.print(imu.tracking.gyro_x_integrate,2);
	Serial.print("|");
	Serial.print(imu.tracking.gyro_y_integrate,2);
	Serial.print("|");
	Serial.print(imu.tracking.gyro_z_integrate,2);
	Serial.print("|");
	//fusion pitch, roll
	Serial.print(imu.tracking.fusion_pitch,2);
	Serial.print("|");
	Serial.print(imu.tracking.fusion_roll,2);
	Serial.print("|");
	Serial.println("ok");

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


void task_dummy(void *arg){

	for(;;){
		Serial.println("teste");
		vTaskDelay(500);
	}
}

numvar bitbash_cmd_test_task_delete( void )
{
	TaskHandle_t xHandle = NULL;
	// Create the task, storing the handle.
	xTaskCreate( task_dummy, "NAME", 128, NULL, 0, &xHandle );

	//
	vTaskDelay(1500);

	// Use the handle to delete the task.
	if( xHandle != NULL )
	{
		vTaskDelete( xHandle );
	}
}



numvar bitlash_cmd_freertos_free(void) {







	char states[]={'R','R','B','S','E'};


	/* Take a snapshot of the number of tasks in case it changes while this
	    function is executing. */
	int uxArraySize = uxTaskGetNumberOfTasks();

	/* Allocate a TaskStatus_t structure for each task.  An array could be
	    allocated statically at compile time. */
	TaskStatus_t *pxTaskStatusArray = (TaskStatus_t *) pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

	if( pxTaskStatusArray != NULL )
	{
		//
		unsigned long ulTotalRunTime, ulStatsAsPercentage;

		/* Generate raw status information about each task. */
		uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

		/* For percentage calculations. */
		ulTotalRunTime /= 100UL;

		/* Avoid divide by zero errors. */
		if( ulTotalRunTime > 0 )
		{

			Serial.println("");
			Serial.println("-----------------------------------------------------");
			Serial.println("Name\t|Order\t|State\t|cPrio\t|bPrio\t|fStack\t|CPU%");
			Serial.println("-----------------------------------------------------");

			/* For each populated position in the pxTaskStatusArray array,
	            format the raw data as human readable ASCII data. */
			for(int x = 0; x < uxArraySize; x++ )
			{
				/* What percentage of the total run time has the task used?
	                This will always be rounded down to the nearest integer.
	                ulTotalRunTimeDiv100 has already been divided by 100. */
				ulStatsAsPercentage = pxTaskStatusArray[x].ulRunTimeCounter / ulTotalRunTime;

				Serial.print(pxTaskStatusArray[x].pcTaskName);
				Serial.print("\t| ");
				Serial.print(pxTaskStatusArray[x].xTaskNumber);
				Serial.print("\t| ");
				Serial.print(states[pxTaskStatusArray[x].eCurrentState]);
				Serial.print("\t| ");
				Serial.print(pxTaskStatusArray[x].uxCurrentPriority);
				Serial.print("\t| ");
				Serial.print(pxTaskStatusArray[x].uxBasePriority);
				Serial.print("\t| ");
				Serial.print(pxTaskStatusArray[x].usStackHighWaterMark);
				Serial.print("\t| ");
				Serial.println(ulStatsAsPercentage);


				//Serial.print(pxTaskStatusArray[x].ulRunTimeCounter);
				//Serial.print("\t| ");


			}
		}
		Serial.println("-----------------------------------------------------");

		/* The array is no longer needed, free the memory it consumes. */
		vPortFree( pxTaskStatusArray );
	}




	Serial.println("");

	extern unsigned long _text;
	extern unsigned long _etext;
	extern unsigned long _data;
	extern unsigned long _edata;
	extern unsigned long _bss;
	extern unsigned long _ebss;
	extern unsigned long _end;
	extern unsigned long _estack;
	extern uint32_t gBrkUsed;

	Serial.print((int)&_data,HEX);
	Serial.println(" data start (ram from code)");
	Serial.print((int)&_edata,HEX);
	Serial.println(" data end   (ram from code)");
	Serial.print((int)&_bss,HEX);
	Serial.println(" bss start (ram from zero)");
	Serial.print((int)&_ebss,HEX);
	Serial.println(" bss end   (ram from zero)");
	Serial.print((int)&_end,HEX);
	Serial.println(" start of heap");
	Serial.print((int)&_estack,HEX);
	Serial.println(" start of stack");

	Serial.println("");

	int program_size=(&_etext-&_text)*4;

	int data_size=(&_edata-&_data)*4;
	int bss_size=(&_ebss-&_bss)*4;
	int heap_stack_size=(&_estack-&_end)*4;

	int32_t free_heap = ((&_estack-&_end)*4)-gBrkUsed;
	int ram_total=(&_estack-&_data)*4;
	int ram_free=free_heap+xPortGetFreeHeapSize();

	Serial.print("GCC flash.used=");
	Serial.println(program_size);

	Serial.print("GCC ram.data.len=");
	Serial.println(data_size);
	Serial.print("GCC ram.bss.len=");
	Serial.println(bss_size);
	Serial.print("GCC ram.heapstack.len=");
	Serial.println(heap_stack_size);

	Serial.println("");

	Serial.print("GCC heap.used=");
	Serial.println(gBrkUsed);
	Serial.print("GCC heap.free=");
	Serial.println(free_heap);

	Serial.println("");

	Serial.print("FREERTOS configTOTAL_HEAP_SIZE=");
	Serial.println(configTOTAL_HEAP_SIZE);
	Serial.print("FREERTOS xPortGetMinimumEverFreeHeapSize=");
	Serial.println(xPortGetMinimumEverFreeHeapSize());
	Serial.print("FREERTOS xPortGetFreeHeapSize=");
	Serial.println(xPortGetFreeHeapSize());

	Serial.println("");

	Serial.print("GCC+FREERTOS ram total=");
	Serial.println(ram_total);

	Serial.print("GCC+FREERTOS ram  used=");
	Serial.println(ram_total-ram_free);

	Serial.print("GCC+FREERTOS ram free=");
	Serial.println(ram_free);

	return 0;
}







////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void task_drone_radio(void *arg){
	//
	bitlash_cmd_cc1101_reset();
	for(;;){
		vTaskDelay(100);
	}
	vTaskDelete(NULL);
}
void task_drone_leds(void *arg){
	//
	pinMode(BLUE_LED,OUTPUT);
	pinMode(RED_LED,OUTPUT);
	pinMode(GREEN_LED,OUTPUT);
	//
	analogWrite(BLUE_LED ,0);
	analogWrite(RED_LED  ,1);
	analogWrite(GREEN_LED,0);

	uint8_t i=0;
	for(;;){
		if( i++ % 2){
			analogWrite(RED_LED  ,i);
		} else {
			analogWrite(RED_LED  ,0);
		}
		vTaskDelay(500);
	}
	vTaskDelete(NULL);
}
void task_drone_imu(void *arg){
	//
	imu.init();
	bitlash_cmd_imu_flat_trim();
	for(;;){
		vTaskDelay(100);
	}
	vTaskDelete(NULL);
}
void task_drone_bitbash(void *arg){
	//
	initBitlash(115200);
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
	addBitlashFunction("rtosfree", (bitlash_function) bitlash_cmd_freertos_free);
	addBitlashFunction("rtosdelete", (bitlash_function) bitbash_cmd_test_task_delete);
	for(;;){
		runBitlash();
	}
	vTaskDelete(NULL);
}


void task_drone_motors(void *arg){
	//
	drone_pilot.motors_init();
	// Perform an action every 10 ticks.
	const TickType_t xFrequency = 10;
	TickType_t xLastWakeTime;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	for(;;){
		// Wait for the next cycle.
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		// Perform action here.
		bitlash_fly_auto();
		//bitlash_fly_man();
	}
	vTaskDelete(NULL);
}


void task_drone_all(void* arg)
{
	xTaskCreate(task_drone_bitbash, "bitlash", 256, NULL, 0, NULL);

	xTaskCreate(task_drone_radio, "radio", 128, NULL, 0, NULL);
	xTaskCreate(task_drone_leds, "leds", 128, NULL, 0, NULL);
	xTaskCreate(task_drone_imu, "imu", 128, NULL, 0, NULL);
	xTaskCreate(task_drone_motors, "motors", 128, NULL, 0, NULL);
	for(;;){
		vTaskDelay(100);
	}
	vTaskDelete(NULL);
}




