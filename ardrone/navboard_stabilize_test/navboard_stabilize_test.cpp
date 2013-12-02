


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
#include "../open_ardrone_v1_lib/mot.h"
#include "../open_ardrone_v1_lib/singleton.h"
#include "../open_ardrone_v1_lib/daemonize.h"


typedef struct {
  int fd;
} navboard_context_t;

int between(double value,double x, double y){
  if( value >= x && value <= y) return 1;
  else return 0;
}

double clamp(double value,double min, double max){
	if(value<min) return min;
	if(value>max) return max;
	return value;
}

double make_stable(double target,double current){

	// distance = final - initial
		double height_distance = target-current;
		double adj_h=0;
		//Fuzzy controller
		double f1=3*1;
		double f2=3*2;
		double f3=3*3;
		double f4=3*4;
		double f5=3*5;
		double f6=3*6;
		//big (sky to target)
		if( height_distance < -f5 ) adj_h-=0.03;
		//medium (sky to target)
		if( between(height_distance,-f6,-f3)) adj_h-=0.02;
		//few (sky to target)
		if( between(height_distance,-f4,-f1)) adj_h-=0.01;
		//stable area
		if( between(height_distance,-f2,f2)) adj_h+=0.0;
		//few (ground to target)
		if( between(height_distance,f1,f4)) adj_h+=0.01;
		//medium (ground to target)
		if( between(height_distance,f3,f6)) adj_h+=0.02;
		//big (ground to target)
		if( height_distance>f5) adj_h+=0.03;

	return adj_h;
}


int navboard_context_init(navboard_context_t* context){

	context->fd = open("/dev/ttyPA2", O_RDWR | O_NOCTTY | O_NDELAY);
	if (context->fd == -1)
	{
		perror("nav_Init: Unable to open /dev/ttyPA2 - ");
		return -1;
	}
	//read calls are non blocking
	fcntl(context->fd, F_SETFL, FNDELAY);

	//set port options
	struct termios options;
	//Get the current options for the port
	tcgetattr(context->fd, &options);
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
	tcsetattr(context->fd, TCSANOW, &options);

	//set /MCLR pin -> reset navboard
	gpio_set(132,1);

	//start acquisition
	uint8_t cmd=0x01;
	write(context->fd,&cmd,1);


	udp_struct udp_vertical;
	udpClient_Init(&udp_vertical,8080);


	uint8_t buffer[128];

	double height=0;
	double speed=0;
	double motor[4]={0,0,0,0};

	mot_Init();

	for(;;){

		int count_ok=0;
		int count_rest=52;

		do {
		 //
	     int n = read(context->fd, &buffer[count_ok], count_rest);
	     if(n<0) continue;
	     count_ok+=n;
	     count_rest-=n;
		} while(count_ok<52);

       ///////////////////////////////////////////////////////////
		uint16_t *packet = (uint16_t*)&buffer;

		double ax=(packet[2] - 2068.0)/100.0;
		double ay=(packet[3] - 1976.0)/100.0;
		double az=(packet[4] - 2052.0)/100.0;
		//printf("ax=%.2f ay=%.2f az=%.2f ",ax,ay,az);
		
        double radian2degree = 57.295779513082320876798154814105f;
		double apitch =  -atan2(ax,az) * radian2degree;
		double aroll  =   atan2(ay,az) * radian2degree;
		double ayaw=0;
		

		//double gx=(packet[5] - 1664.0)/1.0;
		//double gy=(packet[6] - 1669.0)/1.0;
		//double gz=(packet[7] - 1663.0)/1.0;
		
		if(packet[17] < (400*37.5f)){
		   height=packet[17] / 37.5f;
		}

		double speed_min=0.10;
		double speed_max=0.99;

		///////////////////////////////////////////////////////////
		double adj_pitch  = make_stable(0,apitch);
		double adj_roll   = make_stable(0,aroll);
		double adj_yaw    = make_stable(0,ayaw);
		double adj_h      = make_stable(30,height);

		//
		speed+=adj_h;
		speed=clamp(speed,speed_min,speed_max);
		motor[0] = clamp(speed +adj_roll -adj_pitch +adj_yaw,speed_min,speed_max);
		motor[1] = clamp(speed -adj_roll -adj_pitch -adj_yaw,speed_min,speed_max);
		motor[2] = clamp(speed -adj_roll +adj_pitch +adj_yaw,speed_min,speed_max);
		motor[3] = clamp(speed +adj_roll +adj_pitch -adj_yaw,speed_min,speed_max);

		mot_Run(motor[0],motor[1],motor[2],motor[3]);

		printf("m=|%.2f|%.2f|%.2f|%.2f|\r\n",motor[0],motor[1],motor[2],motor[3]);




		//////////////////////////////////////////////////////////
		//printf("pitch=%.2f roll=%.2f h=%.2f\r\n",apitch,aroll,height);
		


	    //udpClient_Send(&udp_vertical, &buffer, 52);

	}

	//init nav structure
    //nav->ts = util_timestamp();
    //nav->dt = 0;
}









int main(int argc, char** argv)
{

	//daemonize();

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



