Open source implementation of Ardrone V1



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
