/***************************************************************************
* 					====Microunit Techonogy Co,.LTD.====
* File Name:
* 		Mu_HttpDown.c
* Description:
* 		This file contain the method of dwonlaod the files from server, use multi-threads 
* Revision History:
* 		14-5-2008 ver1.0
*Author:
* 		ssw (fzqing@gmail.com), cks, wkx
* 					***PROTECTED BY COPYRIGHT***
***************************************************************************/
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

//application head files
#include "../include/Mu_HttpDown.h"


int Mu_GetLoop(Mu_DnStatPtr Ptr)
{
	int socket;
	int ret;
	int i = 0;
	char *query = NULL;
	int count = 0;
	Mu_Build build ;	
	int tempfd = 0;
	char tempname[256];
	snprintf(tempname,256,"%s%s", filename, ".jw");
	
	//if resume, we must read reusm info from file
	if(Ptr->resume){
		if(Mu_ReadFromRec(tempname) < 0){
			printf("error happend when read from record file\n");
			sleep(5);
			goto newdownload;
		}

		printf("the system will resume the download file!\n");
		sleep(10);
	}
	else{
		//if we don't know the length of the file, we need to query server use HEAD method
		//to get the length of file for init the thread
		if(Ptr->length == 0){
			
newdownload:

			//get the length from server
			Mu_InitHttpStat(&(hs[0]));
			
			//connect to server, got the socket;
			while(1){
				if(MuIO.connecter(&socket, mu_url.host,  mu_url.port,  30) < 0){
					if(count++ == 10)
						return MUECNNT;
					
					fprintf(stdout,"cann't connect to sever! will reconnect¡­\n");
					sleep(30);
				}else{
					fprintf(stdout, "connect SUCCESSFUL!\n");
					break;
				}
			}
			//create BUILD structure;
			Mu_InitBuild(&build);
			build.method = MU_HEAD;
			//build.method = MU_GET;
			if((ret = Mu_BuildQuery(build, &query))< 0){
				Mu_ErrorPrint();
				return ret ;
			}
			
			//do build error free(query)
			if(MuIO.writer(socket, query, Mu_Strlen(query)) < 0){
				Mu_ErrorPrint();
				Mu_Free(query);
				Mu_Close(socket);
				return MUESND;
			}
			Mu_Free(query);
printf("after send the query to server\n");
			
			if((ret = Mu_ParseHeader(socket, &hs[0]) < 0)){
				Mu_Close(socket);
				return ret;
			}
			
			Mu_Close(socket);
			Ptr->length = hs[0].bytesleft;
		}
		
		if((ret = Mu_InitThreads(Ptr->length)) <0){
			Mu_ErrorPrint();
			return ret;
		}
		
		//open the file which to recording into
		if((tempfd = open(tempname, O_RDWR|O_CREAT)) < 0){
			Mu_ErrorPrint();
			goto error;
		}

		//copy the startops to sturture HttpStat
		//It will be used to seek
printf("the thread num is %d\n", threadnum);
		for(i = 0; i < threadnum; i ++){
			hs[i].tempfd = tempfd;
			hs[i].fd = Ptr->fd;
			hs[i].seekstart = Ptr->startops;
		}
	}

	//create the threads to download file
	//pthread_mutex_lock(&lock);
	for(i = 0; i < threadnum; i ++)
		pthread_create(&(hs[i].threadnumber),NULL, (void*)Mu_Download, (void*)&(hs[i]));
//	pthread_mutex_unlock(&lock);

	//wait threads to cancell
	for(i=0; i< threadnum; i++)
		pthread_join(hs[i].threadnumber, NULL);
	
	//check the file
	for(i = 0; i < threadnum; i ++){
		if(hs[i].statcode == 303)
			return MUERLC;
		
		if(hs[i].threadstatuse != MU_DNCMP)
			goto error;
	}
	Mu_Close(tempfd);
	
	if(unlink(tempname) < 0) {
		Mu_ErrorPrint();
		return MUEDEL;
	}


	return MUOK;

error:
	Mu_Close(tempfd);
	return MUNCMP;
}


