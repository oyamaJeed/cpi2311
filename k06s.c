/*** TCP/IP server program ***/
/* Usage: ./server HostName */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 7
#define RCVBUFSIZE 32

//char buf[]="Server\n";


int main(int argc, char *argv[])
{
  int sock;
  char recvString;
  struct sockaddr_in ServerAddr;
  struct sockaddr_in ClientAddr;
  struct hostent *HostInfo;
  int ServerSessionID;
  int ClientSessionID;
  int clientlength;

  char echoBuffer[RCVBUFSIZE];
  int recvMsgSize;
  
  if (argc!=2){
    fprintf(stderr,"Usage: ./server HostName!\n");
    exit(1);
  }
  
  if ((ServerSessionID=socket(AF_INET,SOCK_STREAM,0))==-1){
    fprintf(stderr, "Failed to make a socket!\n");
    exit(1);
  }
  
  if ((HostInfo=gethostbyname(argv[1]))==NULL){
    fprintf(stderr,"Failed to find host.\n");
    exit(1);
  }
  
  ServerAddr.sin_family=AF_INET;
  ServerAddr.sin_port=htons(PORT);
  memcpy((char *)&ServerAddr.sin_addr,(char *)HostInfo->h_addr_list[0],
                                                     HostInfo->h_length);
  if (bind(ServerSessionID,(struct sockaddr *)&ServerAddr,
                                               sizeof(ServerAddr))==-1){
    fprintf(stderr,"Failed to assign a socketname.\n");
    exit(1);
  }

  
  if (listen(ServerSessionID,5)==-1){
    fprintf(stderr,"Failed to listen.\n");
    exit(1);
  }

  
  while(1){
    clientlength=sizeof(ClientAddr);
    if ((ClientSessionID=accept(ServerSessionID,
                                       (struct sockaddr *)&ClientAddr,
                                       (socklen_t *)&clientlength))==-1){
      fprintf(stderr,"Failed to accept.\n");
      exit(1);
    }else{
	printf("Connect Client %s\n",inet_ntoa(ClientAddr.sin_addr));
	if((recvMsgSize = recv(ClientSessionID,echoBuffer,RCVBUFSIZE,0)) < 0){
	  printf("failed to recieve\n");
	}
    }
    while(recvMsgSize > 0){
      if(send(ClientSessionID,echoBuffer,recvMsgSize,0) != recvMsgSize){
	printf("failed to send\n");
      }
      if((recvMsgSize = recv(ClientSessionID,echoBuffer,recvMsgSize,0)) < 0){
	printf("failed\n");
      }

      if (shutdown(ClientSessionID,2)==-1){
        fprintf(stderr,"Failed to shutdown.\n");
        exit(1);
      }
      close(ClientSessionID);
    }
  }
}

