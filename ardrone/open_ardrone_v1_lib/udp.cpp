


#include "all_headers_cpp.h"
#include "udp.h"

int udpClient_Init(udp_struct *udp,  int port)
{
  udp->slen=sizeof(udp->si_other);
  
  if ((udp->s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) return 1;
  
  memset((char *) &udp->si_other, 0, sizeof(udp->si_other));

  udp->si_other.sin_family = AF_INET;
  udp->si_other.sin_port = htons(port);

  udp->si_other.sin_addr.s_addr = htonl(INADDR_BROADCAST);

  int broadcastEnable=1;
  //int ret=

  setsockopt(udp->s, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

  char *opt = (char*)"ath0";

  setsockopt(udp->s, SOL_SOCKET, SO_BINDTODEVICE, opt, 4);

  //if (inet_aton(host, &udp->si_other.sin_addr)==0) return 2;

  return 0;
}

int udpClient_Send(udp_struct *udp, void* buf, int len)
{  
  if (sendto(udp->s, buf, len, 0, (const struct sockaddr*)&udp->si_other, udp->slen)<0){
	  perror("udpClient_Send -> ");
	  return 1;
  }
  return 0;
}

void udpClient_Close(udp_struct *udp)
{  
  close(udp->s);
}


int udpServer_Init(udp_struct *udp, int port, int blocking)
{
  udp->slen=sizeof(udp->si_other);
 
  if ((udp->s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) return 1;
 
  if(blocking==0) { 
    // Set socket to be non-blocking.  All of the sockets for    
    // the incoming connections will also be non-blocking since  
    // they will inherit that state from the listening socket.   
    int x;
    x = fcntl(udp->s, F_GETFL, 0);
    fcntl(udp->s, F_SETFL, x | O_NONBLOCK);
  }
 
  //Bind the socket  
  memset((char *) &udp->si_me, 0, sizeof(udp->si_me));
  udp->si_me.sin_family = AF_INET;
  udp->si_me.sin_port = htons(port);
  udp->si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(udp->s, (const struct sockaddr*)&udp->si_me, sizeof(udp->si_me))==-1) return 3;

  return 0;
}

//returns size of packet received or returns -1 and errno=EWOULDBLOCK if no data available
int udpServer_Receive(udp_struct *udp, void* buf, int len)
{
  return recvfrom(udp->s, buf, len, 0, (struct sockaddr*)&udp->si_other, &udp->slen);
}
  
void udpServer_Close(udp_struct *udp)
{  
  close(udp->s);
}