/**************************************************************
Description:
		Read recording from filename.jw to get the thread information
InPut:
		name: filename
Ouput:
		MUOK: no error
		MUERED: read from record error
Lock:
		none
***************************************************************/
int Mu_ReadFromRec(const char *name)
{
	int statfd = 0;
	int i;
	int urllen;
	char url[1024];
	//init httpstats
	for(i = 0; i < MAX_THD; i++)
		Mu_InitHttpStat(&(hs[i]));
	
	
	if(access(name, F_OK) == -1){
		Mu_ErrorPrint();
		return MUERED;
	}

	//open the record file	
	if(-1 == (statfd = open(name, O_RDONLY))){
		Mu_ErrorPrint();
		goto error;
	}
	
	if(read(statfd, &urllen, sizeof(int)) < 0){
		Mu_ErrorPrint();
		goto error;
	}
	
	//read the url from the file, and compare the url
	//make sure the address resuming is the same;
	if((read(statfd , url, urllen) <= 0)){
		//||(Mu_Strlen(url) != Mu_Strlen(mu_url.url))
		//||(strncasecmp(url, mu_url.url, Mu_Strlen(url)))){
		Mu_ErrorPrint();
		goto error;
	}
	
	//read the numbers of the threads
	if((read(statfd, &threadnum, sizeof(int)) <= 0)||(threadnum > MAX_THD))	
		goto error;
	
	//read the structure of Mu_HttpStatus
	for(i = 0; i< threadnum; i ++){
		if(read(statfd, &hs[i], sizeof(Mu_HttpStatus)) <= 0)
			goto error;
	}
	
	Mu_Close(statfd);
	return MUOK;
	
error:
	Mu_Close(statfd);
	return MUERED;
}



int Mu_RecToFile(void)
{
	int i;
	int fd;
	int urllen;
	char temp[256];

	snprintf(temp, 256, "%s.jw", filename);
	if((fd = open(temp, O_RDWR|O_CREAT)) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	} 

	ftruncate(fd, 0);
	urllen = Mu_Strlen(mu_url.url);
	
	if(write(fd, &urllen, sizeof(int)) < 0){
		Mu_ErrorPrint();
		goto error;
	}
	
	//write the URL to record file
	if(((write(fd, mu_url.url, Mu_Strlen(mu_url.url))) < 0)
		||((write(fd, &threadnum, sizeof(int))) < 0)){
		Mu_ErrorPrint();
		goto error;
	}
	
	//write the structure to file
	for(i = 0; i< threadnum; i ++){
		if(write(fd, &hs[i], sizeof(Mu_HttpStatus)) < 0)
			goto error;
	}
	
	Mu_Close(fd);
	return MUOK;
	
error:		
	Mu_Close(fd);
	return MUEREC;
}


