/*
    main_video.h - video driver demo program

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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "../open_ardrone_v1_lib/video.h"
#include "../open_ardrone_v1_lib/udp.h"


int main(int argc,char ** argv)
{
	udp_struct udp_horizontal1;
	//udp_struct udp_horizontal2;
	//udp_struct udp_horizontal3;
	udpClient_Init(&udp_horizontal1,"192.168.1.2",7070);
	//udpClient_Init(&udp_horizontal2,"192.168.1.2",7071);
	//udpClient_Init(&udp_horizontal3,"192.168.1.2",7072);

	vid_struct vid_horizontal;
	vid_horizontal.device = (char*)"/dev/video0";
	vid_horizontal.w=640;
	vid_horizontal.h=480;
	vid_horizontal.n_buffers = 4;
	video_Init(&vid_horizontal);

	img_struct* img_horizontal = video_CreateImage(&vid_horizontal);


	udp_struct udp_vertical;
	udpClient_Init(&udp_vertical,"192.168.1.2",6969);

	vid_struct vid_vertical;
	vid_vertical.device = (char*)"/dev/video1";
	vid_vertical.w=176;
	vid_vertical.h=144;
	vid_vertical.n_buffers = 4;
	video_Init(&vid_vertical);

	img_struct* img_vertial = video_CreateImage(&vid_vertical);

	int i=0;
    for (;;) {
    	i++;

		int len_vertical=(176*144)/2;
		int len_horizontal=(640*480)/2;

    	printf("send frame=%d len_v=%d len_h=%d\r\n",i,len_vertical,len_horizontal);

		video_GrabImage(&vid_vertical, img_vertial);
		udpClient_Send(&udp_vertical, img_vertial->buf, len_vertical);

		video_GrabImage(&vid_horizontal, img_horizontal);
		udpClient_Send(&udp_horizontal1, img_horizontal->buf, len_horizontal);
		//udpClient_Send(&udp_horizontal2, img_horizontal->buf+51200, len_horizontal/3);
		//udpClient_Send(&udp_horizontal3, img_horizontal->buf+102400, len_horizontal/3);


    }

	video_Close(&vid_vertical);
	video_Close(&vid_horizontal);

	
    return 0;
}
