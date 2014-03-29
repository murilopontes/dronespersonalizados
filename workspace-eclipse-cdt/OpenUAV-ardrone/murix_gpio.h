/*
 * murix_gpio.h
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#ifndef MURIX_GPIO_H_
#define MURIX_GPIO_H_



enum gpio_action {
	GPIO_OUT_0,
	GPIO_OUT_1,
	GPIO_IN
};

int gpio_set(int nr,gpio_action val);


#endif /* MURIX_GPIO_H_ */
