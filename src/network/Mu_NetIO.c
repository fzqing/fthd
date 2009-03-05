/*******************************************************************
* 					====Microunit Techonogy Co,.LTD.====
* File Name:
*
* 		Mu_NetIO.c
*
* Description:
*
* 		This file contain the method of operation sockets
*		including command socket and SSL socket 
*
* Revision History:
*
* 		4-5-2008 ver1.0
*
*Author:
*
* 		ssw (fzqing@gmail.com)
*
* 					***PROTECTED BY COPYRIGHT***
********************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/times.h>
#include <sys/select.h>

#include "../include/Mu_NetIO.h"
#include "../include/Mu_error.h"
#include "../include/Mu_Util.h"

static const char rand_sed[] = "the string use to create the encrypt,  Use for MuFTAD!";

/*****************************************************
*Description:
		selection Rounties for socket and SSl use Openssl
		the caller test MuIO.type to detect which routines 
		will be used
Input:
		fd: socket 
		wr: two values, include WAIT_FOR_READ &
			WAIT_FOR_WRITE
		timeout: for seek a timeout value

Output:
		return value ,when no error happened
		return MUOK;
		0 No socket is valid
		>0 the socket is readable or writeable
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 4-5-2008)
******************************************************/
int Mu_SocketSelect(int fd, int wr, double timeout)
{
	int ret = 0;
	struct timeval tmout;

	fd_set fdset;
	fd_set *rset = NULL, *wset = NULL;

	FD_ZERO(&fdset);
	FD_SET(fd, &fdset);

	if(wr & WAIT_FOR_READ)
		rset = &fdset;
	if(wr & WAIT_FOR_WRITE)
		wset = &fdset;

	tmout.tv_sec = (long)timeout;
	tmout.tv_usec = 1000000 * (timeout - (long)timeout);
	
	do
		ret = select(fd + 1, rset, wset, NULL, &tmout);
	while(ret < 0 && errno == EINTR);

	return ret;
}

int Mu_OpensslSelect(int fd, int wr, double timeout)
{
	SSL *ssl = NULL;
	
	//find ssl according to fd in mussltab 
	Mu_FindSsl(ssl, fd);
	if(NULL == ssl) return MUESSL;
	
	//Openssl's select founction 
	//Just use SSL_read function,can use SSL_pending to detect the socket is readable
	//when use to detect the socket is writeable, the SSL_pendng's return value is ZERO

	//If the SSL_pending function returns a return code of 0, 
	//issue the select function, passing the file descriptor of the socket to check if 
	//the socket is readable. Readable means more data has been received from the network 
	//on the socket.
	if(SSL_pending(ssl)){
		if(wr & WAIT_FOR_READ)
			return 1;
		else
			Mu_ErrorPrint();
	}

	return Mu_SocketSelect(fd, wr, timeout);
}
	
int Mu_Selecter(int fd, int wr, double timeout)
{
	int ret = -1;
	
	if(timeout == 0) return 1;
	
	switch(MuIO.type){
		case MU_HTTP:
			ret = Mu_SocketSelect(fd, wr, timeout);
			break;

		case MU_HTTPS:
			ret = Mu_OpensslSelect(fd, wr, timeout);
			break;
		
		default:
			break;
	}

	if(!ret)
		errno = MUETIMEOUT;

	if(ret < 0)
		return 0;

	return ret;
}

/***************************************************************
Description:
		read bytes from socket fd, write the bytes into the buf
		the max length of the buf is len
Input:
		fd: socket read form
		buf: store the bytes sapce
		len: the length of buf
Output:
		the length of reada,the max is length
		when error happened, the return value is negtive
Lock:
		NONE
****************************************************************/
int Mu_Reader(int fd, char *buf, int len)
{
	//select the socket is readable
	if(!(MuIO.selecter(fd, WAIT_FOR_READ, TIMEOUT)))
		return -1;

	switch(MuIO.type){
		case MU_HTTP:
			return Mu_SocketRead(fd, buf, len);
		case MU_HTTPS:
			return Mu_OpensslRead(fd, buf, len);
		default:
			return MUEERO;
	}
}

int Mu_SocketRead(int fd, char *buf, int len)
{
	int res;
	
	do
		res = read(fd, buf, len);
	while(res == -1 && errno == EINTR);

	return res;
}

