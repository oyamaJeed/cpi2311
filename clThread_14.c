/*** clThread_12.c ***/

#include "./myTCPSv.h"
#define SIZE 15*1024


unsigned int getFileSize(const char *path) {
    int size, read_size;
    char read_buf[SIZE];
    FILE *f;

    f = fopen(path, "rb");
    if (f == NULL) {
        return 0;
    }

    size = 0;
    do {
        read_size = fread(read_buf, 1, SIZE, f);
        size += read_size;
    } while(read_size != 0);

    fclose(f);

    return size;
}


int getProcessing(char *body, char *file_path) {

	FILE *f;
	int file_size;

	/* ファイルサイズを取得 */
	file_size = getFileSize(file_path);
	if (file_size == 0) {
		/* ファイルサイズが0やファイルが存在しない場合は404を返す */
		printf("getFileSize error\n");
	}

	/* ファイルを読み込んでボディとする */
	f = fopen(file_path, "r");
	fread(body, 1, file_size, f);
	fclose(f);

}

int createResponseMessage(char *response_message, char *header, char *body, unsigned int body_size) {

	unsigned int no_body_len;
	unsigned int body_len;

	response_message[0] = '\0';

	sprintf(response_message, "HTTP/10.19.134.181 200 OK\r\n%s\r\n", header);

        no_body_len = strlen(response_message);
        body_len = body_size;

        /* ヘッダーフィールドの後ろにボディをコピー */
        memcpy(&response_message[no_body_len], body, body_len);
	
	//printf("responseMassage : %s\n",response_message);

	return no_body_len + body_len;
}

int sendResponseMessage(int sock, char *response_message, unsigned int message_size){
	int send_size;

	send_size = send(sock, response_message, message_size, 0);
	//printf("MSize = %s\n",message_size);

	return send_size;
}

void showMessage(char *message, unsigned int size) {
    unsigned int i;

    printf("Show Message\n\n");

    for (i = 0; i < size; i++) {
        putchar(message[i]);
    }
    printf("\n\n");
}

int sendHtml(int clSock, char *uri){
	int send_size;
	int file_size;
	int request_size, response_size;
	//char request_message[SIZE];
	char response_message[SIZE];
	//char method[SIZE];
	//char target[SIZE];
	char header_field[SIZE];
	char body[SIZE];
	//unsigned int file_size;
	//int send_size;
	int status;
	
	strcpy(uri,"/home/meit2511/cpi2311/k13.html");

	getProcessing(body, uri);

	//file_size = getFileSize(&target[1]);
	file_size = getFileSize(uri);
	sprintf(header_field,"Content-Length: %d\r\n",file_size);

	response_size = createResponseMessage(response_message,header_field,body,file_size);
	printf("responseSize = %d\n",response_size);
	if(response_size == 1){
		printf("createResponseMessage error\n");
		excep("");
        }

	showMessage(response_message, response_size);
	sendResponseMessage(clSock, response_message, response_size);
	return send_size;
}




/*** HANDLE A CLIENT REQUEST ***/
void clHandler(int clSock, unsigned long thrIDStr)
{
	char buf[BUF_LEN];
	int bytesRcvd;
	char replyBuf[R_BUF_LEN];
	char *bufPtr;
	char *nextPtr;
	int i;
	char str[6];
	int dataSize;

	int request_size, response_size;
	char method[SIZE];
	char target[SIZE];
	char body[SIZE];
	unsigned int file_size;
	int send_size;

	char buffer[64];
	char *p = buffer;

	struct httpReqLine line = {NULL,NULL,NULL};
	struct httpReqHeader head = {NULL,NULL};

	snprintf(p, 64, "%lu", thrIDStr);
	pr_msg("thrIDStr:",p);

	/* RECIEVE ONE LINE*/
	while(1){
		dataSize = recvOneLine(clSock, buf);
		line.method = strtok_re(buf, " \t\r\n", &nextPtr);
		pr_msg("METHOD:", line.method);
		line.uri = strtok_re(NULL, " \t\r\n", &nextPtr);
		if(strcmp(line.method,"GET")==0){
			send_size = sendHtml(clSock, line.uri);
		}
		line.httpVer = strtok_re(NULL, " \t\r\n", &nextPtr);
		thrIDStr = line.httpVer;

		/* CLOSE SOCKET */
		if (shutdown(clSock, 2) == -1) {
			excep("FAIL:shutdown()");
		}
		if(send_size != -1){
			close(clSock);
			pr_msg("CONNECTION CLOSED.", "\n");
			return;
		}
	}
}


/*** MAIN THREAD ***/
void *threadMain(void *thrArgs) {
	int clSock;	// Socket Descripter for Client
	unsigned long *thrIDStr;
	
	pthread_detach(pthread_self());	// --> release resources

	thrIDStr = pthread_self();

	/* ARGUMENT */
	clSock = ((struct threadArgs *)thrArgs)->clSock;
	free(thrArgs);
	
	/* HANDLE A CLIENT REQUEST */
	clHandler(clSock,thrIDStr);
	return (NULL);
}
