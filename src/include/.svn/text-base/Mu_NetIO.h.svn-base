#ifndef __MU_NETIO_H__
#define __MU_NETIO_H__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>

#define SSLVER23
#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

SSL_CTX *ssl_ctx;
typedef int (*mu_reader)(int, char *, int);
typedef int (*mu_writer)(int, char *, int);
typedef int (*mu_poller)(int, char *, int);
typedef int (*mu_peeker)(int, char *, int);
typedef int (*mu_closer)(int);
typedef int (*mu_connecter)(int *, const char *, int ,double);
typedef int (*mu_selecter)(int, int, double);

/*define the types of the https or https*/
typedef enum _MuNetType{
	MU_NONE,
	MU_HTTP,
	MU_HTTPS
}MuNetType;

/*define writer or read*/
typedef enum _MuWR{
	MU_WRITE,
	MU_READ
}MuWR;

//the structure of the NetIO, include the fucntions Pointers
typedef struct _MuNetIO{
	volatile int type;		//MuNetType
	mu_reader reader;	//for reader
	mu_writer writer; 		//for writer
	mu_poller poller;		//for poller
	mu_selecter selecter;	//for selecter
	mu_peeker peeker;
	mu_closer closer;
	mu_connecter connecter;		//connect to server
}MuNetIO, *MuNetIOPtr;

MuNetIO MuIO;

//store the ssl conv, 
#define MAX_THD 10
#define TIMEOUT 30

typedef struct _MuSslTab{
	int fd;
	SSL *ssl;
}MuSslTab, *MuSslTadPtr;
MuSslTab mussltab[MAX_THD];

enum {
  WAIT_FOR_READ = 1,
  WAIT_FOR_WRITE = 2
};

#ifndef Mu_FineSsl
	#define Mu_FindSsl(ssl, fd) do{\
		int __i = 0;\
		while(__i < MAX_THD){\
			if(mussltab[__i].fd == fd)\
				break;\
			__i ++;\
		}\
		if(__i == MAX_THD)\
			ssl = NULL;\
		else\
			ssl = mussltab[__i].ssl;\
	}while(0)
#endif



int Mu_InitNetIO(MuNetIOPtr IO);

int Mu_SocketSelecter(int fd, int wr, double timeout);
int Mu_SocketSelect(int fd, int wr, double timeout);
int Mu_OpensslSelect(int fd, int wr, double timeout);

int Mu_Reader(int fd, char *buf, int len);
int Mu_SocketRead(int fd, char *buf, int len);
int Mu_OpensslRead(int fd, char *buf, int len);

int Mu_Writer(int fd, char *buf, int len);
int Mu_SocketWrite(int fd, char *buf, int len);
int Mu_OpensslWrite(int fd, char *buf, int len);

int Mu_Connecter(int *socket, const char *host, int port, double timeout);
int Mu_SocketConnect(int socket, struct sockaddr_in server, const char *host, int port, double timeout);
int Mu_OpensslConnect(int socket);

int Mu_Peeker(int fd, char *buf, int len);
int Mu_SocketPeeker(int fd, char *buf, int len);
int Mu_OpensslPeeker(int fd, char *buf, int len);

int Mu_Closer(int fd);
int Mu_SocketCloser(int fd);
int Mu_OpensslCloser(int fd);

int Mu_SslInit ();
int Mu_CheckCA();

void Mu_InitSslTab(int num);
#endif

	
