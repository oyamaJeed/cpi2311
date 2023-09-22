#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 32
#define MAXPENDING 5

void excep(char *errorMessage);
void HandleTCPClient(int clntSocket);
int CreateTCPServerSocket(unsigned short port);
//int AcceptTCPConnection(int servSock);

void excep(char *errorMessage)
{
	perror(errorMessage);
	exit(1);
}

void HandleTCPClient(int clntSocket)
{
	char echoBuffer[RCVBUFSIZE];
	int recvMsgSize;

	if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
		excep("recv() failed\n");

	while(recvMsgSize > 0)
	{
		if(send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
			excep("send() failed\n");

		if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
			excep("recv() failed\n");
	}

	close(clntSocket);
}


int CreateTCPServerSocket(unsigned short port)
{
	int sock;
	struct sockaddr_in echoServAddr;

	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		excep("socket() failed\n");

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	echoServAddr.sin_port = htons(port);

	if(bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0)
		excep("bind() failed\n");

	if(listen(sock, MAXPENDING < 0))
		excep("listen() failed\n");

	return sock;
}


/*int AcceptTCPError(int servSock)
{
	int clntSock;
	struct sockaddr_in echoClntAddr;
	unsigned int clntLen;

	clntLen = sizeof(echoClntAddr);

	if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
		excep("accept() failed\n");

	printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

	return clntSock;
}*/
