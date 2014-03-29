/*
 * murix_constraint.h
 *
 *  Created on: 20/01/2014
 *      Author: mpontes
 */

#ifndef MURIX_CONSTRAINT_H_
#define MURIX_CONSTRAINT_H_

#include <stdint.h>

double constraint_double(double value,double min,double max);
int16_t constraint_s16(int16_t value,int16_t min,int16_t max);


#endif /* MURIX_CONSTRAINT_H_ */
