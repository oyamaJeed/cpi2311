#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define ECHOMAX 255

void excep(char *errorMessage)
{
	perror(errorMessage);
	exit(1);
}

int main(int argc,char *argv[])
{
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
	unsigned short echoServPort;
	int recvMsgSize;


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


	while(1){
		cliAddrLen = sizeof(echoClntAddr);

		if((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0
		 , (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
			excep("recvfrom() failed\n");

		printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

		if(sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) &echoClntAddr
		 , sizeof(echoClntAddr)) != recvMsgSize)
			excep("sendto() sent a different number of bytes than expected");
	}

}
