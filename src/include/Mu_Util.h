#ifndef __MU_UTIL_H__
#define __MU_UTIL_H__
#include "Mu_error.h"

#define Mu_Free(_note)\
	do{\
		if(_note){\
			free(_note);\
			_note = NULL;\
		}\
	}while(0)
		
#define Mu_Close(_fd)\
	do{\
		if(_fd){\
			close(_fd);\
			_fd = 0;\
		}\
	}while(0)

int Mu_Strlen(const char *Ptr);
char *Mu_itoa(int p);

#endif
