/*
 * murix_gpio.h
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#ifndef MURIX_GPIO_H_
#define MURIX_GPIO_H_


//val=0 -> set gpio output lo
//val=1 -> set gpio output hi
//val=-1 -> set gpio as input (output hi-Z)
int gpio_set(int nr,int val);


#endif /* MURIX_GPIO_H_ */
