/*** utils_12.c ***/

#include "./myTCP.h"

/*** HANDLE EXCEPTION ***/
void excep(char *errMsg)
{
	perror(errMsg);
	exit(1);
}


/*** CONVERT A NUMBER TO STRING (RE-ENTRANT) ***/
void numToStr(long int num, char *str, int digit)
	// str : char <digit>+1 bytes
{
	long int q;
	long int remain;
	
	str[digit] = '\0';
	for (digit--; digit >= 0; digit--) {
		q = num / 10L;
		remain = num - q * 10L;
		str[digit] = (char)remain + '0';
		num = q;
	}
	return;
}


/*** 'inet_ntoa' (RE-ENTRANT) ***/
void inet_ntoa_re(struct in_addr addr, char *str)
	// str : char 16 bytes
{
	unsigned char *p_addr;
	
	p_addr = (unsigned char *)&addr;
	numToStr((long int)*(p_addr +0), str, 3);
	*(str +3) = '.';
	numToStr((long int)*(p_addr +1), str +4, 3);
	*(str +7) = '.';
	numToStr((long int)*(p_addr +2), str +8, 3);
	*(str +11) = '.';
	numToStr((long int)*(p_addr +3), str +12, 3);
	return;
}


/*** PRINT MESSAGE (RE-ENTRANT) ***/
void pr_msg(char *str1, char *str2)
{
	char msg[MSG_LEN] = "";
	char *ptr;
	ptr = msg;
	strcpy(ptr, str1);
	ptr += strlen(str1);
	strcpy(ptr, str2);
	ptr += strlen(str2);
	strcpy(ptr, "\n");
	write(fileno(stdout), msg, strlen(msg));
	return;
}


/*** SPLIT STRINGS TO TOKENS ***/
/*** 'strtok' (RE-ENTRANT) ***/
char *strtok_re(char *str, const char *delim, char **laterPart)
{
	char *ptrLeading;			// Leading Delimiter Scan Pointer
	char *ptrToken;				// Token Scan Pointer
	char delimPlus[16] = " ";	// Dlimiter + Space
	
	if (str == NULL) {
		ptrLeading = *laterPart;	// FOR CONTINUOUS PROCESSING
	} else {
		ptrLeading = str;
	}
	
	/* SKIP DELIMITER　AT THE BEGINNING */
	strcpy(delimPlus +1, delim);
	while (*ptrLeading && (strchr(delimPlus, *ptrLeading) != NULL)) {
		/* '*ptrLeading' IS SOME DELIMITER */
		ptrLeading++;
	}
	
	/* SEARCH THE END OF ONE TOKEN */
	ptrToken = ptrLeading;
	while (*ptrToken && (strchr(delim, *ptrToken) == NULL)) {
		/* '*ptrToken' IS NOT ANY DELIMITER */
		ptrToken++;
	}
	if (*ptrToken) {
		*laterPart = ptrToken + 1;
		*ptrToken = '\0';
	} else {
		*laterPart = ptrToken;
	}
	return (ptrLeading);
}

