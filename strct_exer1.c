#include <stdio.h>
#include <string.h>

#ifndef USER_COM_ENA
	#define USER_COM_ENA
#endif

#include "exer_com.h"

int main(int argv, char *argc[])
{
	ST_EXER	st_val = {'1','2',"Good morning!"};
	ST_EXER *st_val_p = &st_val;

	printf("*** init ****\n");

	printf("struct from=%c\n", st_val_p->from);
	printf("struct to=%c\n", st_val.to);
	printf("struct msg=%s\n",st_val_p->msg);

//	st_val = {'3','4',"Good afternoon!"};
	st_val_p->from = '3';
	st_val_p->to = '4';
	strcpy(st_val_p->msg,"Good Afternoon!");


	printf("*** Modify ****\n");

	printf("struct from=%c\n", st_val_p->from);
	printf("struct to=%c\n", st_val.to);
	printf("struct msg=%s\n", st_val_p->msg);

	printf("COM_VALUE=%d\n", COM_VALUE);

	return 0;
}
