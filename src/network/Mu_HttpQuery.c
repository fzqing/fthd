/*******************************************************************
* 					====Microunit Techonogy Co,.LTD.====
* File Name:
* 		Mu_HttpQuery.c
* Description:
* 		This file contain the method of query the URL use the  
*		method GET, HEAD, POST
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

#include "../include/Mu_HttpQuery.h"

/*******************************************************************
Description:
		read form the xmlfd for signal, which we will post to server
Input:
		socket: we will read from this socket
		xmlfd: read from the xmlfd for signal
Output:
		MUOK:no error
		MUEPST: get the xmlfd length error
		MuESND: send to server error
Lock:
		NONE
********************************************************************/
int Mu_PostSignal(int socket, int xmlfd)
{
	
	char send[2048];
	int readlen = 0;
	off_t totallen;
	struct stat info;
	memset(send, 0, 2048);
	
    //Send the boundary to server, which includeing the ----------------7d0123456789a
	if( MuIO.writer(socket, QUERY_BOUNDARY_END, strlen(QUERY_BOUNDARY_END)) < 0){
		Mu_ErrorPrint();
		return MUESND;
	}

#ifdef DEBUG
printf("%s", QUERY_BOUNDARY);
#endif

	if( MuIO.writer(socket, QUERY_CONTENT, strlen(QUERY_CONTENT)) < 0){
		Mu_ErrorPrint();
		return MUESND;
	}

#ifdef DEBUG
printf("%s", QUERY_CONTENT);
#endif

	//get the length of the file, which will be post to the server
	if( (!xmlfd) ||(fstat(xmlfd, &info) == -1)){
		Mu_ErrorPrint();
		return MUEPST;
	}

	totallen = info.st_size;
	//read the contents from the file, write to the socket to server
	while((totallen)&& (readlen = read( xmlfd, send, sizeof(send))) > 0){
		printf("%s", send);
		if(MuIO.writer(socket, send, readlen)<0){
			Mu_ErrorPrint();
			return MUESND;
		}
		totallen -= readlen;
		readlen = 0;
		memset(send, 0, 2048);
	}

    //send the end of the file to server
	if( MuIO.writer(socket, "\r\n", 2) < 0){
		Mu_ErrorPrint();
		return MUESND;
	}
	
	if( MuIO.writer(socket, QUERY_BOUNDARY_END, strlen(QUERY_BOUNDARY_END)) < 0){
		Mu_ErrorPrint();
		return MUESND;
	}

#ifdef DEBUG
printf("%s", QUERY_BOUNDARY_END);
#endif

    //send the end of packet to server
	if( MuIO.writer(socket, "\r\n", 2) < 0){
		Mu_ErrorPrint();
		return MUESND;
	}
	
	return MUOK;
}


/********************************************************************
Description:
		create the socket to send query, and receive the return value form server
		parse thise
Input:
		Ptr: DnStatPtr
Output:
		MUOK: no error
		MUECON: connect error
		MUERLC: reloction
		MuEQUE: error happened when query
Lock:
		NONE
********************************************************************/
int Mu_Query(Mu_DnStatPtr Ptr){
	char *query = NULL;
	int socket;
	char buffer[4096];
	char *ops;
	int ret;
	int len;
	int count;
	int requerytime = 0;
	int writelen;
	Mu_Build build;
	
requery:
	ops = buffer;
	len = 0;
	count = 0;
	writelen = 0;
	Mu_InitHttpStat(&hs[0]);
	
	//malloc space to store info, 
	//all members must be init
	gettimeofday(&(hs[0].starttime), NULL);
	
	//connect to server, got the socket;
	while(1){
		if(MuIO.connecter(&socket, mu_url.host, mu_url.port, 30) < 0){
			if(count++ == 10){
				Mu_ErrorPrint();
				return MUECON;
			}
			fprintf(stdout, "cann't connect to sever! will reconnect\n");
			sleep(30);
		}else{
			fprintf(stdout, "connect SUCCESSFUL!\n");
			hs[0].threadstatuse = MU_DNCNT;
			break;
		}
	}

	//create BUILD structure;
	Mu_InitBuild(&build);
	build.method = Ptr->type;
	build.fd = Ptr->xmlfd;

	if((ret = Mu_BuildQuery(build, &query)) < 0){
		Mu_ErrorPrint();
		goto error;
	}


printf("*****************\n");
	//do build error
	if(MuIO.writer(socket, query, Mu_Strlen(query)) < 0){
		Mu_ErrorPrint();
		goto error;
	}
	
	Mu_Free(query);
	if(Ptr->type == MU_POST){
		if((ret = Mu_PostSignal(socket, Ptr->xmlfd)) < 0){
			Mu_ErrorPrint();
			goto error;
		}
	}

	if((ret = Mu_ParseHeader(socket, &hs[0])) < 0){
		Mu_ErrorPrint();
		goto error;
	}

	if(CHECK_STATRE(hs[0].statcode)){
		Mu_ErrorPrint();
		return MUERLC;
	}

	if(!CHECK_STATOK(hs[0].statcode)){
		Mu_ErrorPrint();
		goto error;
	}

	len = hs[0].bytesleft;
	memset(buffer, 0, 4096);
	ftruncate(Ptr->fd, 0);
	lseek(Ptr->fd, 0, SEEK_SET);
	
	while(len){
		if((ret = MuIO.reader(socket, buffer, 4095)) < 0){
			Mu_ErrorPrint();
			goto error;
		}

		len -= ret;

#ifdef DEBUG
printf("%s", buffer);
#endif

		//write to file
		writelen = ret;
		ops = buffer;
		while(writelen){
			if((ret = write(Ptr->fd, ops, writelen)) < 0){
				Mu_ErrorPrint();
				goto error;
			}
			ops += ret;
			writelen -= ret;
		}
		memset(buffer, 0, 4096);
	}

	//write(Ptr->fd, ops, 1);
	Mu_Close(socket);
	return MUOK;

error:
	if(ret == MUNBUF){
		Mu_ErrorPrint();
		return ret;
	}

	requerytime ++;
	if(requerytime == 10){
		Mu_ErrorPrint();
		return MUEQUE;
	}
		
	lseek(Ptr->fd, 0, SEEK_SET);
		
	Mu_Close(socket);
	Mu_Free(query);		
	goto requery;
}

