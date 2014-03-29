/*
 * prvGetRegistersFromStack.h
 *
 *  Created on: 23/02/2014
 *      Author: mpontes
 */

#ifndef PRVGETREGISTERSFROMSTACK_H_
#define PRVGETREGISTERSFROMSTACK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* The prototype shows it is a naked function - in effect this is just an
assembly function. */
void FaultISR( void ) __attribute__( ( naked ) );

void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress );


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* PRVGETREGISTERSFROMSTACK_H_ */
