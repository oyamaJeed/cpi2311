/*** clThread_12.c ***/

#include "./myTCP.h"

/*** MAIN THREAD ***/
void *threadMain(void *thrArgs) {
	int clSock;	// Socket Descripter for Client
	
	pthread_detach(pthread_self());	// --> release resources
	
	/* ARGUMENT */
	clSock = ((struct threadArgs *)thrArgs)->clSock;
	free(thrArgs);
	
	/* HANDLE A CLIENT REQUEST */
	clHandler(clSock);
	return (NULL);
}


/*** HANDLE A CLIENT REQUEST ***/
void clHandler(int clSock)
{
	char buf[BUF_LEN];
	int bytesRcvd;
	char replyBuf[R_BUF_LEN];
	char *bufPtr;
	char *nextPtr;
	int i;
	char str[6];
	int dataSize;
	struct httpReqLine line = {NULL,NULL,NULL};
	struct httpReqHeader head = {NULL,NULL};

	/* RECIEVE ONE LINE*/
	dataSize = recvOneLine(clSock, buf);
	line.method = strtok_re(buf, " \t\r\n", &nextPtr);
	pr_msg("METHOD:", line.method);
	line.uri = strtok_re(NULL, " \t\r\n", &nextPtr);
	pr_msg("URI:", line.uri);
	line.httpVer = strtok_re(NULL, " \t\r\n", &nextPtr);
	pr_msg("HTTP_VER:", line.httpVer);
	pr_msg("", "");
	for(int p=0 ; p < HEADERS_MAX ;p++){
		dataSize = recvOneLine(clSock, buf);
		
		numToStr((long int)p, str, 3);
		pr_msg("HEAD NO.", str);
		head.name = strtok_re(buf," \t\r\n", &nextPtr);
		pr_msg("HEAD.NAME:", head.name);
		head.val = strtok_re(NULL," \t\r\n", &nextPtr);
		pr_msg("HEAD.VAL:", head.val);
		pr_msg("", "");
	}

	/*dataSize = recvOneLine(clSock, buf);
	pr_msg("recv():", buf);
	numToStr(dataSize, str, 5);
	pr_msg("<size>:", str);*/
	
	/* CLOSE SOCKET */
	if (shutdown(clSock, 2) == -1) {
		excep("FAIL:shutdown()");
	}
	close(clSock);
	pr_msg("CONNECTION CLOSED.", "\n");
	return;
}

