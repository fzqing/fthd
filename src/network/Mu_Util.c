#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "../include/Mu_Util.h"


int Mu_Strlen(const char *Ptr)
{
	if(Ptr == NULL)
		return 0;
	else
		return (strlen(Ptr));
}

char *Mu_itoa(int p)
{
	char *str = NULL;
	
 	if(!p)
		return NULL;
	else{
		if(NULL == (str = malloc(50))){
			Mu_ErrorPrint();
			return NULL;
		}
		snprintf(str, 50, "%d", p);
		return str;
	}

}

