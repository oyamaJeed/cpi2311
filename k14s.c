/*** k12s.c ***/
/* HTTP SERVER PART 1                 */
/*     Usage: ./k12s <server port> */
/*#include "TCPEchoServer.h"
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
#include<signal.h>*/

#include "./myTCPSv.h"
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

	servSock = createTCPSvSock(echoServPort); //return sock

	for(;;){
		clntSock = acceptTCPConn(servSock);
		if((threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs))) == NULL)
			excep("malloc() failed\n");
		threadArgs -> clntSock = clntSock;

		if(pthread_create(&threadID, NULL, threadMain, (void *)threadArgs) != 0)
			excep("pthread_create() failed\n");
		printf("thread id %ld\n", (long int) threadID);
	}

}

