/*******************************************************************
* 					====Microunit Techonogy Co,.LTD.====
* File Name:
* 		Mu_HttpUrl.c
* Description:
* 		This file contain the method of parse the URL to t the 
*		Information about the http or https
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

#include "../include/Mu_HttpUrl.h"

char *check_protoc[] = {"http:", "https:", NULL};

int Mu_ParseUrl(const char *url)
{
	int ret = MUOK;
	
	if(((ret = Mu_CheckProtoc(url)) < 0)
		||((ret = Mu_CheckUser(url) )  < 0)
	 	||((ret = Mu_GetHostPort(url)) < 0)
	 	||((ret = Mu_GetPathFile(url)) < 0)){
		Mu_ErrorPrint();
	}
	
	return ret;
}	


/***************************************************************
Description:
		check the protocol of the URL, Now , there are 
		two types : http and https
Input:
		url: the string of URL
Output:
		MUOK: no error
		MUEPRO: error protocol
Lock:
		NONE
***************************************************************/
int Mu_CheckProtoc(const char *url)
{
	char **s;
	int i = 0;
	
	for(s = check_protoc; *s; s++, i++){
		if(strncasecmp(url, *s, Mu_Strlen(*s)) == 0){
			switch(i){
				case 0:
					mu_url.scheme = URL_HTTP;
					MuIO.type = MU_HTTP;
#ifdef DEBUG
					printf("mu_url.scheme = HTTP\n");	
#endif
					break;

				case 1:
					mu_url.scheme = URL_HTTPS;
					MuIO.type = MU_HTTPS;
#ifdef DEBUG
					printf("mu_url.scheme = HTTPS\n");
#endif
					break;

				default:
					printf("mu_url.scheme is error!\n");
					return MUNPRO;	//not https/http,then return
					//break;
			}
			return MUOK;
		}
	}
	return MUEPRO;
}

/**********************************************************************
Description:
		t the username & password from URL.
		eg. URL:http://username:password@srver.com:port/pathname/filename
		username is: username
		password is :password
Input:
		Url: the string contain the URL
Output:
		MUOK: no error
		MuEPRO: error protocol
		MuNBUF: no buffer to user
		MUNFIL: can't t file
Lock:
		NONE
***********************************************************************/
int Mu_CheckUser(const char *url)
{
	char *p = NULL;
	char *q = NULL;
	char *s = NULL;
	
	//clear the buffer
	//if no username and password, make sure the length of buffer is ZERO;
	
	//jud whether contain the username & password	
	if(NULL != (p = strstr(url, "//"))){
		p += 2;
		
		//if have no username and password,
		//return MUOK, but the buffers are empty
		if(NULL == (q = strchr(p,'@'))){
			return MUOK;
		}
		
		//there are username and passwrod
		if((NULL != (s = strchr(p,':')))&&(s < q)&&( p != s)){					
			//stored the username
			if(s > (p+1)){
				if((s-p) >= mu_url.usernamelen){	
					if(NULL == mu_url.username){
						if(NULL == (mu_url.username = malloc(s-p+1))){
							Mu_ErrorPrint();
							return MUNBUF;
						}
					}
					else if(NULL == realloc(mu_url.username,(s-p+1))){
						Mu_ErrorPrint();
						return MUNBUF;
					}
					
					mu_url.usernamelen = s-p+1;
				}
				snprintf(mu_url.username, s-p+1, "%s", p);
				printf("usename = %s\n",mu_url.username);	
			}
		
			//store the passwrod
			if(q > (s + 1)){
				if((q-s-1) >= mu_url.passwordlen){
					if(NULL == mu_url.password){
						if(NULL == (mu_url.password = malloc(q-s))){
							Mu_ErrorPrint();
							return MUNBUF;
						}
					}
					else if(NULL == realloc(mu_url.password, (q-s))){
						Mu_ErrorPrint();
						return MUNBUF;
					}
					mu_url.passwordlen = q-s;
				}
				snprintf(mu_url.password, q-s, "%s", s+1);
				printf("password = %s\n",mu_url.password);	
			}
#ifdef DEBUG
			printf("Mu_CheckUser ==> successfully\n");
#endif
			return MUOK;
		}else if(s == NULL){
			//there are username, but not password
			if(q > (p + 1)){
				if((q-p) >= mu_url.usernamelen){
					if(NULL == mu_url.username){
						if(NULL == (mu_url.username = malloc(q-p+1))){
							Mu_ErrorPrint();
							return MUNBUF;
						}
					}
					else if(NULL == realloc(mu_url.username, (q-p+1))){
						Mu_ErrorPrint();
						return MUNBUF;
					}
					
					mu_url.usernamelen = q-p+1;
				}
				
				snprintf(mu_url.username, q-p+1, "%s", p);
				printf("usename = %s\n",mu_url.username);	
			}
#ifdef DEBUG
			printf("Mu_CheckUser ==> successfully\n");		
#endif
			return MUOK;
		}
		else
			return MUEPRO;
	}
	else
		return MUEPRO;
}

