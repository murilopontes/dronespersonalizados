MURIX Internet of Things project
================================

MURIX Internet of Things implementation to:  
Texas Stellaris Launchpad (ek-lm4f120xl)  
Texas Tiva C Series Launchpad (ek-tm4c123gxl)  


CPU: LM4F120H5QR / LM4F230H5QR / TM4C123GH6PM  
CLOCK: 80 MHz  
FPU: yes  
RAM: 32k  
EEPROM: 2k  
FLASH: 256k  

# GPIOs

## WARNING MAX VOLTAGE
(LM4F120H5QR datasheet, page 611)  
All GPIO signals are 5-V tolerant when configured as inputs  
except for PD4, PD5, PB0 and PB1, which are limited to 3.6 V.  

## WARNING SHORT CIRCUIT
[LM4F120 LaunchPad User manual, page 21](https://raw.github.com/murix/murix-ek-lm4f120xl/master/datasheets/Stellaris-LM4F120-LaunchPad-schematic-spmu289c.pdf)


PD0 and PD1 are connected with PB6 and PB7 in short circuit (BOOM!!!)  
REMOVE R9 and R10 to be safe!!!!  

### GPIO-A
2 pins of Port A (PA0-PA1) used for virtual COM  
6 pins of Port A (PA2-PA7) free to be used  

### GPIO-B
8 pins of Port B (PB0-PB7) free to be used (WARNING PB6 and PB7)  

### GPIO-C
4 pins of Port C (PC0-PC3) used for JTAG debugging  
4 pins of Port C (PC4-PC7) free to be used  

### GPIO-D
2 pins of Port D (PD4-PD5) used for USB   
6 pins of Port D (PD0-PD3, PD6-PD7) free to be used (WARNING PD0 and PD1)  

### GPIO-E
6 pins of Port E (PE0-PE5) free to be used  

### GPIO-F
5 pins of Port F (PF0-PF4) used for onboard LEDs and buttons  
* PF4 GPIO SW1 Button
* PF0 GPIO SW2 Button
* PF1 GPIO RGB LED (Red)
* PF2 GPIO RGB LED (Blue)
* PF3 GPIO RGD LED (Green)



## Thanks to all people from:
FreeRTOS project  
http://www.freertos.org/  

Energia project  
http://www.energia.nu/

Bitlash project   
https://github.com/billroy/bitlash

Mbed project   
http://mbed.org/

## Required tools:
IAR for ARM   
My Recommendation: use code limited version   



