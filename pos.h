#ifndef _kokyaku_
#define _kokyaku_

#include<stdio.h>
#include<stdlib.h>
#include<libpq-fe.h>

int users(int start_id, int end_id);
int usrpr(int customer_id);
int usrup(char* customer_id);


#endif
