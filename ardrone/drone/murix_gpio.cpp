/*
 * murix_gpio.cpp
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */


#include "murix_gpio.h"

#include "murix_cpp_headers.h"

//val=0 -> set gpio output lo
//val=1 -> set gpio output hi
//val=-1 -> set gpio as input (output hi-Z)
int gpio_set(int nr,int val)
{
	//printf("gpio_set %d = %d\r\n",nr,val);
	char cmdline[200];
	if(val<0) sprintf(cmdline,"/usr/sbin/gpio %d -d i",nr);
	else if(val>0) sprintf(cmdline,"/usr/sbin/gpio %d -d ho 1",nr);
	else sprintf(cmdline,"/usr/sbin/gpio %d -d ho 0",nr);
	return system(cmdline);
}
