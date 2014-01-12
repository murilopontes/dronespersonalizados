


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
#include "../open_ardrone_v1_lib/PID_v1.h"


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
	double ku=1.0;
	double tu=1.0;
	//
	double kp=0.8*ku;
	double ki=2*kp/tu;
	double kd=kp*tu/8;
	//
	PID     PID_H_Control(&PID_H_Input    ,&PID_H_Output    ,&PID_H_Setpoint    ,kp,ki,kd,DIRECT);
	PID PID_PITCH_Control(&PID_PITCH_Input,&PID_PITCH_Output,&PID_PITCH_Setpoint,kp,ki,kd,DIRECT);
	PID  PID_ROLL_Control(&PID_ROLL_Input ,&PID_ROLL_Output ,&PID_ROLL_Setpoint ,kp,ki,kd,DIRECT);
	//
	PID_H_Control.SetSampleTime(100);
	PID_PITCH_Control.SetSampleTime(100);
	PID_ROLL_Control.SetSampleTime(100);
	//
	PID_H_Control.SetOutputLimits(0.10,0.60);
	PID_PITCH_Control.SetOutputLimits(0.00,0.01);
	PID_ROLL_Control.SetOutputLimits(0.00,0.01);
	//
	PID_H_Control.SetMode(AUTOMATIC);
	PID_PITCH_Control.SetMode(AUTOMATIC);
	PID_ROLL_Control.SetMode(AUTOMATIC);
	/////////////////////////////////////////////////////////////////////////////////////////////////


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
		//printf("a %.2f %.2f %.2f\n",ax,ay,az);
		sum_ax+=ax;
		sum_ay+=ay;
		sum_az+=az;
		///////////////////////////////////////////////////////////
		double gx=(packet[5]);
		double gy=(packet[6]);
		double gz=(packet[7]);
		sum_gx+=gx;
		sum_gy+=gy;
		sum_gz+=gz;
		///////////////////////////////////////////////////////////
		//printf("g %.2f %.2f %.2f\n",gx,gy,gz);
		////////////////////////////////////////////////////////////
		/*
        double radian2degree = 57.295779513082320876798154814105f;
		double apitch =  -atan2(ax,az) * radian2degree;
		double aroll  =   atan2(ay,az) * radian2degree;
		sum_pitch_calibration+=apitch;
		sum_roll_calibration+=aroll;
		 */
	}

	double ax_off=sum_ax/(double)calibration_qtd;
	double ay_off=sum_ay/(double)calibration_qtd;
	double az_off=sum_az/(double)calibration_qtd;

	double gx_off=sum_gx/(double)calibration_qtd;
	double gy_off=sum_gy/(double)calibration_qtd;
	double gz_off=sum_gz/(double)calibration_qtd;



	double gravidade = az_off - ((ax_off+ay_off)/2.0f);
	printf("gravidade=%.2f\n",gravidade);
	az_off = az_off - gravidade;


	printf("ax_off=%.2f\n",ax_off);
	printf("ay_off=%.2f\n",ay_off);
	printf("az_off=%.2f\n",az_off);

	printf("gx_off=%.2f\n",gx_off);
	printf("gy_off=%.2f\n",gy_off);
	printf("gz_off=%.2f\n",gz_off);


	double pitch_offset=sum_pitch_calibration/(double)calibration_qtd;
	double roll_offset=sum_roll_calibration/(double)calibration_qtd;
	printf("navboard calibration done pitch_offset=%.2f roll_offset=%.2f\n",pitch_offset,roll_offset);

	/*
    if(fabs(pitch_offset)>6 || fabs(roll_offset)>6){
    	printf("navboard calibration error exit\n");
    	exit(1);
    }
	 */

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
		double ax=(packet[2] - ax_off)/100.0f;
		double ay=(packet[3] - ay_off)/100.0f;
		double az=(packet[4] - az_off)/100.0f;
		double radian2degree = 57.295779513082320876798154814105f;
		double apitch = ( -atan2(ax,az) * radian2degree );
		double aroll  = (  atan2(ay,az) * radian2degree );
		/////////////////////////////////////////////////////////
		double gx=(packet[5] - gx_off)/2.0f;//6.2f;
		double gy=(packet[6] - gy_off)/2.0f;//6.2f;
		double gz=(packet[7] - gz_off)/4.0f;//6.2f;
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

		sprintf(udp_buffer,"navboard|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|\n",
				height,apitch,aroll,g_pitch,g_roll,g_yaw,fusion_pitch,fusion_roll);
		//printf(udp_buffer);
		udpClient_Send(&udp_vertical, udp_buffer, strlen(udp_buffer));

		///////////////////////////////////////////////////////////


		PID_H_Input = height;
		PID_H_Control.Compute();

		PID_PITCH_Input=fusion_pitch;
		PID_PITCH_Control.Compute();

		PID_ROLL_Input=fusion_roll;
		PID_ROLL_Control.Compute();

		/////////////////////////////////////////////////////////////

		/*
		double adj_yaw=0;
		motor[0] = PID_H_Output  +PID_ROLL_Output -PID_PITCH_Output -adj_yaw;
		motor[1] = PID_H_Output  -PID_ROLL_Output -PID_PITCH_Output +adj_yaw;
		motor[2] = PID_H_Output  -PID_ROLL_Output +PID_PITCH_Output -adj_yaw;
		motor[3] = PID_H_Output  +PID_ROLL_Output +PID_PITCH_Output +adj_yaw;
		 */

		double height_error = height_setpoint - height ;
		if(height_error>0){
			//printf("muito baixo -> accelerar -> ");
			height_speed+=0.01/10.0f;
		}
		if(height_error<0){
			//printf("muito alto -> desaccelerar -> ");
			height_speed-=0.01/10.0f;
		}
		if(height_speed<0.20) height_speed=0.20;
		if(height_speed>0.60) height_speed=0.60;

		printf("height_error=(%.2f-%.2f)=%.2f height_speed=%.2f\n",height_setpoint,height,height_error,height_speed);

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


		motor[0] = height_speed -pitch_speed + roll_speed;
		motor[1] = height_speed -pitch_speed - roll_speed;
		motor[2] = height_speed +pitch_speed - roll_speed;
		motor[3] = height_speed +pitch_speed + roll_speed;

		mot_Run(motor[0],motor[1],motor[2],motor[3]);

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



