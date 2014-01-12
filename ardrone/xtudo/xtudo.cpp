
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

	 udp_struct udp;
	 udpClient_Init(&udp,50003);
	  int msglen;
	  char buf[512];

	  udp_struct udps;
	  udpServer_Init(&udps,50004,1);

	for(;;){

		////////////////////////////////////////////////////////////////////////
		system("/data/video/video_tool -d /dev/video0 -c 1");
		//system("/data/video/udp_broadcast_file 7070 /tmp/horizontal.yuv");
		system("ftpput -v 192.168.1.2 horizontal.yuv  /tmp/horizontal.yuv");

		///////////////////////////////////////////////////////////////////////
		system("/data/video/video_tool -d /dev/video1 -c 1");
		//system("/data/video/udp_broadcast_file 6969 /tmp/vertical.yuv");
		system("ftpput -v 192.168.1.2 vertical.yuv  /tmp/vertical.yuv");
		///////////////////////////////////////////////////////////////////////

		msglen=sprintf(buf,"read-now");
		udpClient_Send(&udp, buf, msglen);
		///////////////////////////////////////////////////////////////////////
		msglen = udpServer_Receive(&udps, buf, 512);
		//////////////////////////////////////////////////////////////////////

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
	system("sysctl -w kernel.panic=0");
    system("sysctl -w kernel.panic_on_oops=0");

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
