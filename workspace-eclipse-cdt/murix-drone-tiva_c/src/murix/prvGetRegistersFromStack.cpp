/*
 * prvGetRegistersFromStack.cpp
 *
 *  Created on: 23/02/2014
 *      Author: mpontes
 */

#include <Energia.h>

#include <prvGetRegistersFromStack.h>



/* The fault handler implementation calls a function called
prvGetRegistersFromStack(). */
void FaultISR(void)
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word prvGetRegistersFromStack    \n"
    );
}




void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
/* These are volatile to try and prevent the compiler/linker optimising them
away as the variables never actually get used.  If the debugger won't show the
values of the variables, make them global my moving their declaration outside
of this function. */
volatile uint32_t r0;
volatile uint32_t r1;
volatile uint32_t r2;
volatile uint32_t r3;
volatile uint32_t r12;
volatile uint32_t lr; /* Link register. */
volatile uint32_t pc; /* Program counter. */
volatile uint32_t psr;/* Program status register. */

    r0 = pulFaultStackAddress[ 0 ];
    r1 = pulFaultStackAddress[ 1 ];
    r2 = pulFaultStackAddress[ 2 ];
    r3 = pulFaultStackAddress[ 3 ];

    r12 = pulFaultStackAddress[ 4 ];
    lr = pulFaultStackAddress[ 5 ];
    pc = pulFaultStackAddress[ 6 ];
    psr = pulFaultStackAddress[ 7 ];




    Serial.println ("[Hard fault handler - all numbers in hex]");
    Serial.print ("R0 = "); Serial.println(r0,HEX);
    Serial.print ("R1 = "); Serial.println(r1,HEX);
    Serial.print ("R2 = "); Serial.println(r2,HEX);
    Serial.print ("R3 = "); Serial.println(r3,HEX);
    Serial.print ("R12 = "); Serial.println(r12,HEX);
    Serial.print ("LR [R14] = "); Serial.print(lr,HEX);  Serial.println(" subroutine call return address");
    Serial.print ("PC [R15] = "); Serial.print(pc,HEX);  Serial.println(" program counter");
    Serial.print ("PSR = "); Serial.println(psr,HEX);
    Serial.print ("BFAR = "); Serial.println((*((volatile unsigned long *)(0xE000ED38))),HEX);
    Serial.print ("CFSR = "); Serial.println((*((volatile unsigned long *)(0xE000ED28))),HEX);
    Serial.print ("HFSR = "); Serial.println((*((volatile unsigned long *)(0xE000ED2C))),HEX);
    Serial.print ("DFSR = "); Serial.println((*((volatile unsigned long *)(0xE000ED30))),HEX);
    Serial.print ("AFSR = "); Serial.println((*((volatile unsigned long *)(0xE000ED3C))),HEX);
    //Serial.print ("SCB_SHCSR = "); Serial.println(SCB->SHCSR);

    //software reset
    Serial.println("Hardfault, please reset system\r\n\r\n\r\n");
    HWREG(NVIC_APINT) = NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ;

      /* When the following line is hit, the variables contain the register values. */
    for( ;; ){
       Serial.println("Hardfault, please reset system");
    }



    /* When the following line is hit, the variables contain the register values. */
    for( ;; );
}