int Mu_OpensslRead(int fd, char *buf, int len)
{
	int res;
	SSL *ssl = NULL;
	
	//find the ssl according to the fd,
	//the function is use to multi-thread ssl downloading
	Mu_FindSsl(ssl, fd);

	if(NULL == ssl)
		return MUEERO;

	do
		res = SSL_read(ssl, buf, len);
	while(res == -1
		&& SSL_get_error(ssl, res) == SSL_ERROR_SYSCALL
		&& errno == EINTR);

	return res;
}


/***********************************************************
Description:
		write the bytes in buffer into the socket,and send them 
		into network, the buffer length is len
Input:
		fd: the socket, though it to send bytes
		buf: bytes store the bytes, which must be send to network
		len: the number of bytes
Output:
		<=0: error happened
		len: send the bytes in buf
****************************************************************/
int Mu_Writer(int fd, char *buf, int len)
{
	if(!(MuIO.selecter(fd, WAIT_FOR_WRITE, TIMEOUT)))
		return -1;
	
	switch(MuIO.type){
		case MU_HTTP:
			return Mu_SocketWrite(fd, buf, len);
		case MU_HTTPS:
			return Mu_OpensslWrite(fd, buf, len);
		default:
			Mu_ErrorPrint();
			break;
	}

	return MUOK;
}

int Mu_SocketWrite(int fd, char *buf, int len)
{
	int res = 0;
	
printf("write to socket!, the length is %d\n", len);
	while(len){
		
		do
			res = write(fd, buf, len);
		while(res == -1 && errno == EINTR);

		if(res <= 0){
			printf("error happened!\n");
			return res;
		}
		
		buf += res;
		len -= res;
printf("write to socket!, the length is %d\n", len);
	}

printf("write to socket have over!, the length is %d have been write to\n", res);
	return res;
}


int Mu_OpensslWrite(int fd, char *buf, int len)
{
	int res;
	SSL *ssl = NULL;
	
	Mu_FindSsl(ssl, fd);
	if(NULL == ssl)
		return MUESSL;

	while(len){

		do
			res = SSL_write(ssl, buf, len);
		while(res == -1
			&& SSL_get_error(ssl, res) == SSL_ERROR_SYSCALL
			&& errno == EINTR);

		if(res <= 0)
			return res;

		buf += res;
		len -= res;
	}

	return res;
}
/************************************************************************
Description:
		This Function Parse the URL of server, t the IP address
		connect to server, If the connection must use SSL, Function 
		will call Routines of Openssl to do that after the connection 
		is built
		These function is a nonblock connect
Input:
		socket: the sokcet can be used to connect to server
		host:	name of the server or the IP address
		port:	port number
		timeout: for select
Output:
		the statuse of functio run
*************************************************************************/

