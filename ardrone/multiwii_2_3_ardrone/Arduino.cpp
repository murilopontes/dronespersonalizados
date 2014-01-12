/*
 * Arduino.cpp
 *
 *  Created on: 12/01/2014
 *      Author: mpontes
 */



#include "Arduino.h"

int millis(void){ return 1; }
int micros(void){ return 1; }
void delay(int){}

int constrain(int x, int a, int b){
	int s = x;
	if (x < a) s = a;
	if (x > b) s = b;
	return s;
}

int min(int a,int b){
	if (a < b) return a;
	else return b;
}

int max(int a,int b){
	if (a > b) return a;
	else return b;
}
int sq(int a){
	return a*a;
}

uint8_t PWM_PIN[8];

