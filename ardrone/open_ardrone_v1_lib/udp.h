
#ifndef __UDP_H
#define __UDP_H

#include <netinet/in.h>
#include <stdint.h>

typedef struct
{
  int s;
  struct sockaddr_in si_me;
  struct sockaddr_in si_other;
  socklen_t slen;
} udp_struct;

int  udpClient_Init(udp_struct *udp, int port);
int  udpClient_Send(udp_struct *udp, void* buf, int len);
void udpClient_Close(udp_struct *udp);

int udpServer_Init(udp_struct *udp, int port, int blocking);
int udpServer_Receive(udp_struct *udp, void* buf, int len);
void udpServer_Close(udp_struct *udp);

#endif
