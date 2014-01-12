

#include "../open_ardrone_v1_lib/all_headers_cpp.h"
#include "../open_ardrone_v1_lib/vbat.h"
#include "../open_ardrone_v1_lib/udp.h"
#include "../open_ardrone_v1_lib/singleton.h"
#include "../open_ardrone_v1_lib/daemonize.h"

typedef struct {
	uint32_t type;
	uint32_t offset;
	uint8_t buffer[1024];
} packet_t;

int main(int argc, char *argv[]) {

	if(argc!=3){
		printf("%s <port> <file>\n",argv[0]);
		exit(1);
	}

	int port=3333;
	char* name="/tmp/messages";

	port=atoi(argv[1]);
	name=argv[2];

	printf("udp broadcast start!\n");

	FILE *file;
	char *buffer;
	unsigned long fileLen;
	file = fopen(name, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s\n", name);
		exit(1);
	}

	//Get file length
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate memory
	buffer=(char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!\n");
		fclose(file);
		exit(1);
	}

	//Read file contents into buffer
	fread(buffer, fileLen, 1, file);
	fclose(file);

	udp_struct udp;
	if(udpClient_Init(&udp,port)){
		fprintf(stderr, "udpClient_Init error!\n");
		exit(1);
	}

	packet_t pkt;

	//start packet
	pkt.type=0;
	udpClient_Send(&udp, &pkt, sizeof(pkt));
    //
	for(int offset=0;offset<fileLen;offset+=1024){
		pkt.type=1;
		pkt.offset=offset;
		memcpy(&pkt.buffer,&buffer[offset],1024);

		//for(int k=0;k<3;k++){
		  udpClient_Send(&udp, &pkt, sizeof(pkt));
		//}
		//printf("sendto offset=%d\n",offset);
		usleep(100);
	}
	//end packet
	pkt.type=2;
	udpClient_Send(&udp, &pkt, sizeof(pkt));

	udpClient_Close(&udp);

	free(buffer);
	printf("udp broadcast end!\n");

	return 0;
}
