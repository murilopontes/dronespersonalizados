Official source code repository for http://dronespersonalizados.blogspot.com.br/


workspace-eclipse-cdt (Embedded projects)
=========================================

Drone using Ar.Drone V1 
-----------------------
* ARM926 128M-RAM 128M-FLASH

Drone using Stellaris / Tiva
----------------------------
* ARM Cortex-M4F 32k-RAM 256k-FLASH 2k-eeprom 12xADC-12bit 16xPWM-16bit
* ICDI JTAG Drivers - http://www.ti.com/tool/stellaris_icdi_drivers

Install ICDI JTAG in Windows 8 (Stellaris / Tiva)
-------------------------------------------------
* Run Windows cmd.exe as administrator
* bcdedit -set loadoptions DISABLE_INTEGRITY_CHECKS
* bcdedit -set TESTSIGNING ON
* shutdown /r
* Install ICDI without problems

Drone using STM32VLDISCOVERY 
----------------------------
* ARM Cortex-M3 8k-RAM 128k-FLASH

Drone using FRDM-KL25Z 
----------------------
* ARM Cortex-M0 16k-RAM 128k-FLASH

GY-GPS6MV1 (GPS)
----------------
* Ublox NEO-6M

GY-273 (Magnetometer)
---------------------
* HMC5883L

GY-65 (Barometer)
-----------------
* BMP085

GY-521 (Accelerometer and Gyroscope)
------------------------------------
* MPU6050
* 1k FIFO
* 6 x ADC 16 bits
* "Digital Motion Processor" (DMP)
* 2k2 i2c pull-ups resistors [please not install more i2c devices in bus]


workspace-fritzing (Hardware projects)
======================================

Quadcopter
----------
* Frame Turnigy Talon V1
* Motors A2212/13T
* ESCs Hobbywing Fly Fun 30A
* Propellers 10 x 4.5

workspace-visual-studio (Windows projects)
==========================================

Ardrone pilot
-------------
* Pilot ardrone with opensource firmware using a xbox controller 

