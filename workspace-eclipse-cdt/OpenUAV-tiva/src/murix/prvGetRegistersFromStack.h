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
#ifdef __IAR_SYSTEMS_ICC__
void FaultISR( void );
#else
void FaultISR( void ) __attribute__( ( naked ) );
#endif

void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress );


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* PRVGETREGISTERSFROMSTACK_H_ */
