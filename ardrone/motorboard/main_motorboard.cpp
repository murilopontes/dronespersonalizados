/*
    main_motorboard.c - AR.Drone motor demo program

    Copyright (C) 2011 Hugo Perquin - http://blog.perquin.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
 */







#include "../open_ardrone_v1_lib/all_headers_cpp.h"
#include "../open_ardrone_v1_lib/type.h"
#include "../open_ardrone_v1_lib/util.h"
#include "../open_ardrone_v1_lib/mot.h"
#include "../open_ardrone_v1_lib/singleton.h"


int main(int argc, char** argv)
{
	//
	singleton(argv[0]);

	//
	system("killall -9 program.elf");

	printf("q=quit\r\n");

	printf("w=pitch front\r\n");
	printf("e=pitch back\r\n");

	printf("r=roll left\r\n");
	printf("t=roll right\r\n");

	printf("y=yaw cw\r\n");
	printf("u=yaw ccw\r\n");

	printf("<space>=stop all\r\n");
	printf(",=speed up all\r\n");
	printf(".=speed down all\r\n");

	printf("1=motor1 min\r\n");
	printf("2=motor2 min\r\n");
	printf("3=motor3 min\r\n");
	printf("4=motor4 min\r\n");
	printf("5=motor all min \r\n");

	printf("Leds:  a=loff\r\n");
	printf("Leds:  s=green\r\n");
	printf("Leds:  d=orange\r\n");
	printf("Leds:  f=red\r\n");

	mot_Init();


	float throttle1 = 0;
	float throttle2 = 0;
	float throttle3 = 0;
	float throttle4 = 0;

	//0.01-0.02 random motor shutdown
	//
	float step=0.03;
    float min=0.03;

	//main loop
	while(1) {
		//handle user input
		int c=tolower(util_getch());

		if(c=='w'){
			printf("go front\r\n");
			throttle1 = min; 	throttle2 = min;
			throttle4 +=step;  throttle3 +=step;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
			printf("Throttle %f %f %f %f \r\n",throttle1,throttle2,throttle3,throttle4);
		}
		if(c=='e'){
			printf("go back\r\n");
			throttle1 +=step; 	throttle2 +=step;
			throttle4 =min;  throttle3 =min;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
			printf("Throttle %f %f %f %f \r\n",throttle1,throttle2,throttle3,throttle4);
		}

		if(c=='r'){
			printf("go left\r\n");
			throttle1 +=step;  throttle2 =min;
			throttle4 +=step;  throttle3 =min;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
			printf("Throttle %f %f %f %f \r\n",throttle1,throttle2,throttle3,throttle4);
		}
		if(c=='t'){
			printf("go right\r\n");
			throttle1 =min; 	throttle2 +=step;
			throttle4 =min;  throttle3 +=step;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
			printf("Throttle %f %f %f %f \r\n",throttle1,throttle2,throttle3,throttle4);
		}

		if(c=='y'){
			printf("go cw\r\n");
			throttle1 =min;  throttle2 +=step;
			throttle4 +=step;  throttle3 =min;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
			printf("Throttle %f %f %f %f \r\n",throttle1,throttle2,throttle3,throttle4);
		}
		if(c=='u'){
			printf("go ccw\r\n");
			throttle1 +=step; 	throttle2 =min;
			throttle4 =min;  throttle3 +=step;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
			printf("Throttle %f %f %f %f \r\n",throttle1,throttle2,throttle3,throttle4);
		}


		if(c=='q'){
			mot_Stop();
			break;
		}

		if(c=='1') {
			printf("Run Motor1 \r\n");
			throttle1 = step;
			throttle2 = 0;
			throttle3 = 0;
			throttle4 = 0;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
		}
		if(c=='2') {
			printf("Run Motor2 \r\n");
			throttle1 = 0;
			throttle2 = step;
			throttle3 = 0;
			throttle4 = 0;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
		}
		if(c=='3') {
			printf("Run Motor3 \r\n");
			throttle1 = 0;
			throttle2 = 0;
			throttle3 = step;
			throttle4 = 0;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
		}
		if(c=='4') {
			printf("Run Motor4 \r\n");
			throttle1 = 0;
			throttle2 = 0;
			throttle3 = 0;
			throttle4 = step;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
		}
		if(c=='5') {
			printf("Run All Motors\r\n");
			throttle1 = step;
			throttle2 = step;
			throttle3 = step;
			throttle4 = step;
			mot_Run(throttle1,throttle2,throttle3,throttle4);
		}
		if(c==',') {
			throttle1 -= step;
			throttle2 -= step;
			throttle3 -= step;
			throttle4 -= step;
			printf("Throttle down %f %f %f %f \r\n",throttle1,throttle2,throttle3,throttle4);
			mot_Run(throttle1,throttle2,throttle3,throttle4);
		}
		if(c=='.') {
			throttle1 += step;
			throttle2 += step;
			throttle3 += step;
			throttle4 += step;
			printf("Throttle up %f %f %f %f \r\n",throttle1,throttle2,throttle3,throttle4);
			mot_Run(throttle1,throttle2,throttle3,throttle4);
		}
		if(c==' ') {
			printf("Stop\r\n");
			throttle1 = 0;
			throttle2 = 0;
			throttle3 = 0;
			throttle4 = 0;
			mot_Stop();
		}
		if(c=='a') {
			printf("Leds off\r\n");
			mot_SetLeds(MOT_LEDOFF,MOT_LEDOFF,MOT_LEDOFF,MOT_LEDOFF);
		}
		if(c=='s') {
			printf("Leds green\r\n");
			mot_SetLeds(MOT_LEDGREEN,MOT_LEDGREEN,MOT_LEDGREEN,MOT_LEDGREEN);
		}
		if(c=='d') {
			printf("Leds orange\r\n");
			mot_SetLeds(MOT_LEDORANGE,MOT_LEDORANGE,MOT_LEDORANGE,MOT_LEDORANGE);
		}
		if(c=='f') {
			printf("Leds red\r\n");
			mot_SetLeds(MOT_LEDRED,MOT_LEDRED,MOT_LEDRED,MOT_LEDRED);
		}

		//yield to other threads
		pthread_yield();
	}

	//
	mot_Close();
	return 0;
}
