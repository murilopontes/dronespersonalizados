



//C library: http://en.cppreference.com/w/c/header
#include <assert.h> //	Conditionally compiled macro that compares its argument to zero
#include <complex.h> // (since C99)	Complex number arithmetic
#include <ctype.h> //	Functions to determine the type contained in character data
#include <errno.h> //	Macros reporting error conditions
#include <fenv.h> // (since C99)	Floating-point environment
#include <float.h> //	Limits of float types
#include <inttypes.h> // (since C99)	Format conversion of integer types
#include <iso646.h> // (since C99)	 Alternative operator spellings
#include <limits.h> //	Sizes of basic types
#include <locale.h> //	Localization utilities
#include <math.h> //	Common mathematics functions
#include <setjmp.h> //	Nonlocal jumps
#include <signal.h> //	Signal handling
//#include <stdalign.h> // (since C11)	 Type alignment control
#include <stdarg.h> //	 Variable arguments
//#include <stdatomic.h> // (since C11)	Atomic types
#include <stdbool.h> // (since C99)	Macros supporting type bool
#include <stddef.h> //	 Common macro definitions
#include <stdint.h> // (since C99)	Fixed-width integer types
#include <stdio.h> //	Input/output
#include <stdlib.h> //	 General utilities: memory management, program utilities, string conversions, random numbers
//#include <stdnoreturn.h> // (since C11)	 Non-returning functions
#include <string.h> //	String handling
#include <tgmath.h> // (since C99)	Type-generic math (macros wrapping math.h and complex.h)
//#include <threads.h> // (since C11)	Thread library
#include <time.h> //	Time/date utilities
//#include <uchar.h> // (since C11)	UTF-16 and UTF-32 character utilities
#include <wchar.h> // (since C99)	Extended multibyte and wide character utilities
#include <wctype.h> // (since C99)	Wide character classification and mapping utilities

//posix API
#include <unistd.h>

//posix threads
#include <pthread.h>

//file control options
#include <fcntl.h>

//terminal I/O interfaces
#include <termios.h>


//#include "../open_ardrone_v1_lib/type.h"
//#include "../open_ardrone_v1_lib/util.h"
#include "../open_ardrone_v1_lib/gpio.h"
#include "../open_ardrone_v1_lib/udp.h"



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




#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

static void daemonize(void)
{
    pid_t pid, sid;
    int fd;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS); /*Killing the Parent Process*/
    }

    /* At this point we are executing as the child process */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory. */
    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }


    fd = open("/dev/null",O_RDWR, 0);

    if (fd != -1)
    {
        dup2 (fd, STDIN_FILENO);
        dup2 (fd, STDOUT_FILENO);
        dup2 (fd, STDERR_FILENO);

        if (fd > 2)
        {
            close (fd);
        }
    }

    /*resettign File Creation Mask */
    umask(027);
}




//
void verify_singleton(char* program_name) {


    //strip program_name from complete path
	char* slash = strrchr(program_name, '/');
	if (slash) {
		program_name = (slash + 1);
	}

	//format pidof command
	char cmd_str[2048];
	sprintf(cmd_str, "pidof %s", program_name);
	printf("cmd_str->%s\r\n", cmd_str);

	//execute pidof command
	char cmd_output[2048];
	FILE* pidof_fd = popen(cmd_str, "r");
	fgets(cmd_output, sizeof(cmd_output), pidof_fd);
	pclose(pidof_fd);
	printf("result of pidof ->(%s)\r\n", cmd_output);

	//
	int pids = 0;
	const char s[2] = " ";
	char *token = strtok(cmd_output, s);
	while (token != NULL) {
		printf("pid[%d]=%s\r\n", pids, token);
		token = strtok(NULL, s);
		pids++;
	}
	if (pids != 1) {
		printf("exit! this program is already running\r\n");
		exit(1);
	} else {
		printf("ok! normal workflow\r\n");
	}
}

int main(int argc, char** argv)
{

	daemonize();

	//
	verify_singleton(argv[0]);

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



