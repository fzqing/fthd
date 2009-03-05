#ifndef __MU_HTTP_H__
#define __MU_HTTP_H__
#include <time.h>
#include <pthread.h>
#include "Mu_NetIO.h"

#define MU_MAX_HEADER 1024 //Mu_FetchHeader
#define USER_AGENT "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT)" 
#define QUERY_CONTENT "Content-Disposition: form-data; name=\"cmdXML\"; filename=\"xml\"\r\nContent-Type: text/xml\r\n\r\n"
#define QUERY_BOUNDARY "----------7d0123456789a\r\n"
#define QUERY_BOUNDARY_END "------------7d0123456789a\r\n"
//#define TIMEOUT 30
#define DEBUG
char *filename;
char *relocate;
int threadnum ;
pthread_mutex_t lock;
//SSL_CTX *ssl_ctx;

#if 0
#ifndef _MU_BOOL_
#define _MU_BOOL_
	typedef enum _Mu_BOOL{
		FALSE,
		TRUE
	}MU_BOOL;
#endif
#endif
//include Mu_Query.h
typedef enum _Mu_BuildMethod{
	MU_NOMETH,
	MU_GET = 1,
	MU_HEAD,
	MU_POST
}Mu_BuildMethod;

//include Mu_Down.h
typedef enum _Mu_Urlscheme
{
	URL_NONE,
	URL_HTTP,
	URL_HTTPS
}Mu_Urlscheme;


typedef enum _Mu_DnType
{
	MU_QUERY = 1,
	MU_DOWNLOAD = 2
}Mu_DnType;


typedef enum _Mu_DnStatCode
{
	MU_DNCNT = 1,
	MU_DNRCV,
	MU_DNCMP,
	MU_RECNT,
}Mu_DnStatCode;


typedef struct _Mu_URL
{
	char *url;
	char *host;
	char *path;
	char *file;
	char *username;
	char *password;
	//enum url_scheme scheme;
	
	Mu_Urlscheme scheme;
	int port;
	int urllen;
	int hostlen;
	int pathlen;
	int filelen;
	int usernamelen;
	int passwordlen;
}Mu_URL, *Mu_URLPtr;
 Mu_URL mu_url ;

typedef struct _Mu_Build
{
	Mu_BuildMethod method;
	int fd;
	off_t startops;
	off_t endops;
}Mu_Build, *Mu_BuildPtr;

//include Mu_Query.h
typedef struct _Mu_DnStat{
	Mu_BuildMethod type; //the type of Query
	int fd;		//the file descriptor we will write into
	int xmlfd;	//POST signal
	off_t startops;//the start opsition of the file
	off_t length;	//the length of the file
	int resume;	//wether resuming downloading
}Mu_DnStat, *Mu_DnStatPtr;

typedef struct _Mu_Httpstatus{
	pthread_t threadnumber;//the number of thread
	off_t seekstart;	//the space, where theread will wirte into
	off_t seeknew;	//use to resume, when thread downloading, It must change the value
	off_t bytesleft;	//left bytes, the thread left
	off_t bytesexcept;	//NEED to download bytes, the thread will be
	int retry;		//the retry times
	int fd;			//the file descriptor, the thread will write into
	int tempfd;
	int statcode;		//when the thread running, it's the statcode of http or https
	int threadstatuse;	//statuse of the thread
	struct timeval starttime;	//the start time of the thread start downloading
	struct timeval dltime;	//the endtime of the thread cancell the downloading
	int rates;		//the rate of downloading
}Mu_HttpStatus, *Mu_HttpStatusPtr;

Mu_HttpStatus hs[MAX_THD];


#define CHECK_STATOK(x) (((x) >= 200) && ((x) <= 299))
#define CHECK_STATRE(x) ((x) == 303)

#endif