int Mu_Connecter(int *socketfd, const char *host, int port, double timeout)
{
	int ret = 0;
	struct sockaddr_in server;
	struct hostent hostbuf, *hp;
	int herr;

	char *temphost = NULL;
	int temphostlen = 1024;

	//check the parameters
	if(!port){
		switch(MuIO.type){
			case MU_HTTP:
				port = 80;
				break;
			case MU_HTTPS:
				port = 443;
		                break;
			default:
				Mu_ErrorPrint();
				return MUNPRT;
		}
	}

	if(NULL == host){
		Mu_ErrorPrint();
		return MUNHST;
	}

	bzero((void *)&server, sizeof(server));

	//get the host name
	//first get the addr of servers, detect the addr is a IP address
	//if inet_pton is error, then get the server though DNS
	if(1 != inet_pton(AF_INET, host, &(server.sin_addr))){
		
		if(NULL == (temphost = malloc(temphostlen))){
			Mu_ErrorPrint();
			return MUEERO;
		}

		memset(temphost, 0, temphostlen);
#if 1
printf("the host is :%s\n", host);
		while((ret = gethostbyname_r(host, &hostbuf, temphost, temphostlen, &hp, &herr)) && (errno == ERANGE)){
			temphostlen *= 2;
			if(NULL == (temphost = realloc(temphost, temphostlen))){
				Mu_ErrorPrint();
				Mu_Free(temphost);
				return MUNBUF;
			}

			memset(temphost, 0, temphostlen);
		}

		if(ret){
			Mu_Free(temphost);
			Mu_ErrorPrint();
			return MUECNNT;
		}
		memcpy((void *)&server.sin_addr, hp->h_addr, hp->h_length);
printf("the host {%s} 's IP addr is:%s\n", host, inet_ntoa(*(struct in_addr*)hp->h_addr_list[0]));
#else
		if((hp = gethostbyname(host)) == 0){
			Mu_ErrorPrint();
			Mu_Free(temphost);
			return MUECNNT;
		}
		server.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;

#endif

		//free the temp buffer
		Mu_Free(temphost);
	}

fprintf(stdout, "the port is %d\n", port);

	server.sin_family = AF_INET;
//	server.sin_family = hp->h_addrtype;
	server.sin_port = htons(port);

	//create socket
	if((*socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 1){
		Mu_ErrorPrint();
		return MUNSKT;
	}
fprintf(stdout, "the socket is %d\n", *socketfd);

	//connect to server USE socket
	ret = Mu_SocketConnect(*socketfd, server, host, port, timeout);
	if(ret < 0)
		return ret;

	//connect USE openssl
	if(MuIO.type == MU_HTTPS)
		ret = Mu_OpensslConnect(*socketfd);

	return ret;
}

		
int Mu_SocketConnect(int socketfd, struct sockaddr_in server, const char *host, int port, double timeout)
{
	int flags, block = 0;
	int statuse;
	fd_set wset;
	struct timeval tv;
	//struct sockaddr_in server;

	//chan to noblock IO
	flags = fcntl(socketfd, F_GETFL, 0);
	if(flags != -1)
		block = fcntl(socketfd, F_SETFL, flags|O_NONBLOCK);
	else{
		fprintf(stdout, "[connect] cant' set the socket to noblock!\n");
		block = -1;
	}

	//connect to socket , when connect to failure, return a error
	statuse = connect(socketfd, (struct sockaddr *)&server, sizeof(server));

	if(statuse == -1 && block != -1 && errno == EINPROGRESS){
		FD_ZERO(&wset);
		FD_SET(socketfd, &wset);
		//wset = rset;

		tv.tv_sec = (long)timeout;
		tv.tv_usec = 1000000*(timeout - (long)timeout);

		statuse = select((socketfd + 1), NULL, &wset, NULL, &tv);
		if(statuse > 0){
			socklen_t arglen = sizeof(statuse);
			//int arglen = sizeof(statuse);

			if(getsockopt(socketfd, SOL_SOCKET, SO_ERROR, (void *)&statuse, &arglen) < 0)
				statuse = errno;

			if(statuse)
				errno = statuse, statuse = -1;
			if(errno == EINPROGRESS)
				errno = ETIMEDOUT;
		}else if(statuse == 0){
			Mu_Close(socketfd);
			errno = ETIMEDOUT;
			statuse = -1;
		}
	}

	if(statuse < 0){
		Mu_Close(socketfd);
		
		if(errno == ECONNREFUSED){
			fprintf(stdout, "[connect] error: ECONNREFUSED!\n");
			return MUEREFUSED;
		}
		else{
			fprintf(stdout, "[connect] error: NOT ECONNREFUSED!\n");
			return MUECNNT;
		}
	}else{
		flags = fcntl(socketfd, F_GETFL, 0);
		if(flags != -1){
			fcntl(socketfd, F_SETFL, flags&~O_NONBLOCK);
		}
	}

    printf("Connect OK!\n");
	return MUOK;
}


int Mu_OpensslConnect(int socket)
{
	SSL *conn;
	int i = 0;

	//the ssl_ctx is a global value
	if(ssl_ctx == NULL)
		goto error1;

	conn = SSL_new(ssl_ctx);
	if(NULL == conn)
		goto error1;

	if(!SSL_set_fd(conn, socket))
		goto error1;
	
	SSL_set_connect_state(conn);
	if(SSL_connect(conn) <= 0 
		|| conn->state != SSL_ST_OK){
		Mu_ErrorPrint();
		goto error1;
	}

	//record the SSL into mussltab
	while(i < MAX_THD && mussltab[i].fd != -1)
		i ++;
	if(i == MAX_THD)
		goto error2;

	mussltab[i].fd = socket;
	mussltab[i].ssl = conn;		//the conn is local value, but store it .........
					//because the ssl is malloc to space to store by Openssl
	
	return MUOK;

error2:
	SSL_shutdown(conn);
error1:
	SSL_free(conn);
	return MUESSL;
}

/******************************************************
Description:
		This Function Use to read bytes from the socket
		But these function doesn't fetch the bytes from
		receive buffer
Input:
		fd: the socket of read
		buf: space to stroed bytes received form server
		bufsize: the number of space
Output:
		number of bytes received from server
********************************************************/
int Mu_Peeker(int fd, char *buf, int bufsize)
{
    if(!(MuIO.selecter(fd, WAIT_FOR_READ, 30)))
       return -1;
 
    switch(MuIO.type){
        case MU_HTTP:
            return Mu_SocketPeeker(fd, buf, bufsize);
        case MU_HTTPS:
            return Mu_OpensslPeeker(fd, buf, bufsize);
        default:
		Mu_ErrorPrint();
            break;
    }
 
    return MUOK;
}

int Mu_SocketPeeker(int fd, char *buf, int bufsize)
{
	int res;
	
	do
		res = recv(fd, buf, bufsize, MSG_PEEK);
	while(res == -1 && errno == EINTR);

	return res;
}

int Mu_OpensslPeeker(int fd, char *buf, int bufsize)
{
    int res;
	SSL *ssl;
 
    Mu_FindSsl(ssl, fd);
    if(NULL == ssl)
        return MUESSL;

	do
		res = SSL_peek(ssl, buf, bufsize);
	while(res == -1
		&& SSL_get_error(ssl, res) == SSL_ERROR_SYSCALL
		&& errno == EINTR);

	return res;
}

/*******************************************************
Description:
		This Function use to close socket 
		If the socket USE SSL, It will call the functions of
		openssl to close the sokcet
Input:
		the scoket
Output:
		MUOK
********************************************************/
int Mu_Closer(int fd)
{
    switch(MuIO.type){
        case MU_HTTP:
            return Mu_SocketCloser(fd);
        case MU_HTTPS:
            return Mu_OpensslCloser(fd);
        default:
		Mu_ErrorPrint();
            break;
    }
 
    return MUOK;
} 
	
int Mu_SocketCloser(int fd)
{
	close(fd);
	return MUOK;
}

int Mu_OpensslCloser(int fd)
{
	SSL *ssl = NULL;
	int i = 0;

	Mu_FindSsl(ssl, fd);
	if(NULL == ssl){
		Mu_ErrorPrint();
		goto error;
	}

	SSL_shutdown(ssl);
	SSL_free(ssl);

	while(i < MAX_THD && mussltab[i].fd == fd)
		i ++;

	if(i == MAX_THD) goto error;

	mussltab[i].fd = -1;
	mussltab[i].ssl = NULL;
	
	return MUOK;
	
error:
	return MUESSL;
}

	

int Mu_InitNetIO(MuNetIOPtr IO)
{
	//should contain the init of SSL_CTX
	IO->type = MU_NONE;
	IO->reader = (mu_reader)Mu_Reader;
	IO->writer = (mu_writer)Mu_Writer;
	IO->connecter = (mu_connecter)Mu_Connecter;
	IO->poller = NULL;
	IO->selecter = (mu_selecter)Mu_Selecter;
	IO->closer = (mu_closer)Mu_Closer;
	IO->peeker = (mu_peeker)Mu_Peeker;

	return MUOK;
}
/*****************************************************************
Description:
		init the SSL_CTX, use for https
Input:
		NONE
Output:
		MUOK: no error happened
		MUESSL: ssl error
LOCK:
		NONE
******************************************************************/
int Mu_SslInit (){
	SSL_METHOD *meth;

	if (ssl_ctx){
		Mu_ErrorPrint();
		return MUESSL;
	}

	//Init the PRNG. If that fails, bail out. 
	RAND_seed(rand_sed, sizeof(rand_sed));
	if (RAND_status () != 1){
		fprintf (stdout,"Could not seed PRNG; consider using --random-file.\n");
		goto error;
	}

	SSL_library_init ();
	SSL_load_error_strings ();
	SSLeay_add_all_algorithms ();
	SSLeay_add_ssl_algorithms ();
	
# if defined (SSLVER2)
	meth = SSLv2_client_method ();
#elif defined (SSLVER3)
	meth = SSLv3_client_method ();
#elif defined (SSLVER1)
	meth = TLSv1_client_method ();
#elif defined (SSLVER23)
	meth = SSLv23_client_method ();
#else
	abort();
#endif

	ssl_ctx = SSL_CTX_new(meth);
	if (!ssl_ctx){
		Mu_ErrorPrint();
		goto error;
	}

	Mu_CheckCA();
	return MUOK;
	
error:
	if (ssl_ctx)
		SSL_CTX_free(ssl_ctx);
	return MUESSL;
}

int Mu_CheckCA()
{
	return MUOK;
}

void  Mu_InitSslTab(int num)
{
	int i = 0;
	while(i < num){
		mussltab[i].fd = -1;
		mussltab[i].ssl = NULL;
		i ++;
	}

	return;
}


