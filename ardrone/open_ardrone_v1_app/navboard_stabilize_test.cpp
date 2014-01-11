


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
#include "../open_ardrone_v1_lib/PID_v1.h"


void make_stable(double kp,double ki, double kd){

	//////////////////////////////////////////////////////////////
	int fd = open("/dev/ttyPA2", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		perror("nav_Init: Unable to open /dev/ttyPA2 - ");
		return;
	}
	//////////////////////////////////////////////////////////////
	//read calls are non blocking
	fcntl(fd, F_SETFL, FNDELAY);
	/////////////////////////////////////////////////////////////
	//set port options
	struct termios options;
	//Get the current options for the port
	tcgetattr(fd, &options);
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
	tcsetattr(fd, TCSANOW, &options);
	/////////////////////////////////////////////////////////////
	//set /MCLR pin -> reset navboard
	gpio_set(132,1);
	////////////////////////////////////////////////////////////
	//start acquisition
	uint8_t cmd=0x01;
	write(fd,&cmd,1);
	////////////////////////////////////////////////////////////
	udp_struct udp_vertical;
	udpClient_Init(&udp_vertical,8080);
	/////////////////////////////////////////////////////////////
	double motor[4]={0,0,0,0};
	mot_Init();
	///////////////////////////////////////////////////////////////////////////
	/*
	previous_error = 0
	integral = 0
	start:
	  error = setpoint - measured_value
	  integral = integral + error*dt
	  derivative = (error - previous_error)/dt
	  output = Kp*error + Ki*integral + Kd*derivative
	  previous_error = error
	  wait(dt)
	  goto start
	*/
	//
	double PID_H_Setpoint, PID_H_Input, PID_H_Output;
	double PID_PITCH_Setpoint,PID_PITCH_Input,PID_PITCH_Output;
	double PID_ROLL_Setpoint,PID_ROLL_Input,PID_ROLL_Output;
	//
	PID_H_Input = 0;
	PID_H_Setpoint = 30;
    PID_PITCH_Input=0;
    PID_PITCH_Setpoint=0;
    PID_ROLL_Input=0;
    PID_ROLL_Setpoint=0;
    //
	PID     PID_H_Control(&PID_H_Input    ,&PID_H_Output    ,&PID_H_Setpoint    ,6.4,0.025,24,DIRECT);
    PID PID_PITCH_Control(&PID_PITCH_Input,&PID_PITCH_Output,&PID_PITCH_Setpoint,3.3,0.030,23,DIRECT);
    PID  PID_ROLL_Control(&PID_ROLL_Input ,&PID_ROLL_Output ,&PID_ROLL_Setpoint ,3.3,0.030,23,DIRECT);
    //
	PID_H_Control.SetSampleTime(100);
    PID_PITCH_Control.SetSampleTime(100);
    PID_ROLL_Control.SetSampleTime(100);
    //
	PID_H_Control.SetOutputLimits(0.10,0.60);
    PID_PITCH_Control.SetOutputLimits(0.00,0.02);
    PID_ROLL_Control.SetOutputLimits(0.00,0.02);
    //
	PID_H_Control.SetMode(AUTOMATIC);
    PID_PITCH_Control.SetMode(AUTOMATIC);
    PID_ROLL_Control.SetMode(AUTOMATIC);
    /////////////////////////////////////////////////////////////////////////////////////////////////

	uint8_t buffer[128];
	double height=0;
	for(;;){
		////////////////////////////////////////////////////////////
		int count_ok=0;
		int count_rest=52;
		do {
		 //
	     int n = read(fd, &buffer[count_ok], count_rest);
	     if(n<0) continue;
	     count_ok+=n;
	     count_rest-=n;
		} while(count_ok<52);
       ///////////////////////////////////////////////////////////
		uint16_t *packet = (uint16_t*)&buffer;
		/////////////////////////////////////////////////////////
		if(packet[17] < (400*37.5f)){
		   height=packet[17] / 37.5f;
		}
		/////////////////////////////////////////////////////////
		double ax=(packet[2] - 2068.0)/100.0;
		double ay=(packet[3] - 1976.0)/100.0;
		double az=(packet[4] - 2052.0)/100.0;
		//printf("ax=%.2f ay=%.2f az=%.2f ",ax,ay,az);
        double radian2degree = 57.295779513082320876798154814105f;
		double apitch =  -atan2(ax,az) * radian2degree;
		double aroll  =   atan2(ay,az) * radian2degree;
		///////////////////////////////////////////////////////////

 	    PID_H_Input = height;
		PID_H_Control.Compute();

		PID_PITCH_Input=apitch;
		PID_PITCH_Control.Compute();

		PID_ROLL_Input=aroll;
		PID_ROLL_Control.Compute();

		/////////////////////////////////////////////////////////////

		double adj_yaw=0;

		motor[0] = PID_H_Output  +PID_ROLL_Output -PID_PITCH_Output -adj_yaw;
		motor[1] = PID_H_Output  -PID_ROLL_Output -PID_PITCH_Output +adj_yaw;
		motor[2] = PID_H_Output  -PID_ROLL_Output +PID_PITCH_Output -adj_yaw;
		motor[3] = PID_H_Output  +PID_ROLL_Output +PID_PITCH_Output +adj_yaw;

		mot_Run(motor[0],motor[1],motor[2],motor[3]);

		///////////////////////////////////////////////////////////////

		printf("h=%.2f(adj=%.2f) pitch=%.2f(adj=%.2f) roll=%.2f(adj=%.2f)  m=|%.2f|%.2f|%.2f|%.2f|\r\n",
				height,PID_H_Output,
				apitch,PID_PITCH_Output,
				aroll,PID_ROLL_Output,
				motor[0],motor[1],motor[2],motor[3]);




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

	//
	//daemonize();

	//
	singleton(argv[0]);

	//
	system("killall -9 program.elf");

	make_stable(0,0,0);





	return 0;
}



