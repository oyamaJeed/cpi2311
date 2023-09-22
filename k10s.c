#include "TCPEchoServer.h"
#include <pthread.h>
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

void *ThreadMain(void *arg);
int AcceptTCPConnection(int servSock);

struct ThreadArgs{
	int clntSock;
};

int main(int argc, char *argv[]){
	int servSock;
	int clntSock;
	unsigned short echoServPort;
	pthread_t threadID;
	struct ThreadArgs *threadArgs;

	if(argc != 2){
		fprintf(stderr,"Usage:%s<SERVER PORT>\n", argv[0]);
		exit(1);
	}
	echoServPort = atoi(argv[1]);

	servSock = CreateTCPServerSocket(echoServPort);

	for(;;){
		//perror("Yo!Say!\n");
		clntSock = AcceptTCPConnection(servSock);
		//perror("tatiga");
		if((threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs))) == NULL)
			excep("malloc() failed\n");
		threadArgs -> clntSock = clntSock;

		if(pthread_create(&threadID, NULL, ThreadMain, (void *)threadArgs) != 0)
			excep("pthread_create() failed\n");
		printf("with thread %ld\n", (long int) threadID);
		//perror("natuwo");
	}

}

void *ThreadMain(void *threadArgs){
	int clntSock;

	pthread_detach(pthread_self());

	clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
	free(threadArgs);

	HandleTCPClient(clntSock);

	return(NULL);
}

int AcceptTCPConnection(int servSock){
	int clntSock;
	struct sockaddr_in echoClntAddr;
	unsigned int clntLen;

	clntLen = sizeof(echoClntAddr);
	//perror("sigekisuru");

	if((clntSock = accept(servSock, (struct sockaddr *)&echoClntAddr, &clntLen)) < 0)
		excep("accept() failed\n");

	printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

	return clntSock;
}
