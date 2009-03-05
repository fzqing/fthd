/*******************************************************************
* 					====Microunit Techonogy Co,.LTD.====
* File Name:
*
* 		MuFTAD_Init.c
*
* Description:
*
* 		This file contain the method of Init the date and Device
*		Information, Use it ,we can send query and download 
*		from server
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
#include <errno.h>
#include <sys/statfs.h>
#include <pthread.h>
#include <openssl/ssl.h>

#include <openssl/err.h>
#include "include/main.h"
#include "include/Mu_NetIO.h"
#include "include/Mu_error.h"
#include "include/MuFTAD_Init.h"
#include "include/Mu_Struct_main.h"



/*******************************************************************
Description:
		Init the datas, which will be used in the programm. 
Input:
		NONE
Output:
		MUOK: no error happened
		< 0 : error
LOCK:
		NONE
*********************************************************************/
int MuFTAD_DateInit(void)
{
    	int i;
	ssl_ctx = NULL;
	
	pthread_mutex_init(&lock, NULL); //for multi-thread
	Mu_SslInit();		//for SSL
	Mu_InitURL(&mu_url);
	
	if((Mu_FddfStatusHeadCreate(&MuFddfStatHead) < 0)
		||(Mu_DirStatusHeadCreate(&MuDirStatHead) < 0)
		||(Mu_ServiceInfoCreate(&MuServiceInfo) < 0)
		||(Mu_FddfListHeadCreate(&MuFddfListHead) < 0)
		||(Mu_FddfFileCreate(&MuFddfFile) < 0)
		||(Mu_FileInfoCreate(&MuFileInfo) < 0)
		||(Mu_SegmentHeadCreate(&MuSegmentHead) < 0)
		||(Mu_EyewearADPCreate(&MuEyewearADPrcv) < 0)
		||(Mu_EyewearADPCreate(&MuEyewearADPsnd) < 0)
		||(Mu_XmlDeviceInfoCreate(&MuDeviceInfo) < 0)
		||(Mu_DBIInfoPtrCreate(&MuDBIInfo) < 0)
		||(Mu_DBUDirtPtrCreate(&MuDBUDirt) < 0)
		||(Mu_DBUInfoPtrCreate(&MuDBUInfo) <0)
		||(Mu_DBURatePtrCreate(&MuDBURate) < 0)){
		
		Mu_ErrorPrint();
		return MUNBUF;
	}

	/*init the array of the resumeinfo*/
	for(i = 0; i < 10; i ++){
		if(Mu_ResumeInfoInit(MuResume, i) < 0){
			Mu_ErrorPrint();
			return MUEERO;
		}
	}
		
	/*Init the IO functions of Network*/
	if(Mu_InitNetIO(&MuIO) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}

	/*Build the fd for to receive the contents from server*/
	if(((rcv_fd = open("rcv", O_RDWR|O_CREAT, 0644)) < 0)
		||((snd_fd = open("snd", O_RDWR|O_CREAT, 0644)) < 0)
		||((stat_fd = open("stat", O_RDWR|O_CREAT, 0644)) < 0)
		||((list_fd = open("list", O_RDWR|O_CREAT, 0644)) < 0)){
		
		Mu_ErrorPrint();
		return MUEERO;
	}

#if 0
	/*when close the file or power down, we need to delete the file automatic*/
	unlink("rcv");
	unlink("snd");
	unlink("stat");
	unlink("list");
#endif

	return MUOK;
}



/******************************************************************
Description:
		read the service info.xml and deviceinfo.xml, get the value of
		MuServiceInfo and MuDeviceInfo
Input:
		serverinfo: serviceinfo.xml
		deviceinfo: deviceinfo.xml
Output:
		MUOK: no error
		MUENSP: no space to store program
		MUEFSY: file system error
		MUNXML: can't find the xml
LOCK:
		NONE
*******************************************************************/
int MuFTAD_DeviceInit(const char *serverinfo, const char *deviceinfo)
{
	struct statfs fs_info;
	int space_rate;
	int fd = 0;
	
	//check the space of the disk
	if(statfs("/", &fs_info) != 0){
		Mu_ErrorPrint();
		return MUEFSY;
	}

	if((space_rate = 
		(int)(100*((double)fs_info.f_bavail/(double)fs_info.f_blocks))) < 2){
		
		Mu_ErrorPrint();
		return MUENSP;
	}

	fprintf(stdout, "there are %dPercent left for downloading!\n", space_rate);
	
	//parse the server information XML
	if((fd = open(serverinfo, O_RDONLY)) < 0
		||(Mu_Parse(PARSE_SERVICE_INFO, fd, NULL, &MuServiceInfo) < 0)){
		
		Mu_ErrorPrint();
		Mu_Close(fd);
		return MUNXML;
	}

	Mu_Close(fd);

	//parse the device information
	if(((fd = open(deviceinfo, O_RDONLY)) < 0)
		||(Mu_Parse(PARSE_DEVICE_INFO, fd, NULL, &MuDeviceInfo) < 0)){
		Mu_ErrorPrint();
		Mu_Close(fd);
		return MUNXML;
	}

	Mu_Close(fd);
	return MUOK;
}


void MuFTAD_FreeSource(void)
{
	if (ssl_ctx){
		SSL_CTX_free(ssl_ctx);
		ssl_ctx = NULL;
	}

	Mu_FddfStatusHeadFree(MuFddfStatHead);
	Mu_DirStatusHeadFree(MuDirStatHead);
	Mu_ServiceInfoFree(MuServiceInfo);
	Mu_FddfListHeadFree(MuFddfListHead);
	Mu_FddfFileFree(MuFddfFile);
	Mu_SegmentHeadFree(MuSegmentHead);
	Mu_EyewearADPFree(MuEyewearADPrcv);
	Mu_EyewearADPFree(MuEyewearADPsnd);
	Mu_XmlDeviceInfoFree(MuDeviceInfo);
	
	Mu_Free(MuDBIInfo);
	Mu_Free(MuDBUInfo);
	Mu_Free(MuDBUDirt);
	Mu_Free(MuDBURate);

	Mu_FreeURL(mu_url);

	return;

}
		
