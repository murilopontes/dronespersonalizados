/*
 * murix_gpio.cpp
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */


#include "murix_gpio.h"

#include "murix_cpp_headers.h"

int gpio_set(int nr,gpio_action val)
{

	char cmdline[200];

	switch(val){
	case GPIO_IN:
		sprintf(cmdline,"/usr/sbin/gpio %d -d i",nr);
		break;
	case GPIO_OUT_1:
		sprintf(cmdline,"/usr/sbin/gpio %d -d ho 1",nr);
		break;
	case GPIO_OUT_0:
		sprintf(cmdline,"/usr/sbin/gpio %d -d ho 0",nr);
		break;
	}
	return system(cmdline);

}
