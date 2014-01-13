


//posix API
#include <unistd.h>

//posix threads
#include <pthread.h>

//file control options
#include <fcntl.h>

//terminal I/O interfaces
#include <termios.h>

#include "../open_ardrone_v1_lib/util.h"
#include "../open_ardrone_v1_lib/all_headers_cpp.h"
#include "../open_ardrone_v1_lib/gpio.h"
#include "../open_ardrone_v1_lib/udp.h"
#include "../open_ardrone_v1_lib/mot.h"
#include "../open_ardrone_v1_lib/singleton.h"
#include "../open_ardrone_v1_lib/daemonize.h"


void make_stable(void){

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
	udpClient_Init(&udp_vertical,50002);
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


	char udp_buffer[1000];
	uint8_t buffer[128];
	double height=0;

	double sum_pitch_calibration=0;
	double sum_roll_calibration=0;
	int calibration_qtd=200;
	///////////////////////
	double sum_ax=0;
	double sum_ay=0;
	double sum_az=0;
	////////////////////////
	double sum_gx=0;
	double sum_gy=0;
	double sum_gz=0;

	printf("navboard cabibration samples=%d wait...\n",calibration_qtd);
	for(int l=0;l<calibration_qtd;l++){
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
		double ax=(packet[2]);
		double ay=(packet[3]);
		double az=(packet[4]);
		double gx=(packet[5]);
		double gy=(packet[6]);
		double gz=(packet[7]);

	}


	printf("navboard started udp broadcast\n");


	double g_pitch = 0;
	double g_roll = 0;
	double g_yaw = 0;

	double ts_back = util_timestamp();
	double ts_now = util_timestamp();

	double fusion_pitch = 0.0;
	double fusion_roll = 0.0;

	uint32_t bad_count=0;

	double height_setpoint=30.0;
	double height_speed=0.0;

	double pitch_setpoint=0.0;
	double pitch_speed=0.0;

	double roll_setpoint=0.0;
	double roll_speed=0.0;

	double yaw_setpoint=0.0;
	double yaw_speed=0.0;

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

		/*
		for(int i=0;i<26;i++){
		printf("%04x|",packet[i]);
		}
		printf("\n");
		 */
		if(packet[0]!=0x0032){
			printf("drop bad read=%d\n",bad_count);
			bad_count++;
			continue;
		}

		/////////////////////////////////////////////////////////
		if(packet[17] /37.5f < 650 ){
			height=packet[17] / 37.5f;
		}
		/////////////////////////////////////////////////////////
		double ax=(packet[2] - 2000.0f)/100.0f;
		double ay=(packet[3] - 2000.0f)/100.0f;
		double az=(packet[4] - 2000.0f)/100.0f;
		double radian2degree = 57.295779513082320876798154814105f;
		double apitch = ( -atan2(ax,az) * radian2degree );
		double aroll  = (  atan2(ay,az) * radian2degree );
		/////////////////////////////////////////////////////////
		double gx=(packet[5] - 1666.0f)/2.0f;//6.2f;
		double gy=(packet[6] - 1666.0f)/2.0f;//6.2f;
		double gz=(packet[7] - 1666.0f)/4.0f;//6.2f;
		/////////////////////////////////////////////////////////

		ts_back = ts_now;
		ts_now  = util_timestamp();
		double dt = ts_now-ts_back;

		g_pitch+=gy*dt;
		g_roll+=gx*dt;
		g_yaw+=gz*dt;

		//
		if(g_pitch<-180)g_pitch=-180;
		if(g_pitch>180)g_pitch=180;
		//
		if(g_roll<-180)g_roll=-180;
		if(g_roll>180)g_roll=180;
		//
		if(g_yaw<-180)g_yaw=-180;
		if(g_yaw>180)g_yaw=180;

		//
		fusion_pitch= 0.98*(fusion_pitch+gy*dt)+0.02*apitch;
		fusion_roll = 0.98*(fusion_roll+gx*dt)+0.02*aroll;

		/////////////////////////////////////////////////////////



		double height_error = height_setpoint - height ;
		if(height_error>0){
			//printf("muito baixo -> accelerar -> ");
			height_speed+=0.01/10.0f;
		}
		if(height_error<0){
			//printf("muito alto -> desaccelerar -> ");
			height_speed-=0.01/10.0f;
		}
		if(height_speed<0.52) height_speed=0.52;
		if(height_speed>0.60) height_speed=0.60;

		//printf("height_error=(%.2f-%.2f)=%.2f height_speed=%.2f\n",height_setpoint,height,height_error,height_speed);

		double pitch_error = pitch_setpoint - fusion_pitch;
		if(pitch_error>0){
			pitch_speed+=0.01/10.0f;
		}
		if(pitch_error<0){
			pitch_speed-=0.01/10.0f;
		}
		if(pitch_speed<0) pitch_speed=0;
		if(pitch_speed>5) pitch_speed=5;

		double roll_error = roll_setpoint - fusion_roll;
		if(roll_error>0){
			roll_speed+=0.01/10.0f;
		}
		if(roll_error<0){
			roll_speed-=0.01/10.0f;
		}
		if(roll_speed<0) roll_speed=0;
		if(roll_speed>5) roll_speed=5;


		motor[0] = height_speed -pitch_speed + roll_speed - yaw_speed;
		motor[1] = height_speed -pitch_speed - roll_speed + yaw_speed;
		motor[2] = height_speed +pitch_speed - roll_speed - yaw_speed;
		motor[3] = height_speed +pitch_speed + roll_speed + yaw_speed;

		mot_Run(motor[0],motor[1],motor[2],motor[3]);

		sprintf(udp_buffer,"navboard|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|\n",
				height,apitch,aroll,g_pitch,g_roll,g_yaw,fusion_pitch,fusion_roll,
				motor[0],motor[1],motor[2],motor[3]);

		//printf(udp_buffer);
		udpClient_Send(&udp_vertical, udp_buffer, strlen(udp_buffer));

		///////////////////////////////////////////////////////////////
		/*
		printf("h=%.2f(adj=%.2f) pitch=%.2f(adj=%.2f) roll=%.2f(adj=%.2f)  m=|%.2f|%.2f|%.2f|%.2f|\r\n",
				height,PID_H_Output,
				apitch,PID_PITCH_Output,
				aroll,PID_ROLL_Output,
				motor[0],motor[1],motor[2],motor[3]);
		 */
	}

}









int main(int argc, char** argv)
{

	//
	//daemonize();

	//
	singleton(argv[0]);

	//
	system("killall -9 program.elf");

	system("sysctl -w kernel.panic=0");
	system("sysctl -w kernel.panic_on_oops=0");

	make_stable();





	return 0;
}



