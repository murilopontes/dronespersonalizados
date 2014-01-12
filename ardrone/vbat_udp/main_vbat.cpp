/*
    main_vbat.c - AR.Drone battery voltage monitor demo program

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
#include "../open_ardrone_v1_lib/vbat.h"
#include "../open_ardrone_v1_lib/udp.h"
#include "../open_ardrone_v1_lib/singleton.h"
#include "../open_ardrone_v1_lib/daemonize.h"

int main(int argc, char *argv[]) {

	singleton(argv[0]);

	system("killall -9 program.elf");

	//daemonize();

	pthread_t thread_vbat;

	pthread_create(&thread_vbat, NULL,vbat_work, NULL);

	pthread_join(thread_vbat,NULL);

	return 0;
}
