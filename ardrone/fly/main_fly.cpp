/*
    main_fly.c - AR.Drone fly demo program

    Copyright (C) 2011 Hugo Perquin - http://blog.perquin.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/



#include "../open_ardrone_v1_lib/all_headers_cpp.h"
#include "../open_ardrone_v1_lib/type.h"
#include "../open_ardrone_v1_lib/util.h"
#include "../open_ardrone_v1_lib/mot.h"
#include "../open_ardrone_v1_lib/udp.h"
#include "controlthread.h"

int main()
{

  ///////////////////////////////////////////////////////////////////////////
  //kill program.elf
  int rc = system("/usr/bin/killall -9 program.elf > /dev/null 2>&1");
  printf("killall -9 program.elf -> returncode=%d  (0=killed,256=not found)\n",rc);	

  //////////////////////////////////////////////////////////////////////////
  //controller start
  ctl_Init();

  ///////////////////////////////////////////////////////////////////////////////
  //command packet buffer
  char cmdbuf[1024];
  //control channel is UDP port 7777
  udp_struct udpCmd;
  //server UDP server in blocking mode
  udpServer_Init(&udpCmd,7777,1);
  while(1) { 
	//clear command buffer
	memset(&cmdbuf,0,sizeof(cmdbuf));
    //wait for next packet on cmd port
	int cmdlen=udpServer_Receive(&udpCmd, cmdbuf, sizeof(cmdbuf));
	if(cmdlen<=0){
		continue;
	}

	///////////////////////////////////////
    //command format "s,<pitch>,<roll>,<yaw>,<height>" //
	//example:
	// "s,30,0,0,100" -> set pitch to 30 degree, roll to 0 degree, yaw to 0 degree, height to 100 cm

	//////////////////////////////////
	//delimiter
    char delims[] = ",";

    ///////////////////////////////////
    //search for "s"
    char *result = NULL;
    result = strtok( cmdbuf, delims );
    if(strcmp(result,"s")) continue;

    ///////////////////////////////////
    //search for <pitch>,<roll>,<yaw>,<height>
    int i=0;
    float val[4];
    result = strtok( NULL, delims );
    while( i<4 && result != NULL ) {
      val[i]=atof(result);
      result = strtok( NULL, delims );
      i++;
    }
    //////////////////////////////////////////////////
    if(i==4) {
      //print successfully decoded command <pitch>,<roll>,<yaw>,<height>
      printf("s:%f,%f,%f,%f\r\n", val[0],val[1],val[2],val[3] );
      //send to controller execute command <pitch>,<roll>,<yaw>,<height>
      ctl_SetSetpoint(val[0],val[1],val[2],val[3]);
    }

  }

  //controller end
  ctl_Close();
  return 0;

}
