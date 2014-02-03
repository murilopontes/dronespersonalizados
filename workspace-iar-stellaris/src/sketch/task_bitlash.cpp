
//ENERGIA
#include "Arduino.h"
#include "pitches.h"

//FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//TASK IT-SELF
#include "task_bitlash.h"

#include "bitlash.h"

extern "C" void task_bitlash(void *pvParameters){
  
  initBitlash(115200);
  while(1){
    runBitlash();
  }
  
}




