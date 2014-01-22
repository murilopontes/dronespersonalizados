
Open source implementation of Ardrone V1
===========================================



Drone Connect (Windows application)
-----------------------------------

# Install Visual Studio Express 2013 for Windows Desktop
http://www.visualstudio.com/downloads/download-visual-studio-vs

# Install SlimDX via NuGet in Visual Studio

# Install JSon.Net  via NuGet in Visual Studio



Drone Firmware
--------------------------------------

# install Sourcery G++ Lite 2009q1-203

https://sourcery.mentor.com/GNUToolchain/release858?lite=arm
https://sourcery.mentor.com/GNUToolchain/package4574/public/arm-none-linux-gnueabi/arm-2009q1-203-arm-none-linux-gnueabi.exe


Boost 1.55 -> gcc crash
Boost 1.49 -> ok

# Boost compile guide

1) install GCC 
arm-2009q1-203-arm-none-linux-gnueabi.exe in c:/arm-gcc

2) create boost user config
~/user-config.jam
using gcc : arm : "C:/arm-gcc/bin/arm-none-linux-gnueabi-g++" ;

3) extract, bootstrap and build boost
bootstrap
b2 toolset=gcc-arm target-os=linux link=static threadapi=pthread -j4
