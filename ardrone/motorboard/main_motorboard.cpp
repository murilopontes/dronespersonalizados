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
	system("sysctl -w kernel.panic=0");
	system("sysctl -w kernel.panic_on_oops=0");

	printf("build %s %s\r\n",__DATE__,__TIME__);
	printf("|-----------------------|\r\n");
	printf("|  Q = quit             |\r\n");
	printf("|  <space> = min speed  |\r\n");
    printf("|                       |\r\n");
	printf("|     ^          ^      |\r\n");
	printf("|     E          I      |\r\n");
	printf("| < S   D >  < J   K >  |\r\n");
	printf("|     X          M      |\r\n");
	printf("|    \\_/        \\_/     |\r\n");
	printf("|-----------------------|\r\n");

	mot_Init();

    double height_speed=0;
    double pitch_speed=0;
    double roll_speed=0;
    double yaw_speed=0;
    double motor[4]={0,0,0,0};

	//main loop
	while(1) {
		//handle user input
		int c=tolower(util_getch());

		if(c=='e') pitch_speed+=0.01;
		if(c=='x') pitch_speed-=0.01;

		if(c=='s') roll_speed-=0.01;
		if(c=='d') roll_speed+=0.01;

		if(c=='j') yaw_speed-=0.01;
		if(c=='k') yaw_speed+=0.01;

		if(c=='i') height_speed+=0.01;
		if(c=='m') height_speed-=0.01;

		if(c==' '){
			height_speed=0.10;
			pitch_speed=0.0;
			roll_speed=0.0;
			yaw_speed=0.0;
		}

		motor[0] = height_speed -pitch_speed + roll_speed - yaw_speed;
		motor[1] = height_speed -pitch_speed - roll_speed + yaw_speed;
		motor[2] = height_speed +pitch_speed - roll_speed - yaw_speed;
		motor[3] = height_speed +pitch_speed + roll_speed + yaw_speed;

		mot_Run(motor[0],motor[1],motor[2],motor[3]);

		if(c=='q'){
			mot_Stop();
			break;
		}

		//yield to other threads
		pthread_yield();
	}

	//
	mot_Close();
	return 0;
}
