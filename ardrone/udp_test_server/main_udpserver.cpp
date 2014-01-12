





#include "../open_ardrone_v1_lib/all_headers_cpp.h"
#include "../open_ardrone_v1_lib/udp.h"

void diep(const char *s)
{
  perror(s);
  exit(1);
}

int main(void)
{
	system("killall -9 program.elf");
	system("sysctl -w kernel.panic=0");
	system("sysctl -w kernel.panic_on_oops=0");

  udp_struct udp;

  int msglen;
  char buf[512];
  
  if(udpServer_Init(&udp,9930,0)) diep("udpServer_Init");
  
  int i;
  for (i=0; i<10; i++) {
    int cnt=0;
    do {
      msglen = udpServer_Receive(&udp, buf, 512);
      cnt++;
    } while(msglen<=0);
    printf("Received at cnt=%d -> '%s'\n\n", cnt, buf);
  }
  
  udpClient_Close(&udp);
  return 0;
}
