/*******************************************************************
* 					====Microunit Techonogy Co,.LTD.====
* File Name:
* 		Mu_HttpUtil.c
* Description:
* 		This file contain the method of Init and checksum
* Revision History:
* 		14-5-2008 ver1.0
*Author:
* 		ssw (fzqing@gmail.com), cks, wkx
* 					***PROTECTED BY COPYRIGHT***
********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/vfs.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/ssl.h>

#include "../include/Mu_HttpUtil.h"

/*********************************************************
Description:
		base64 check sum, use for query servr
input:
		*s : string will be checked
		store: return value
		length: the length of 8s
Output:
		void
Lock:
		NONE
*********************************************************/
void Mu_Base64Encode(const char *s, char *store, int length)
{
	//Conversion table
	char tbl[64] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3',
		'4', '5', '6', '7', '8', '9', '+', '/'
	};
	
	int i;
	unsigned char *p = (unsigned char *) store;
	
	//Transform the 3x8 bits to 4x6 bits, as required by base64
	for (i = 0; i < length; i += 3){
		*p++ = tbl[s[0] >> 2];
		*p++ = tbl[((s[0] & 3) << 4) + (s[1] >> 4)];
		*p++ = tbl[((s[1] & 0xf) << 2) + (s[2] >> 6)];
		*p++ = tbl[s[2] & 0x3f];
		s += 3;
	}
	
	//Pad the result if necessary
	if (i == length + 1){
		*(p - 1) = '=';
	}else if (i == length + 2){
		*(p - 1) = *(p - 2) = '=';
	}
	
	//Zero-terminate it
	*p = '\0';

	return;
}

char *Mu_GetCookiesForHead()
{
	char wwwcookie[] = "Cookie: ";
	char *ret = NULL;

	ret = (void *)malloc((sizeof(char)*(Mu_Strlen(wwwcookie)+Mu_Strlen(cookies)))+ 3);
	sprintf(ret, "%s%s\r\n", wwwcookie, cookies);

	return ret;
}
	
	

//Get user base-info
char *Mu_GetBasicAuth(const char *user, const char *passwd)
{
	char *p1, *p2;
	char wwwauth[] = "Authorization: BASIC ";
	
	char *ret = NULL;
	int len = Mu_Strlen(user) + Mu_Strlen(passwd) + 1;
	int b64len = 4 * ((len + 2) / 3);
	p1 = (void *)malloc(sizeof(char) * len + 1);
	sprintf(p1, "%s:%s", user, passwd);
	p2 = (void *)malloc(sizeof(char) * b64len + 1);
	
	/*Encode username:passwd to base 64 */
	Mu_Base64Encode(p1, p2, len);
	
	ret = (void *)malloc((sizeof(char)*Mu_Strlen(wwwauth))+ b64len + 3);
	sprintf(ret, "%s%s\r\n", wwwauth, p2);
	Mu_Free(p1);
	Mu_Free(p2);
	
	//Return Base64code of base-info
	return ret;
}

void Mu_InitURL(Mu_URLPtr mu_url)
{
	mu_url->url = NULL;
	mu_url->host = NULL;
	mu_url->path = NULL;
	mu_url->file = NULL;
	mu_url->username = NULL;
	mu_url->password = NULL;

	mu_url->scheme = 0;
	mu_url->port = 0;
	mu_url->urllen = 0;
	mu_url->hostlen = 0;
	mu_url->pathlen = 0;
	mu_url->filelen = 0;
	mu_url->usernamelen = 0;
	mu_url->passwordlen = 0;

	return;
}

void Mu_FreeURL(Mu_URLPtr Ptr)
{
	Mu_Free(Ptr->url);
	Mu_Free(Ptr->host);
	Mu_Free(Ptr->path);
	Mu_Free(Ptr->username);
	Mu_Free(Ptr->password);

	Ptr->scheme = 0;
	Ptr->port = 0;
	Ptr->urllen = 0;
	Ptr->hostlen = 0;
	Ptr->pathlen = 0;
	Ptr->filelen = 0;
	Ptr->usernamelen = 0;
	Ptr->passwordlen = 0;

	return;


}
void Mu_InitBuild(Mu_BuildPtr build)
{
	build->fd = 0;
	build->endops = 0;
	build->startops = 0;
	build->method = MU_NOMETH;

	return;

}
void Mu_InitDnStat(Mu_DnStatPtr DnStat)
{
	DnStat->type = MU_NOMETH;
	DnStat->fd = 0;
	DnStat->xmlfd = 0;
	DnStat->startops = 0;
	DnStat->length = 0;
	DnStat->resume = 0;
	return;
}	 	

void Mu_InitHttpStat(Mu_HttpStatusPtr Ptr)
{
	Ptr->threadnumber = 0;
	Ptr->seekstart = 0;
	Ptr->seeknew = 0;
	Ptr->bytesleft= 0;
	Ptr->bytesexcept= 0;
	Ptr->retry = 0;
	Ptr->fd = 0;
	Ptr->tempfd = 0;
	Ptr->statcode = 0;
	Ptr->threadstatuse= 0;
	
	Ptr->starttime.tv_sec = 0;
	Ptr->starttime.tv_usec = 0;
	Ptr->dltime.tv_sec = 0;
	Ptr->dltime.tv_usec = 0;
	
	Ptr->rates = 0;
	return;
}

/****************************************************************
Description:
		When Download the files, we need to Use multi-thread,
		Before use them, we must Init It

		cacculate the number of thread, Then Init them
Input:
		length:	the total length of the file(or segments)
Output:
		MUOK: no error
LOCK:
		NONE
******************************************************************/
int Mu_InitThreads(off_t length)
{
	off_t threadsize = length;
	//off_t temp = 0;
	int tempnum = MAX_THD;
	int i = 0;

#if 1
	//get the segments length
	while((tempnum > 1) && ((threadsize =((int)(length/(tempnum - 1)))) < MAX_THD_LEN)) {
		tempnum --;
	}
	
	//record the threads'numbers
	if(1 == tempnum)
		threadnum = 2;	//only one pthread will be created.
	else
		threadnum = tempnum; //tempnum pthread will be created.
#endif
//	threadnum = tempnum = 1;
printf("the threads is %d\n", tempnum);
	for(i = 0; i < threadnum ; i ++)
		Mu_InitHttpStat(&(hs[i]));

	//set the NO. of the bytes, every thread will download¡­
	for(i = 0; i < threadnum - 1; i ++){
		hs[i].bytesleft = threadsize;
		hs[i].bytesexcept= threadsize;
		hs[i].seeknew = threadsize * i;
	}
	
	//get ride of the last thread, in order to make sure the file is complete
	hs[i].bytesleft = length - (threadsize * i);
	hs[i].bytesexcept= length - (threadsize * i);
	hs[i].seeknew = threadsize * i;
	
printf("after the thread Init \n");
printf("the seekstart is %ld, the seeknew is %ld\n", hs[i].seekstart, hs[i].seeknew);
	return MUOK;
}	

