
//ENERGIA
#include "Arduino.h"

//FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//TASKS
#include "task_adc.h"
#include "task_bitlash.h"
#include "task_buttons.h"
#include "task_esc_program_card.h"
#include "task_hmc5883l.h"
#include "task_leds.h"
#include "task_mpu6050_dmp.h"


//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
extern "C" void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
      Serial.println("FreeRTOS API: vApplicationStackOverflowHook()");
    }
}

void setup()
{
  //Arduino startup setup:
  Serial.begin(115200);
  Serial.print("\r\n.\r\n.\r\n");
  Serial.println("Arduino API setup(): basic system setup OK!");
  
    xTaskCreate(task_buttons, (signed portCHAR *)"buttons", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(task_leds   , (signed portCHAR *)"leds"   , configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(task_bitlash, (signed portCHAR *)"bitlash", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    
    
    //xTaskCreate(task_adc,  (signed portCHAR *)"adc", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    //xTaskCreate(task_mpu6050_dmp, (signed portCHAR *)"6050dmp", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    //xTaskCreate(task_esc,  (signed portCHAR *)"escp", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    
    
    Serial.println("FreeRTOS API: vTaskStartScheduler()");
    vTaskStartScheduler();  
}

void loop()
{
  //Arduino loop:
  Serial.println("Arduino API loop(): never execute when running FreeRTOS");
}

