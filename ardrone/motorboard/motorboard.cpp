

#include "all_headers_cpp.h"
#include "gpio.h"
#include "motorboard.h"

int mot_fd; /* File descriptor for the port */


int motorboard_cmd(u08 cmd, u08 *reply, int replylen) {
	write(mot_fd,&cmd,1);
	return read(mot_fd,reply,replylen);
}

int motorboard_Init() {

	

	//configure motors
	int retval=0;
	u08 reply[256];
	int m;
	for(m=0;m<4;m++) {
		gpio_set(68+m,-1);
		motorboard_cmd(0xe0,reply,2);
		if(reply[0]!=0xe0 || reply[1]!=0x00)
		{
			printf("motor%d cmd=0x%02x reply=0x%02x\n",m+1,(int)reply[0],(int)reply[1]);
			retval=1;
		}
		motorboard_cmd(m+1,reply,1);
		gpio_set(68+m,1);
	}

	//all select lines active
	gpio_set(68,-1);
	gpio_set(69,-1);
	gpio_set(70,-1);
	gpio_set(71,-1);

	//start multicast
	motorboard_cmd(0xa0,reply,1);
	motorboard_cmd(0xa0,reply,1);
	motorboard_cmd(0xa0,reply,1);
	motorboard_cmd(0xa0,reply,1);
	motorboard_cmd(0xa0,reply,1);

	//reset IRQ flipflop - on error 106 read 1, this code resets 106 to 0
	gpio_set(106,-1);
	gpio_set(107,0);
	gpio_set(107,1);

	
	return retval;
}


//run motors: 0.0=minimum speed, 1.0=maximum speed (clipped to these values)
void mot_Run(float m0, float m1, float m2, float m3)
{
  mot.mot[0]=m0;
  mot.mot[1]=m1;
  mot.mot[2]=m2;
  mot.mot[3]=m3;


  //need 0xff for reliable startup, after startup min pwm 0x50 is allowed
  const u16 mot_pwm_min=0x00;
  const u16 mot_pwm_max=0x1ff;

  //convert to pwm values, clipped at mot_pwm_min and mot_pwm_max
  float pwm[4];
  int i;
  for(i=0;i<4;i++) {
    if(mot.mot[i]<0.0) mot.mot[i]=0.0;
    if(mot.mot[i]>1.0) mot.mot[i]=1.0;
    pwm[i]=mot_pwm_min + mot.mot[i]*(mot_pwm_max-mot_pwm_min);
	if(pwm[i]<mot_pwm_min) pwm[i]=mot_pwm_min;
	if(pwm[i]>mot_pwm_max) pwm[i]=mot_pwm_max;
  }

  mot_SetPWM((u16)pwm[0],(u16)pwm[1],(u16)pwm[2],(u16)pwm[3]);
}

//write motor speed command
//cmd = 001aaaaa aaaabbbb bbbbbccc ccccccdd ddddddd0 
void motorboard_SetPWM(u16 pwm0, u16 pwm1, u16 pwm2, u16 pwm3)
{
	u08 cmd[5];
	cmd[0] = 0x20 | ((pwm0&0x1ff)>>4);
	cmd[1] = ((pwm0&0x1ff)<<4) | ((pwm1&0x1ff)>>5);
	cmd[2] = ((pwm1&0x1ff)<<3) | ((pwm2&0x1ff)>>6);
	cmd[3] = ((pwm2&0x1ff)<<2) | ((pwm3&0x1ff)>>7);
	cmd[4] = ((pwm3&0x1ff)<<1);
	write(mot_fd, cmd, 5);
}

