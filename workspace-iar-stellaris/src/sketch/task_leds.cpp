
//ENERGIA
#include "Arduino.h"
#include "pitches.h"

//FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//TASK IT-SELF
#include "task_leds.h"

//PWM-Write
#include "wiring_private.h"


extern "C" void task_leds(void *pvParameters){
  
  pinMode(RED_LED, OUTPUT); 
  pinMode(BLUE_LED, OUTPUT); 
  pinMode(GREEN_LED, OUTPUT); 
  
  int pwmValue=1;  
  
  int i=0;
  
  while(1){
    
    if(i==0){
      analogWrite(RED_LED,0);
      analogWrite(BLUE_LED,0);
      analogWrite(GREEN_LED,pwmValue);
    } else if(i==1){
      analogWrite(RED_LED,0);
      analogWrite(BLUE_LED,pwmValue);
      analogWrite(GREEN_LED,0);
    } else if(i==2){
      //PWMWrite(RED_LED,1000,500,50);
      analogWrite(RED_LED,pwmValue);
      analogWrite(BLUE_LED,0);
      analogWrite(GREEN_LED,0);
    }
    i++;
    if(i>2){
      i=0;
    }
    
    vTaskDelay(2000);
  }
  
}




