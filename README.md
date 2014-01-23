
Open source implementation of Ardrone V1
===========================================
-Drone Connect status:
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

-Firmware status:
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




Drone Connect (Windows application)
-----------------------------------

1) Install Visual Studio Express 2013 for Windows Desktop
http://www.visualstudio.com/downloads/download-visual-studio-vs

2) Open solution (pc\openfight-apps.sln), select DroneConnect project as startup project

3) Install SlimDX via NuGet in Visual Studio

4) Install JSon.Net  via NuGet in Visual Studio

5) Build entire solution

6) Run DroneConnect 


Drone Firmware
--------------------------------------

1) Install Visual Studio Express 2013 for Windows Desktop
http://www.visualstudio.com/downloads/download-visual-studio-vs

2) install  Sourcery G++ Lite 2009q1-203 in c:/arm-gcc
https://sourcery.mentor.com/GNUToolchain/release858?lite=arm
https://sourcery.mentor.com/GNUToolchain/package4574/public/arm-none-linux-gnueabi/arm-2009q1-203-arm-none-linux-gnueabi.exe

3) create boost user config
~/user-config.jam
using gcc : arm : "C:/arm-gcc/bin/arm-none-linux-gnueabi-g++" ;

4) extract boost-x.xx.7z in c:\

5) launch "Developer Command Prompt for VS2013"

6) cd c:\boost-x.xx

7) bootstrap boost build system
bootstrap

8) build all required boost libraries
b2 toolset=gcc-arm target-os=linux link=static threadapi=pthread -j8

Boost build summary
-----------------------------------------------------------------

* Boost 1.55 fail, but needed features for project still working

Boost 1.55 -> arm-2009q1-203-arm-none-linux-gnueabi crash

```sh
...failed updating 13 targets...
...skipped 9 targets...
...updated 588 targets...
```

Boost 1.54 -> arm-2009q1-203-arm-none-linux-gnueabi crash
> ...failed updating 12 targets...
> ...skipped 9 targets...
> ...updated 592 targets...

Boost 1.53 -> arm-2009q1-203-arm-none-linux-gnueabi crash
> ...failed updating 2 targets...
> ...skipped 3 targets...
> ...updated 540 targets...

Boost 1.52 -> arm-2009q1-203-arm-none-linux-gnueabi OK!
> ...updated 527 targets...
> The Boost C++ Libraries were successfully built!
> The following directory should be added to compiler include paths:
>     C:/boost_1_52_0
> The following directory should be added to linker library paths:
 >    C:\boost_1_52_0\stage\lib



