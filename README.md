Open source implementation of Ardrone V1
===========================================

This project not modify any file of official firmware.

Should be installed alongside of latest official firmware.

Tested with firmware version 1.11.5 updated using AR.Freeflight v2.4.10

Project Install
===============

The install is simple. 
Copy "drone" firmware using your favorite ftp tool to ardrone.
Open ardrone telnet, and type:
```sh
/data/video/drone
```
Now, open Drone Connect Windows aplication and use one Xbox 360 joystick to pilot the drone.

```sh
Analog Stick Left = pitch, roll
Analog Stick Right = yaw, height
A = take off
B = emergency
```

Project Status
==============

Drone Connect status
---------------------
* [working] Wifi automatic connect
* [working] Ping monitor 
* [working] Xbox 360 Joystick
* [working] Cameras snapshot: Horizontal and Vertical
* [working] Navboard RAW sensor monitors (accelerometer, gyroscope, ultrasonic)
* [working] Navboard Sensors Fusion monitors
* [working] Navboard Calibration
* [working] Voltage monitor
* [working] Motors PWM monitor
* [not working] PID auto tunning

Firmware status
----------------
* [working] http server - static page and cameras
* [working] Cameras: Horizontal and vertical
* [not working] H264 video compress hardware (/dev/p264)
* [not working] http joystick/control page 
* [working] voltage monitor sensor
* [working] voltage monitor json server
* [working] joystick json receive server
* [working] Motors PWM terminal
* [working] Motors PWM monitor json server
* [working] Navboard RAW (accelerometer, gyroscope, ultrasonic) 
* [working] Navboard Sensors Fusion
* [working] Navboard Calibration json server
* [working] Direct Drive / Pilot Motors via Telnet
* [working] Direct Drive / Pilot Motors via Joystick json server
* [not working] Stabilized pilot with PID controllers via Joystick json server


Project Build Instructions
==========================

Drone Connect (Windows application)
-----------------------------------

* Install Visual Studio Express 2013 for Windows Desktop

http://www.visualstudio.com/downloads/download-visual-studio-vs

* Open solution (pc\openfight-apps.sln), select DroneConnect project as startup project
* Install SlimDX via NuGet in Visual Studio
* Install SlimDX SDK January 2012 (required by Axiom 3D RenderWindow)

https://slimdx.googlecode.com/files/SlimDX%20SDK%20%28January%202012%29.msi

* Install JSon.Net  via NuGet in Visual Studio
* Install Axiom 3d Game Engine  via NuGet in Visual Studio
* Build entire solution
* Run DroneConnect 



Drone Firmware
--------------------------------------

1) Install Visual Studio Express 2013 for Windows Desktop

http://www.visualstudio.com/downloads/download-visual-studio-vs

2) install  Sourcery G++ Lite 2009q1-203 in c:/arm-gcc

https://sourcery.mentor.com/GNUToolchain/release858?lite=arm
https://sourcery.mentor.com/GNUToolchain/package4574/public/arm-none-linux-gnueabi/arm-2009q1-203-arm-none-linux-gnueabi.exe

3) create boost user config in your directory (~/user-config.jam) with follow content

using gcc : arm : "C:/arm-gcc/bin/arm-none-linux-gnueabi-g++" ;

4) extract boost-x.xx.7z in c:\

5) launch "Developer Command Prompt for VS2013"

6) cd c:\boost-x.xx

7) bootstrap boost build system

```sh
C:\boost_1_55_0>bootstrap
```

8) build all required boost libraries

```sh
C:\boost_1_55_0>b2 toolset=gcc-arm target-os=linux link=static threadapi=pthread -j8
```

9) Install JDK

10) Eclipse J2EE 

11) Eclipse CDT

12) Import Eclipse Workspace

13) Eclipse Build firmware



Boost build summary
-------------------

* Boost 1.55 fail, but needed features for project still working

Boost 1.55 -> arm-2009q1-203-arm-none-linux-gnueabi  crash 

```sh
...failed updating 13 targets...
...skipped 9 targets...
...updated 588 targets...
```

Boost 1.54 -> arm-2009q1-203-arm-none-linux-gnueabi  crash

```sh
...failed updating 12 targets...
...skipped 9 targets...
...updated 592 targets...
```

Boost 1.53 -> arm-2009q1-203-arm-none-linux-gnueabi  crash 

```sh
...failed updating 2 targets...
...skipped 3 targets...
...updated 540 targets...
```

Boost 1.52 -> arm-2009q1-203-arm-none-linux-gnueabi OK!

```sh
...updated 527 targets...
The Boost C++ Libraries were successfully built!
The following directory should be added to compiler include paths:
    C:/boost_1_52_0
The following directory should be added to linker library paths:
    C:\boost_1_52_0\stage\lib
```



