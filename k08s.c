#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/file.h>
#include<errno.h>
#include<signal.h>

#define ECHOMAX 255

void excep(char *errorMessage);
void SIGIOHandler(int sigType);
void UseIdleTime();

int sock;

void excep(char *errorMessage)
{
	perror(errorMessage);
	exit(1);
}

void SIGIOHandler(int sigType){
	struct sockaddr_in echoClntAddr;
	unsigned int clntLen;
	int recvMsgSize;
	char echoBuffer[ECHOMAX];

	do
	{
		clntLen = sizeof(echoClntAddr);

		if((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0
		 , (struct sockaddr *) &echoClntAddr, &clntLen)) < 0){
			if(errno != EWOULDBLOCK)
				excep("recvfrom() failed\n");
		}else{
			printf("Handling client %s\n",inet_ntoa(echoClntAddr.sin_addr));

			if(sendto(sock, echoBuffer, recvMsgSize, 0
			 , (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr))
			 != recvMsgSize)
				excep("sendto() failed");
		}
	}while(recvMsgSize >= 0);
}

void UseIdleTime()
{
	printf("./\n");
	sleep(3);
}


int main(int argc,char *argv[])
{
	struct sockaddr_in echoServAddr;
	unsigned int cliAddrLen;
	unsigned short echoServPort;
	struct sigaction handler;


	if(argc != 2){
		printf("Usage : %s <UDP SERVER PORT>\n",argv[0]);
		exit(1);
	}


	echoServPort = atoi(argv[1]);


	if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		excep("socket() failed\n");


	memset(&echoServAddr, 0, sizeof(echoServAddr));	/*kouzoutaini 0 wo umeru*/
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	echoServAddr.sin_port = htons(echoServPort);


	if(bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
		excep("bind() failed\n");


	handler.sa_handler = SIGIOHandler;

	if(sigfillset(&handler.sa_mask) < 0)
		excep("sigfillset() failed\n");

	handler.sa_flags = 0;

	if(sigaction(SIGIO, &handler, 0) < 0)
		excep("sigaction() failed for SIGIO\n");

	if(fcntl(sock, F_SETOWN, getpid()) < 0)
		excep("Unable to set process owner to us\n");

	if(fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
		excep("Unable to put client sock into nonblocking/async mode\n");


	while(1){
		UseIdleTime();
	}

}
