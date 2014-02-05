
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

numvar timer1(void) { 
	return 4; 	// return the value of Timer 1
}

extern "C" void task_bitlash(void *pvParameters){
  
  //initBitlash(115200);
  initBitlash(115200);
  addBitlashFunction("timer1", (bitlash_function) timer1);
  
  while(1){
    runBitlash();
  }
  
}




