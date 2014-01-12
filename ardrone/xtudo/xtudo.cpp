
#include "../open_ardrone_v1_lib/all_headers_cpp.h"
#include "../open_ardrone_v1_lib/vbat.h"
#include "../open_ardrone_v1_lib/udp.h"
#include "../open_ardrone_v1_lib/singleton.h"
#include "../open_ardrone_v1_lib/daemonize.h"


void* vbat_task(void* arg){
	for(;;){
		system("/data/video/vbat_udp");
	}
	return NULL;
}

void* video_horizontal_task(void* arg){
	for(;;){
		system("/data/video/video_tool -d /dev/video0 -c 1");
		usleep(500);
		//system("/data/video/udp_broadcast_file 7070 /tmp/horizontal.yuv");
		system("ftpput -v 192.168.1.2 horizontal.yuv  /tmp/horizontal.yuv");
		usleep(500);
		system("/data/video/video_tool -d /dev/video1 -c 1");
		usleep(500);
		//system("/data/video/udp_broadcast_file 6969 /tmp/vertical.yuv");
		system("ftpput -v 192.168.1.2 vertical.yuv  /tmp/vertical.yuv");
		usleep(500);
	}
	return NULL;
}

/*
void* video_vertical_task(void* arg){
	for(;;){

	}
	return NULL;
}
*/

int main(int argc, char *argv[]) {


	singleton(argv[0]);

	system("sync");
	system("killall -9 program.elf");
	system("killall -9 vbat_udp");

	daemonize();


	pthread_t thread_vbat;
	pthread_t thread_video_h;
	pthread_t thread_video_v;

	pthread_create(&thread_vbat, NULL,vbat_task, NULL);
	pthread_create(&thread_video_h, NULL,video_horizontal_task, NULL);
	//pthread_create(&thread_video_v, NULL,video_vertical_task, NULL);

	pthread_join(thread_vbat,NULL);

	return 0;
}
