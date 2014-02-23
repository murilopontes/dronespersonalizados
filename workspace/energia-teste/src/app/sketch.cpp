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
	xTaskCreate(led_Task, "led", 128, NULL, 0, NULL);
	vTaskStartScheduler();
}


void loop()
{
  // put your main code here, to run repeatedly:

}

