#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define ECHOMAX 255
#define ECHORECV 32

void excep(char *errorMessage)
{
	perror(errorMessage);
	exit(1);
}

int main(int argc,char *argv[])
{
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	unsigned short echoServPort;
	unsigned int  fromSize;
	char *servIP;
	char *echoString;
	char echoBuffer[32];
	int echoStringLen;
	int respStringLen;

	if((argc<3) || (argc>4)){
		printf("Usage:%s <Server IP> <Echo Word> [<Echo Port>]\n",argv[0]);
		exit(1);
	}



	servIP = argv[1];
	echoString = argv[2];



	if((echoStringLen = strlen(echoString))>ECHOMAX){
		excep("Echo word too long\n");
		exit(1);
	}



	if(argc == 4){
		echoServPort = atoi(argv[3]);
		printf("%d\n",echoServPort);//iran
	}else{
		echoServPort = 7;
	}



	if((sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))<0){
		excep("Failed to make socket\n");
		exit(1);
	}



	memset(&echoServAddr,0,sizeof(echoServAddr));

	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);
	echoServAddr.sin_port = htons(echoServPort);

	if(sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *) 
	 &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
		excep("sent a different number of bytes than expected\n");

	fromSize = sizeof(fromAddr);

	if((respStringLen = recvfrom(sock,echoBuffer,ECHORECV,0
	 ,(struct sockaddr *)&fromAddr,&fromSize)) > strlen(echoBuffer))
		excep("recvfrom failed\n");

	if(echoServAddr.sin_addr.s_addr!=fromAddr.sin_addr.s_addr){
		printf("Error : received a packet from unknown source\n");
		exit(1);
	}

	echoBuffer[respStringLen] = '\0';
	printf("Received : %s\n",echoBuffer);

	close(sock);
	exit(0);
}
