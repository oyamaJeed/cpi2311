#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 50002

#define RCVBUFSIZE 32

int main(int argc,char *argv[])
{
	int sock;
	struct sockaddr_in ServerAddr;
	unsigned short ServerPort;
	char *servIP;
	char *echoString;
	char echoBuffer[RCVBUFSIZE];
	unsigned int echoStringLen;
	int bytesRcvd,totalBytesRcvd;

	if((argc<3) || (argc>4))
	{
		fprintf(stderr,"Usage:%s<Server IP><Echo Word>[<Echo Port>]\n",argv[0]);
		exit(1);
	}

        servIP=argv[1];
        echoString=argv[2];

	if(argc==4)
		ServerPort=atoi(argv[3]);
	else
		ServerPort=7;

	if((sock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
		fprintf(stderr,"Failed to make socket\n");
		exit(1);
	}

	memset(&ServerAddr,0,sizeof(ServerAddr));
	ServerAddr.sin_family =AF_INET;
	ServerAddr.sin_addr.s_addr=inet_addr(servIP);
	ServerAddr.sin_port = htons(ServerPort);

	if(connect(sock,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr))<0){
		fprintf(stderr,"Failed to connect Server\n");
		exit(1);
	}

	echoStringLen = strlen(echoString);

	if(send(sock,echoString,echoStringLen,0)!=echoStringLen){
		fprintf(stderr,"Failed to send\n");
		exit(1);
	}

	totalBytesRcvd=0;
	printf("Recieved:");

	while(totalBytesRcvd < echoStringLen){
		if((bytesRcvd=recv(sock,echoBuffer,RCVBUFSIZE-1,0))<=0){
			fprintf(stderr,"Failed or connection closed prematurely\n");
			exit(1);
		}
		totalBytesRcvd += bytesRcvd;
		echoBuffer[bytesRcvd]='\0';
		printf("%s\n",echoBuffer);
	}

	printf("\n");

	close(sock);
	exit(0);
}


