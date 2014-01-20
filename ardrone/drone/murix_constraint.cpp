/*
 * murix_constraint.cpp
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */


#include "murix_constraint.h"

double constraint_double(double value,double min,double max){
	double out=value;
	if(out<min)out=min;
	if(out>max)out=max;
	return out;
}

int16_t constraint_s16(int16_t value,int16_t min,int16_t max){
	int16_t out=value;
	if(out<min)out=min;
	if(out>max)out=max;
	return out;
}


