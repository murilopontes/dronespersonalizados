

#include "../open_ardrone_v1_lib/all_headers_cpp.h"
#include "../open_ardrone_v1_lib/video.h"
#include "../open_ardrone_v1_lib/singleton.h"


int main(int argc,char ** argv)
{
	//
	singleton(argv[0]);

	//
	system("killall -9 program.elf");

	//////////////////////////////////////////////////////////////////////////////

	vid_struct camera_horizontal;
	camera_horizontal.device = (char*)"/dev/video0";
	camera_horizontal.w=640;
	camera_horizontal.h=480;
	camera_horizontal.n_buffers = 4;
	video_Init(&camera_horizontal);

	img_struct* img_horizontal = video_CreateImage(&camera_horizontal);
	video_GrabImage(&camera_horizontal, img_horizontal);

	FILE *write_ptr;
	write_ptr = fopen("camera-horizontal.yuv","wb");  // w for write, b for binary
	fwrite(img_horizontal->buf,camera_horizontal.w*camera_horizontal.h,1,write_ptr);
	fclose(write_ptr);

	video_Close(&camera_horizontal);

	/////////////////////////////////////////////////////////////////////////

	vid_struct camera_vertical;
	camera_vertical.device = (char*)"/dev/video1";
	camera_vertical.w=176;
	camera_vertical.h=144;
	camera_vertical.n_buffers = 4;
	video_Init(&camera_vertical);

	img_struct* img_vertical = video_CreateImage(&camera_vertical);
	video_GrabImage(&camera_vertical, img_vertical);

	//FILE *write_ptr;
	write_ptr = fopen("camera-vertical.yuv","wb");  // w for write, b for binary
	fwrite(img_vertical->buf,camera_vertical.w*camera_vertical.h,1,write_ptr);
	fclose(write_ptr);


	video_Close(&camera_vertical);
	

	printf("The end!\r\n");
    return 0;
}