void Mu_Download(Mu_HttpStatusPtr Ptr)
{	
	int ret = 0;
	int writelen =0;
	int socket = 0;
	char *query = NULL;
	Mu_Build build;
	int boolprintf = 0;
	char buffer[4096];
	char *buffptr = buffer;
	Mu_HttpStatus httpstatus;

	//off_t startops= hs[temp].seeknew; //back up the seeknew
	int revlen;
	int count;
	int redown;
	unsigned long timeuse;
	
	//check the thread wethere it's compelete
	if(Ptr->threadstatuse == MU_DNCMP)
		return;
	
redownload:
	boolprintf = 0;
	revlen = 0;
	count = 0;
	redown = 0;
	
	//init the httpstatuse
	Mu_InitHttpStat(&httpstatus);

	//get the start time of the download
	//use the lock to make sure the synchronization
	gettimeofday(&(Ptr->starttime), NULL);
	
	//connect to server, got the socket;	
	while(1){
		if(MuIO.connecter(&socket, mu_url.host, mu_url.port, 30) < 0){
			if(count++ == 10)
				return;
			
			fprintf(stdout, "[Mu_Download]Can't connect to sever! will reconnect¡­\n");
			sleep(30);
		}else{
			fprintf(stdout,"[Mu_Download]connect SUCCESSFUL!\n");
			//change the statuse of thread
			Ptr->threadstatuse = MU_DNCNT;
			break;
		}
	}
	
	//make the query
	Mu_InitBuild(&build);
	build.method = MU_GET;
	build.startops = Ptr->seeknew;
	build.endops = Ptr->seeknew + Ptr->bytesexcept; //cotent -length: startops/endops

printf("+++++++++++++the thread >%d< the %ld left, %ld start, %ld end...........\n", Ptr->threadnumber,  Ptr->bytesleft, build.startops, build.endops);
	//build the query and then send to server
	if(((ret = Mu_BuildQuery(build, &query)) < 0)
		||(ret = MuIO.writer(socket, query, Mu_Strlen(query)) < 0)
		||((ret = Mu_ParseHeader(socket, &httpstatus) < 0))){
		goto error;
	}
	
	if(CHECK_STATRE(httpstatus.statcode))
		return;
	
	if(!(CHECK_STATOK(httpstatus.statcode)))
		goto error;
	
	Mu_Free(query);
/*	
	if(Ptr->bytesexcept != Ptr->bytesleft)
		return;
*/
	memset(buffer, 0, 4096);
	
	if(-1 == (Ptr->fd = open(filename, O_WRONLY|O_CREAT, 0644)))//0644 is no mode to own
		return;
	
	lseek(Ptr->fd, (Ptr->seekstart + Ptr->seeknew), SEEK_SET);
	Ptr->threadstatuse = MU_DNRCV;
	
	while(Ptr->bytesleft > 0){
		//record counting
		boolprintf ++ ;
		
		if(boolprintf == 50)
			boolprintf = 0;
		
		if((ret= MuIO.reader(socket, buffer, 4095)) < 0)
			goto error;
		
		revlen = ret;
		//write to file
		writelen = ret;
		buffptr = buffer;
printf("the thread ===>%d<=== have write %d bytes into file,the left is %ld\n", Ptr->threadnumber, ret, Ptr->bytesleft);
		while(writelen){
			if((ret = write(Ptr->fd, buffptr, writelen)) < 0)
				goto error_2;
			
	        buffptr += ret;		
			writelen -= ret;
		}

		Ptr->bytesleft -= revlen;
		Ptr->seeknew += revlen;

		//calculate the rate of downloading
		gettimeofday(&(Ptr->dltime), NULL);
		timeuse = Ptr->starttime.tv_usec -Ptr->dltime.tv_usec;
		//hs[temp].rates = (revlen / timeuse)*1000;

		//record to file
		if(boolprintf == 0){
			pthread_mutex_lock(&lock);
			Mu_RecToFile();
			pthread_mutex_unlock(&lock);
		}

		memset(buffer, 0, 4096);
	}
	
	pthread_mutex_lock(&lock);
	Ptr->threadstatuse= MU_DNCMP;
	Mu_RecToFile();
	pthread_mutex_unlock(&lock);

error_2:
	Mu_Close(socket);
	Mu_Close(Ptr->fd);
	Mu_Free(query);
	return;
	
error:
	fprintf(stdout,"[Mu_Download]Redownload the files ¡­.\n");
	redown ++;
	
	//free query buffer
	Mu_Free(query);
	//close fd
	Mu_Close(socket);
	Mu_Close(Ptr->fd);
	
	if(ret == MUNBUF) //make sure thre MUNBUF is not -1
		return;

	if(redown == 10)
		return;
	else{
		pthread_mutex_lock(&lock);
		Ptr->threadstatuse= MU_RECNT;
		pthread_mutex_unlock(&lock);
		goto redownload;
	}
	
	return;
}


/***************************************************
Description:
		This function is the main entry for downloading or querying
		for http or https;
		This function parse the URL, then jump to another functions
		to process;
InPut:
		URL: the location of the file
		Method:
			in clude MU_QUERY & MU_DOWNLOAD
		Ptr: the structure of Mu_DnStatPtr. which including
			the info of download
OUTPU:
		MUOK: no error
		MUEURL: the format of URL is error
		MUEFSY: the file system  is error
		MUENSP: there are no space to download
		MUEMEH: the download method is error

LOCK:
		NONE
********************************************************/
int Mu_HttpPlus(const char *url, int method, Mu_DnStatPtr Ptr)
{
	int ret = MUOK;	
	int space_rate;
	struct statfs fs_info;
	const char *url_back = url;

	//everytime ,we must init the array of the SSLTAB
	Mu_InitSslTab(MAX_THD);
	
printf("the value of MuDnStat.resume is %d , print for the first time\n", Ptr->resume);

retry:
	//parse the URL 
	if((ret = Mu_ParseUrl(url_back)) < 0){
		Mu_ErrorPrint();
		return MUEURL;
	}
	
	switch(method){
		//query
		case MU_QUERY:
			if((ret = Mu_Query(Ptr)) < 0){
				
				//process the relocation
				if(ret == MUERLC){
					url_back = location;	
					goto retry;
				}else
					goto error;
			}
			break;
		
		case MU_DOWNLOAD:
			if((ret = Mu_GetLoop(Ptr)) < 0){
				//process the relocation
				if(ret == MUERLC){
					url_back = location;
					goto retry;
				}else
					goto error;
			}
			break;
			
		default:
			return MUEMEH;
	}
	
error:
	if(statfs("/", &fs_info)!=0){
		Mu_ErrorPrint();
		return MUEFSY;
	}

	if((space_rate =(int)(100 *((double)fs_info.f_bavail / (double)fs_info.f_blocks))) < 2){
		Mu_ErrorPrint();
		return MUENSP;
	}else
		return ret;
}
