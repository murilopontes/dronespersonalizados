
//ENERGIA
#include "Arduino.h"
#include "pitches.h"

//FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//TASK IT-SELF
#include "task_adc.h"

//PWM-Write
#include "wiring_private.h"


extern "C" void task_adc(void *pvParameters){
  
  pinMode(RED_LED, OUTPUT); 
  pinMode(BLUE_LED, OUTPUT); 
  pinMode(GREEN_LED, OUTPUT);   
  
  while(1){
    
    //ADC has 12bits --> 2 ^ 12 = 4096
    int sensorValue = analogRead(PB_5);
    Serial.print("ADC PB5=");
    Serial.print(sensorValue);
    

    
    //PWM 50 Hz, 20ms
    //|--------------20ms --------------\
    //| 1ms  | 1ms  | 1ms  | .......     | 
    //| 1000 | 1000 | 1000 | .......     |
    //|----------------------------------
    //|      |^^^^^^^^^^^^^|
    //|------| min ... max |-------------
    //|------| motor speed |-------------
    //|      ^             ^            ^
    //|     1000         2000         20000
    //|----------------------------------
    
    // analog-res= 1000 x 20 = 20000
    //|    
    /*
    int pwmValue = map(sensorValue, 0, 4095, 999, 1999);
    Serial.print(" mapped to PWM=");
    Serial.println(pwmValue);
    
    PWMWrite(PC_6,20000,pwmValue,50);
    PWMWrite(PC_7,20000,pwmValue,50);
    PWMWrite(PD_6,20000,pwmValue,50);
    PWMWrite(PD_7,20000,pwmValue,50);
    */
    
    int pwmLedValue = map(sensorValue, 0, 4095, 1, 254);
    Serial.print(" mapped to PWM=");
    Serial.println(pwmLedValue);
    
      analogWrite(RED_LED,pwmLedValue);
      analogWrite(BLUE_LED,pwmLedValue);
      analogWrite(GREEN_LED,pwmLedValue);
      /*
      analogWrite(PC_6,pwmLedValue);
      analogWrite(PC_7,pwmLedValue);
      analogWrite(PD_6,pwmLedValue);
      analogWrite(PD_7,pwmLedValue);
    */
    PWMWrite(PC_6,255,pwmLedValue,50);
    PWMWrite(PC_7,255,pwmLedValue,50);
    PWMWrite(PD_6,255,pwmLedValue,50);
    PWMWrite(PD_7,255,pwmLedValue,50);
  }
}




