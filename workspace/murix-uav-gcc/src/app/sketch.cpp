/*
 * sketch.cpp
 *
 *  Created on: 23/02/2014
 *      Author: mpontes
 */

#include <Energia.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <bitlash.h>
#include <murixdrone.h>



void test_all_serials(void *pvParameters){

	Serial.begin(115200);
	Serial1.begin(115200);
	Serial2.begin(115200);
	Serial3.begin(115200);
	Serial4.begin(115200);
	Serial5.begin(115200);
	Serial6.begin(115200);
	Serial7.begin(115200);
	for(;;){
		Serial.println("0");
		Serial1.println("1");
		Serial2.println("2");
		Serial3.println("3");
		Serial4.println("4");
		Serial5.println("5");
		Serial6.println("6");
		Serial7.println("7");
		vTaskDelay(1000);
	}
}

void task_bitlash(void *pvParameters){

	initBitlash(115200);
	for(;;){
		runBitlash();
	}
}



void serial_events(void *pvParameters){
	for(;;){
		serialEventRun();
	}
}

void led_Task(void *pvParameters)
{
	pinMode(GREEN_LED,OUTPUT);
	int i=0;
	for(;;){


		if(i%2){
			analogWrite(GREEN_LED,10);
		} else {
			analogWrite(GREEN_LED,0);
		}
		i++;

		vTaskDelay(1000);
	}
}




void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);

	//xTaskCreate(led_Task, "led", 128, NULL, 0, NULL);
	//xTaskCreate(serial_events, "serialrx", 128, NULL, 0, NULL);
	//xTaskCreate(test_all_serials, "serialtx", 128, NULL, 0, NULL);
	//xTaskCreate(task_bitlash, "bitlash", 128, NULL, 0, NULL);

	create_all_drone_tasks();

	vTaskStartScheduler();
}


void loop()
{
	// put your main code here, to run repeatedly:
	Serial.println("freertos sched die!!");
}

