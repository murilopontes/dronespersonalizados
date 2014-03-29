/*
 * freertos_application_stack_overflow_hook.c
 *
 *  Created on: 22/02/2014
 *      Author: mpontes
 */


#include <Energia.h>

#include <FreeRTOS.h>
#include <task.h>

extern "C" void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
  //
  // This function can not return, so loop forever.  Interrupts are disabled
  // on entry to this function, so no processor interrupts will interrupt
  // this loop.
  //
  while(1)
  {
    Serial.print("vApplicationStackOverflowHook=");
    Serial.println(pcTaskName);
  }
}
