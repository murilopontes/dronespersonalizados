/*
 * Arduino.h
 *
 *  Created on: 12/01/2014
 *      Author: mpontes
 */

#ifndef ARDUINO_H_
#define ARDUINO_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "math.h"

int millis(void);
int micros(void);
void delay(int);
int constrain(int x, int a, int b);
int min(int a,int b);
int max(int a,int b);
int sq(int a);
#define PI 3.14

#define BUZZERPIN_OFF 0
#define LEDPIN_OFF 0
#define LEDPIN_TOOGLE 0



#define ROLLPIN 0
#define PITCHPIN 0
#define YAWPIN 0
#define THROTTLEPIN 0
#define AUX1PIN 0
#define AUX2PIN 0
#define AUX3PIN 0
#define AUX4PIN 0

#define PCINT_PIN_COUNT 1
#define PCINT_RX_BITS 0

#define PGM_P int

#endif /* ARDUINO_H_ */
