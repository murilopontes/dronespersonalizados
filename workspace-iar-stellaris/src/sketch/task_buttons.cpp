
//ENERGIA
#include "Arduino.h"
#include "pitches.h"

//FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//TASK IT-SELF
#include "task_buttons.h"

extern "C" void task_buttons(void *pvParameters){
  pinMode(PUSH1,INPUT_PULLUP);
  pinMode(PUSH2,INPUT_PULLUP);
  
  int buttonState1_back = digitalRead(PUSH1);
  int buttonState2_back = digitalRead(PUSH2);
  
  while(1){
    int buttonState1_now = digitalRead(PUSH1);
    int buttonState2_now = digitalRead(PUSH2);
    
    if(buttonState1_now!=buttonState1_back){
      if(buttonState1_now==0){
        Serial.println("SW1 press");
      } else {
        Serial.println("SW1 release");
      }
      buttonState1_back=buttonState1_now;
    }
    if(buttonState2_now!=buttonState2_back){
      if(buttonState2_now==0){
        Serial.println("SW2 press");
      } else {
        Serial.println("SW2 release");
      }
      buttonState2_back=buttonState2_now;
    }
    vTaskDelay(5);
  }
  
}




