

//posix API
#include <unistd.h>

//posix threads
#include <pthread.h>

//file control options
#include <fcntl.h>

//terminal I/O interfaces
#include <termios.h>

#include "../open_ardrone_v1_lib/all_headers_cpp.h"
#include "../open_ardrone_v1_lib/gpio.h"
#include "../open_ardrone_v1_lib/udp.h"
#include "../open_ardrone_v1_lib/singleton.h"
#include "../open_ardrone_v1_lib/daemonize.h"

typedef struct {
  int fd;
} navboard_context_t;

int navboard_context_init(navboard_context_t* ctx){

	ctx->fd = open("/dev/ttyPA2", O_RDWR | O_NOCTTY | O_NDELAY);
	if (ctx->fd == -1)
	{
		perror("nav_Init: Unable to open /dev/ttyPA2 - ");
		return -1;
	}
	//read calls are non blocking
	fcntl(ctx->fd, F_SETFL, FNDELAY);

	//set port options
	struct termios options;
	//Get the current options for the port
	tcgetattr(ctx->fd, &options);
	//Set the baud rates to 460800
	cfsetispeed(&options, B460800);
	cfsetospeed(&options, B460800);

	//Enable the receiver and set local mode
	options.c_cflag |= (CLOCAL | CREAD);
	//clear input options
	options.c_iflag = 0;
	//clear local options
	options.c_lflag=0;
	//clear output options (raw output)
	options.c_oflag &= ~OPOST;
	//Set the new options for the port
	tcsetattr(ctx->fd, TCSANOW, &options);

	//set /MCLR pin -> reset navboard
	gpio_set(132,1);

	//start acquisition
	uint8_t cmd=0x01;
	write(ctx->fd,&cmd,1);


	udp_struct udp_vertical;
	udpClient_Init(&udp_vertical,8080);


	uint8_t buffer[128];


	for(;;){

		int count_ok=0;
		int count_rest=52;

		do {
		 //
	     int n = read(ctx->fd, &buffer[count_ok], count_rest);
	     if(n<0) continue;
	     count_ok+=n;
	     count_rest-=n;
		} while(count_ok<52);


/*
		int16_t *px = &buffer[0];
	  for(i=0;i<26;i++){
		  printf("%d|",px[i]);
	  }
	  printf("\r\n");
*/

	    udpClient_Send(&udp_vertical, (char*)&buffer, 52);

	}

	//init nav structure
    //nav->ts = util_timestamp();
    //nav->dt = 0;
}




int main(int argc, char** argv)
{

	daemonize();

	//
	singleton(argv[0]);

	//
	system("killall -9 program.elf");

	//
	int ret;
	navboard_context_t ctx;
	
	//
	ret=navboard_context_init(&ctx);

	//
	if(ret < 0) return ret;



	return 0;
}