/**********************************************************************
Description:
		t the servername & port from URL.
		eg. URL:http://username:password@srver.com:port/pathname/filename
		host is: server
		port is :port
Input:
		Url: the string contain the URL
Output:
		MUOK: no error
		MuEPRO: error protocol
		MuNBUF: no buffer to user
		MUNFIL: can't t file
Lock:
		NONE
***********************************************************************/
int Mu_GetHostPort(const char *url)
{
	char *p = NULL;
	char *q = NULL;
	char *s = NULL;
	char count[6]; //use to store port
	//int i = 0;
	memset(count, 0, 6);
	//memset(mu_url.host, 0, mu_url.hostlen);
	mu_url.port = 0;
	
	if(NULL != (p = strstr(url, "//"))){
		//have the host segments
		if((p += 2) &&(NULL != (q = strchr(p,'/'))) && ((q - p) != 1)){

			//skip the username and password
			if((NULL != (s = strchr(p,'@')))&& (s < q-1)){
#ifdef DEBUG
				fprintf(stdout, "[Mu_GetHostPort]skip the username&password to got hostname\n");
#endif
				p = s + 1;
			}
			else if(NULL == s)
				fprintf(stdout, "[Mu_GetHostPort]have no username and password\n");
			else if(s >= q){
				fprintf(stdout, "[Mu_GetHostPort]usename have the char /, U should omit it!\n");
				return MUEPRO;
			}
			
			//t ride of the port & hostname
			if((NULL != (s = strchr(p,':')))&&(s < q)){			
				if( s > (p+1)) {
					if((s-p) >= mu_url.hostlen){
						if(NULL == mu_url.host){
							if(NULL == (mu_url.host = malloc(s-p+1))){
								Mu_ErrorPrint();
								return MUNBUF;
							}
						}
						else if(NULL == realloc(mu_url.host, s-p+1)){
							Mu_ErrorPrint();
							return MUNBUF;
						}
						
						//bzero
						mu_url.hostlen = s-p+1;
					}
					snprintf(mu_url.host, s-p+1, "%s", p);
					printf("mu_url.host = %s\n",mu_url.host);
				}
				
				//anaylise the port
				if(q > (s + 1)){
					if((q-s-1) <= 6){
						snprintf(count, q-s, "%s", s+1);
						mu_url.port = atoi(count);
						printf("mu_url.port = %d\n",mu_url.port);
						return MUOK;
					}
				}
			}
			else if(s == NULL){
				if(q > ( p + 1)){
					if((q-p) >= mu_url.hostlen){
						if(NULL == mu_url.host){
							if(NULL == (mu_url.host = malloc(q-p+1))){
								Mu_ErrorPrint();
								return MUNBUF;
							}
						}
						else if(NULL == realloc(mu_url.host, q-p+1))
							return MUNBUF;
						
						//bezero
						mu_url.hostlen = q-p+1;
					}
					snprintf(mu_url.host, q-p+1, "%s", p);
					printf("host is = %s\n",mu_url.host);	
				}
			}
			else
				return MUEPRO;
					
			return MUOK;
		}
		else
			return MUNHST;
	}
	else
		return MUEPRO;
}


/**********************************************************************
Description:
		t the path & file from URL.
		eg. URL:http://username:password@srver.com/pathname/filename
		path is: pathname
		file is :filename
Input:
		Url: the string contain the URL
Output:
		MUOK: no error
		MuEPRO: error protocol
		MuNBUF: no buffer to user
		MUNFIL: can't t file
Lock:
		NONE
***********************************************************************/
int Mu_GetPathFile(const char *url)
{
	char *p = NULL;
	char *q = NULL;
	char *s = NULL;

	//clear the buffer of path&file
#ifdef DEBUG
printf("[Mu_GetPathFile] the url is: %s\n", url);
#endif
	
	//skip the "//"
	if((NULL == (s = strstr(url, "//"))) || !(*(s += 2)))
		return MUEPRO;
	
	//t path
	if((NULL != (p = strchr(s,'/'))) && (NULL != (q = strrchr(s,'/')))){
		//have path
		//if there is NOT path, we should skip and DO NOT return error!
		if(q > (p + 1)){
			if((q-p-1) >= mu_url.pathlen){
				if(NULL == mu_url.path){
					if(NULL == (mu_url.path = malloc(q-p))){
						Mu_ErrorPrint();
						return MUNBUF;
					}
				}
				else if(NULL == realloc(mu_url.path, q-p)){
					Mu_ErrorPrint();
					return MUNBUF;
				}
				
				//bezero
				mu_url.pathlen = q-p;
			}
			
			snprintf(mu_url.path, q-p, "%s", p+1);
			printf("mu_url.path = %s\n",mu_url.path);
		}
		
		//t filename
		if(q-url+1 != Mu_Strlen(url)){
			if(Mu_Strlen(q+1) >= mu_url.filelen){
				if(NULL == mu_url.file){
					if(NULL == (mu_url.file = malloc(Mu_Strlen(q+1)+1))){
						Mu_ErrorPrint();
						return MUNBUF;
					}
				}
				else if(NULL == realloc(mu_url.file, Mu_Strlen(q+1)+1)){
					Mu_ErrorPrint();
					return MUNBUF;
				}
				mu_url.filelen = Mu_Strlen(q+1)+1;
			}		
			snprintf(mu_url.file, Mu_Strlen(q+1)+1, "%s", q+1);
			printf("mu_url.file= %s\n",mu_url.file);
			return MUOK;
		}
		else{
			Mu_ErrorPrint();
			return MUNFIL;
		}
	}
	else{
		Mu_ErrorPrint();
		return MUEPRO;
	}
}




