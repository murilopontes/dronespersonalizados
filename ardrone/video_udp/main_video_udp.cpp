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
#include <stdint.h>
#include <sys/types.h>
#include "../open_ardrone_v1_lib/video.h"
#include "../open_ardrone_v1_lib/udp.h"


#define YUV2R(y, u, v) ({ \
		int r = (y) + ((((v) - 128) * 1436) >> 10); r > 255 ? 255 : r < 0 ? 0 : r; })
#define YUV2G(y, u, v) ({ \
		int g = (y) - ((((u) - 128) * 352 + ((v) - 128) * 731) >> 10); g > 255 ? 255 : g < 0 ? 0 : g; })
#define YUV2B(y, u, v) ({ \
		int b = (y) + ((((u) - 128) * 1814) >> 10); b > 255 ? 255 : b < 0 ? 0 : b; })

#define CLIP(color) (unsigned char)(((color) > 0xFF) ? 0xff : (((color) < 0) ? 0 : (color)))



void v4lconvert_yuv420_to_rgb24(const unsigned char *src, unsigned char *dest,
		int width, int height, int yvu)
{
	int i, j;

	const unsigned char *ysrc = src;
	const unsigned char *usrc, *vsrc;

	if (yvu) {
		vsrc = src + width * height;
		usrc = vsrc + (width * height) / 4;
	} else {
		usrc = src + width * height;
		vsrc = usrc + (width * height) / 4;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j += 2) {
#if 0 /* fast slightly less accurate multiplication free code */
			int u1 = (((*usrc - 128) << 7) +  (*usrc - 128)) >> 6;
			int rg = (((*usrc - 128) << 1) +  (*usrc - 128) +
					((*vsrc - 128) << 2) + ((*vsrc - 128) << 1)) >> 3;
			int v1 = (((*vsrc - 128) << 1) +  (*vsrc - 128)) >> 1;

			*dest++ = CLIP(*ysrc + v1);
			*dest++ = CLIP(*ysrc - rg);
			*dest++ = CLIP(*ysrc + u1);
			ysrc++;

			*dest++ = CLIP(*ysrc + v1);
			*dest++ = CLIP(*ysrc - rg);
			*dest++ = CLIP(*ysrc + u1);
#else
			*dest++ = YUV2R(*ysrc, *usrc, *vsrc);
			*dest++ = YUV2G(*ysrc, *usrc, *vsrc);
			*dest++ = YUV2B(*ysrc, *usrc, *vsrc);
			ysrc++;

			*dest++ = YUV2R(*ysrc, *usrc, *vsrc);
			*dest++ = YUV2G(*ysrc, *usrc, *vsrc);
			*dest++ = YUV2B(*ysrc, *usrc, *vsrc);
#endif
			ysrc++;
			usrc++;
			vsrc++;
		}
		/* Rewind u and v for next line */
		if (!(i&1)) {
			usrc -= width / 2;
			vsrc -= width / 2;
		}
	}
}

int main(int argc,char ** argv)
{
	/*
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
*/

	udp_struct udp_vertical;
	udpClient_Init(&udp_vertical,6969);

	vid_struct vid_vertical;
	vid_vertical.device = (char*)"/dev/video1";
	vid_vertical.w=176;
	vid_vertical.h=144;
	vid_vertical.n_buffers = 4;
	video_Init(&vid_vertical);

	img_struct* img_vertial = video_CreateImage(&vid_vertical);

	int len_rgb=176*144*3;
	unsigned char* rgbbuf = (unsigned char*) malloc(len_rgb);

	int i=0;
    for (;;) {
    	i++;

		//unsigned int len_vertical=(vid_vertical.w*vid_vertical.h)/2;
		//unsigned int len_horizontal=(vid_horizontal.w*vid_horizontal.h)/2;

    	printf("send frame=%d len_v=%d len_h=%d\r\n",i,len_rgb,0);//len_horizontal);

		video_GrabImage(&vid_vertical, img_vertial);

		v4lconvert_yuv420_to_rgb24(img_vertial->buf,rgbbuf,176,144,1);

		udpClient_Send(&udp_vertical, rgbbuf, 65000);

		/*
		video_GrabImage(&vid_horizontal, img_horizontal);
		udpClient_Send(&udp_horizontal1, img_horizontal->buf, len_horizontal);
*/

    }

	video_Close(&vid_vertical);
	//video_Close(&vid_horizontal);

	
    return 0;
}
