/*******************************************************************
* 					====Microunit Techonogy Co,.LTD.====
* File Name:
*
* 		MuFTAD_Util.c
*
* Description:
*
* 		This file contain the method of communication with server
*
* Revision History:
*
* 		7-5-2008 ver1.0
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

#include <openssl/err.h>
#include "include/main.h"

#include "include/MuFTAD_Init.h"
#include "include/MuFTAD_Proc.h"

/*****************************************************
*Description:
		Get the XML file from server, Device build the URL, then 
		query server though GET method,
		after that, device parse the XML get the method of login
Input:
		none
Output:
		return value ,when no error happened
		return MUOK;
		0 No ERROR
		<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 7-5-2008)
******************************************************/
int MuFTAD_PreLogin(void)
{
	char *addr;
	int querylen;
	char *query;
	int ret = MUOK;

	//point to the list of server
	ServerHeadPtr server = MuServiceInfo->server;

	//check the current address, we'll use to query from server
	if(server->cur == NULL){
		Mu_ErrorPrint();
		return MUEERO;
	}

again:
	if(NULL == (addr = server->cur->addr)){
		Mu_ErrorPrint();
		return MUEERO;
	}

	//build the query address
	querylen = snprintf(NULL, 0,
			"https://%s/%s", addr, 
			MuServiceInfo->apipathlist->prelogin);

	querylen++;
	if(NULL == (query = malloc(querylen))){
		Mu_ErrorPrint();
		return MUNBUF;
	}
	snprintf(query, querylen,
			"https://%s/%s", addr, 
			MuServiceInfo->apipathlist->prelogin);


	//Init the MuDnStat
	Mu_InitDnStat(&MuDnStat);
	MuDnStat.type = MU_GET;
	MuDnStat.fd = rcv_fd;

	ret = Mu_HttpPlus(query, MU_QUERY, (Mu_DnStatPtr)(&MuDnStat));
	MuFTAD_Free(query);

	//if connect error, we must reconnect to the server
	if(ret == MUECNNT){
		if(server->cur->next != NULL)
			server->cur = server->cur->next;
		else
			server->cur = server->next;

		goto again;
	}

	//if error happened, shall return
	if(ret != MUOK){
		Mu_ErrorPrint();
		MuFTAD_ErrorProc(ret);
		return MUEERO;
	}

	//parse the information to MuEyewearADPrcv
	if((ret = Mu_Parse(PARSE_EYEWEAR_ADP, rcv_fd,
						NULL, &MuEyewearADPrcv)) < 0){

		Mu_ErrorPrint();
		return ret;
	}

	return MUOK;
}

int MuFTAD_Login(void)
{
	int ret;

	do{
		if((ret = MuFTAD_EyeADPPost(LOGIN)) < 0
			||(ret = MuFTAD_XmlProc(LOGIN)) < 0){

			Mu_ErrorPrint();
			return ret;
		}
	}while(need_retry >= 0);

	//download the update fireware,  if it's exist
	//when download them failed, can't return with error
	if(MuFTAD_DnUpdate() < 0)
		fprintf(stdout, "can't download the update fireware!\n");

	return MUOK;
}


int MuFTAD_UpSRV120(void)
{
	int ret;

	do{
		if((ret = MuFTAD_EyeADPPost(UPSTAT120)) < 0
			||(ret = MuFTAD_XmlProc(UPSTAT120)) < 0){

			Mu_ErrorPrint();
			return MUEERO;
		}
	}while(need_retry >= 0);

	return MUOK;
}

int MuFTAD_UpSRV121(void *Ptr, int type, int suc)
{
	int ret;

	do{
		if((ret = MuFTAD_UpDnPost(Ptr, UPDN121, type, suc)) < 0
			||(ret = MuFTAD_XmlProc(UPDN121)) < 0){

			Mu_ErrorPrint();
			return MUEERO;
		}
	}while(need_retry >= 0);

	return MUOK;
}

int MuFTAD_UpSRV122(void *Ptr, int type, int suc)
{
	int ret;

	do{
		if((ret = MuFTAD_UpDnPost(Ptr, UPFILE122, type, suc)) < 0
			||(ret = MuFTAD_XmlProc(UPFILE122)) < 0){

			Mu_ErrorPrint();
			return MUEERO;
		}
	}while(need_retry >= 0);

	return MUOK;
}


int MuFTAD_Logout101(void)
{
	int ret;

	do{
		if((ret = MuFTAD_EyeADPPost(LOGOUT101)) < 0
			||(ret = MuFTAD_XmlProc(LOGOUT101)) < 0){

			Mu_ErrorPrint();
			return MUEERO;
		}
	}while(need_retry >= 0);

	return MUOK;
}


int MuFTAD_Logout102(void)
{
	int ret;

	do{
		if((ret = MuFTAD_EyeADPPost(LOGOUT102)) < 0
			||(ret = MuFTAD_XmlProc(LOGOUT102)) < 0){

			Mu_ErrorPrint();
			return MUEERO;
		}
	}while(need_retry >= 0);

	return MUOK;
}

int MuFTAD_Query110(void)
{
	int ret;

	do{
		if((ret = MuFTAD_EyeADPPost(QUERY110)) < 0
			||(ret = MuFTAD_XmlProc(QUERY110)) < 0){

			Mu_ErrorPrint();
			return MUEERO;
		}
	}while(need_retry >= 0);

	return MUOK;
}
/*******************************************************
Description:
		according to the contents of a FddfStatusNode, query server
		for the FDDF list;
Input:
		Ptr: FddfStatusNode structure, which contain the members
		 used to query server
Output:
		return value ,when no error happened
		return MUOK;
		0 No ERROR
		<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 7-5-2008)
******************************************************/
int MuFTAD_Query111(void *Ptr, int type, int resume)
{
	int ret;
	char *addr = NULL;
	int method;

	//build signal and post to server!!
	do{
		if(MuFTAD_QueryListPost(Ptr, type) < 0
			||MuFTAD_XmlProc(QUERY111) < 0){

			Mu_ErrorPrint();
			return MUEERO;
		}
	}while(need_retry >= 0);

	//if we got the XML, which contain list of the programm's address
	if((ret = Mu_Parse(PARSE_FDDF_LIST, list_fd,
		                     "//payload", &MuFddfListHead))< 0){
		Mu_ErrorPrint();
		return ret;
	}

	while(MuFddfListHead->total){
		addr = MuFddfListHead->next->addr;
		method = MuFddfListHead->authmethod;

		if(MuFTAD_QueryFddf(addr, method, resume) < 0){
			Mu_ErrorPrint();
			return MUEERO;
		}

		//delete one of the node, NOT the list of ALL
		if(Mu_FddfListNodeDelete(MuFddfListHead) < 0){
			Mu_ErrorPrint();
			return MUEERO;
		}

		MuFddfListHead->total --;
	}
	return MUOK;
}
/*****************************************************************
Description:
		Init the structure of MuEyewearADPsnd, it's used to build the query 
		signal. In the program, this structure's strings are all point to other
		contents.
		It has NO real spce for strings
Input:
		NONE
Output:
		MUOK: no error
		MUEERO: error happened
Lock:
		NONE
********************************************************************/
int MuFTAD_EyeADPSndInit(void)
{
	//init the structure of MuEyewearADPsnd
	Mu_EyewearADPInit(MuEyewearADPsnd);

	//Init the Protocol version and OPC of the query
	MuEyewearADPsnd->Protocolver = PROCVERSION;
	MuEyewearADPsnd->ProtocolOPC = 0;

	//create the login method
	//in the functin, we make challenge's content of the MuEyewearADPrcv
	//Be pointer to MuEyewearADPsnd
	//set the MuEyewearrcv NULL;
	MuEyewearADPsnd->prelogin->encryptionmethod = 
								MuEyewearADPrcv->prelogin->encryptionmethod;
	MuEyewearADPsnd->prelogin->challenge =
								MuEyewearADPrcv->prelogin->challenge;
	MuEyewearADPrcv->prelogin->challenge = NULL;

	//create the IDinit and IDApp
	MuEyewearADPsnd->device_info->DeviceIDInt = MuDeviceInfo->IdInt;
	MuEyewearADPsnd->device_info->DeviceIDApp= MuDeviceInfo->App;
	
	MuEyewearADPsnd->device_info->firmwareversion =
								FIREWEAREVERSION;
	MuEyewearADPsnd->device_info->hardwareversion =
								MuDeviceInfo->Hardwareversion;
	MuEyewearADPsnd->device_info->serverinfostep = 
								MuServiceInfo->stepping;

	return MUOK;
}


/**************************************************************
Description:
		dnowload the fireware update, and recording the description into
		update.xml
Input:
		NONE
Output:
		MUOK: no error
		MUEERO: error
LOCK:
		NONE
***************************************************************/
int MuFTAD_DnUpdate(void)
{
	int dnfd = 0;
	int recfd = 0;
	char *query = NULL;
	char *addr = NULL;
	int ret  = MUOK;
	char temp[256];
	ServerHeadPtr server = MuServiceInfo->server;
	
	UpdateInfoPtr MuUpdateInfo;
	SRV_SvrInfPtr MuSRVInfo;

	//create the structure for parse the info of Updates
	Mu_SRV_SvrInfCreate(&MuSRVInfo);
	Mu_UpdateInfoCreate(&MuUpdateInfo);
	
	//parse the XML
	if(Mu_Parse(PARSE_UPDATE_INFO, rcv_fd,
				"//payload", &MuSRVInfo) < 0){
		Mu_ErrorPrint();
		goto error;
	}

	//create the update recording XML
	snprintf(temp, 256, "./%s/%s", UPDATEPATH, UPDATEFILE);
	if((recfd = open(temp, O_RDWR|O_CREAT, 0644)) < 0){
		Mu_ErrorPrint();
		goto error_1;
	}

	//if update service Info XML
	if(MuSRVInfo->type == 100){
		MuUpdateInfo->srvsvrinfo->version = MuSRVInfo->version1;
		MuUpdateInfo->srvsvrinfo->checksum = MuSRVInfo->Md5Num;
		MuSRVInfo->Md5Num = NULL;

		snprintf(temp, 256, "./%s/%s", UPDATEPATH, SERVICEINFO);
		if((dnfd = open(temp, O_RDWR|O_CREAT, 0644))< 0){
			Mu_ErrorPrint();
			goto error_1;
		}
	}else if(MuSRVInfo->type == 101){
		MuUpdateInfo->srvfirmsf->firmwareversion = MuSRVInfo->version1;
		MuUpdateInfo->srvfirmsf->firmwarechecksum = MuSRVInfo->Md5Num;
		MuSRVInfo->Md5Num = NULL;

		snprintf(temp, 256, "./%s/%s", UPDATEPATH, FIREWEARE);
		if((dnfd = open(temp, O_RDWR|O_CREAT, 0644)) < 0){
			Mu_ErrorPrint();
			goto error_1;
		}
	}else
		goto error_1;

	//recording to ./updatepath/recording.xml, before download the firewares
	if(Mu_BuildXML(BUILD_UPDATE_INFO, recfd, MuUpdateInfo, NULL) < 0){
		Mu_ErrorPrint();
		goto error_2;
	}
	
again:
	addr = server->cur->addr;
	//malloc the query space, build the URL
	if(MuFTAD_MakeURL(&query, MuSRVInfo->method, MuSRVInfo->ServerInfo) < 0){
		Mu_ErrorPrint();
		goto error_2;
	}

	//Init MuDnStat to query server
	Mu_InitDnStat(&MuDnStat);
	MuDnStat.type = MU_GET;
	MuDnStat.fd = dnfd;
	ret = Mu_HttpPlus(query, MU_QUERY, (Mu_DnStatPtr)(&MuDnStat));
	MuFTAD_Free(query);

	//reconncet to server
	if(ret == MUECNNT){
		if(server->cur)
			server->cur = server->cur->next;
		else
			server->cur = server->next;

		goto again;
	}

	if(ret != MUOK){
		MuFTAD_ErrorProc(ret);
		goto error_2;
	}

	if(Mu_Md5File(dnfd, MuSRVInfo->Md5Num) < 0){
		Mu_ErrorPrint();
		goto error_2;
	}
	
	Mu_Close(recfd);
	Mu_Close(dnfd);

	//delete the MuSRVInfo
	//Mu_SRV_SvrInfDelete(MuSRVInfo);

	//delete the updateinfo
	Mu_UpdateInfoDelete(MuUpdateInfo);
	return MUOK;

error_2:
	Mu_Close(dnfd);
error_1:
	Mu_Close(recfd);
error:
	//delete the MuSRVInfo
	//Mu_SRV_SvrInfDelete(MuSRVInfo);

	//delete the updateinfo
	Mu_UpdateInfoDelete(MuUpdateInfo);

	//delete all the contents of the directory contain
	snprintf(temp, 256, "rm -rf ./%s/*", UPDATEPATH);
	system(temp);

	return MUEERO;
}
/***********************************************************
Description:
		after query servers use OPCCODE = 110, get the suscriped programs
		this function process 2xx and 4xx according to payload
Input:
		NONE
output:
		MUOK:  no error
		MUEERO: error
LOCK:
		NONE
****************************************************************/
int MuFTAD_Query110Proc(void)
{
	int ret;
	int i;
	int type;
	int method;
	FddfStatusNodePtr FdStPtr = NULL;
	DirStatusNodePtr DiStPtr = NULL;
	DirDownloadAddrPtr AddrPtr = NULL;
	DirDownloadAddrPtr AddrPtr_2 = NULL;
	

	//process the 2xx programms
	do{
		if((ret = Mu_Parse(PARSE_MULTI_2XX, stat_fd,
						"//payload", &MuFddfStatHead)) < 0){
			//when function Mu_Parse march the end of the 
			// 2xxpayload, we need to break the while loop
			//instead of return simplely!
			if(ret == MUEX2XX)
				break;
			else
				MUEERO;
		}

		//find the lastest list
		for(i = 0; i < 4; i ++){
			if(MuFddfStatHead[i].total)
				break;
		}
		
		//if have no 2XX to get ride of
		if(i = 4 && MuFddfStatHead[i].total == 0)
			break;

		while(MuFddfStatHead[i].total){
			FdStPtr = MuFddfStatHead[i].next;
			type = MuFddfStatHead[i].type;
			
			if(MuFTAD_Query111((void *)FdStPtr, type, 0)< 0){
				Mu_ErrorPrint();
				continue;
			}

			if(Mu_FddfStatusNodeDelete(&(MuFddfStatHead[i]), FdStPtr) < 0){
				Mu_ErrorPrint();
				return MUEERO;
			}

			MuFddfStatHead[i].total --;
		}
	}while(1);


	//process the 4xx programms
	do{
		if((ret = Mu_Parse(PARSE_MULTI_4XX, stat_fd,
						"//payload", &MuDirStatHead)) < 0){
			if(ret == MUEX4XX)
				break;
			else
				return MUEERO;
		}

		//find the lastest list
		for(i = 0; i < 4; i ++){
			if(MuDirStatHead[i].total)
				break;
		}

		//have no 4xx to get ride of
		if(i == 4 && MuDirStatHead[i].total == 0)
			break;

		while(MuDirStatHead[i].total){
			DiStPtr = MuDirStatHead[i].nodenext;
			method = DiStPtr->authmethod;
			AddrPtr = DiStPtr->addrnext;
			type = DiStPtr->type;

			while(AddrPtr){
				if(MuFTAD_Download4XX(AddrPtr->addr, type, method) < 0){
					Mu_ErrorPrint();
					continue;
				}

				AddrPtr_2 = AddrPtr;
				AddrPtr = AddrPtr->next;
				if(Mu_DirDownloadAddrDeleteone(DiStPtr, AddrPtr_2) < 0){
					Mu_ErrorPrint();
					return MUEERO;
				}
			}

			if(MuFTAD_UpSRV121((void *)DiStPtr, 903, 0) < 0){
				Mu_ErrorPrint();
				return MUEERO;
			}

			
			if(Mu_DirStatusNodeDelete(&(MuDirStatHead[i]), DiStPtr) < 0){
				Mu_ErrorPrint();
				return MUEERO;
			}

			MuDirStatHead[i].total --;
		}
	}while(1);

	return MUOK;

}
/**********************************************************
Description:
		Download the FDDF of the programms
Input:
		url: path and filename, without the server
		authmethod: wether https or http
		resume: ...
Output:
		MUOK:
		MUEERO:
LOCK:
		NONE
**********************************************************/
int MuFTAD_QueryFddf(const char *url, int authmethod, int resume)
{
	int xmlfd;
	int dnfd;
	int i;
	int ret;

	//store the resume for use 
	int resume_back = resume;
	int count = 0;
	char *query = NULL;
	char *addr = NULL;
	off_t offset = 0;
	
	char temp_1[256];
	ServerHeadPtr server = MuServiceInfo->server;
	SegmentNodePtr Ptr_1 = MuSegmentHead->next;
	SegmentNodePtr Ptr_2 = MuSegmentHead->next;

	snprintf(temp_1, 256,  "%s.mu", MuFddfFile->name);

	//check the file name.mv is exist or not,when the file is exist
	//the function will return with MUOK
	if(access(MuFddfFile->name, F_OK) == 0)
		return MUOK;
#if 0
//These lines are useless
	//if the name name.mv is not exist, then check the name.mv.mu whther 
	//exist...
	if(access(temp_1, F_OK) == 0)
		resume = 1;
#endif

	//if need to resume, open the file with readable & writeable
	if(resume){
		if((dnfd = open(temp_1, O_RDWR)) < 0){
			Mu_ErrorPrint();
			return MUEERO;
		}
	}else{
	//if NEEDN'T to resume, Create the file
		if((dnfd = open(temp_1, O_RDWR|O_CREAT, 0644)) < 0){
			Mu_ErrorPrint();
			return MUEERO;
		}
	}
	
again:
	if(MuFTAD_MakeURL(&query, authmethod, url) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}

	//query server
	Mu_InitDnStat(&MuDnStat);
	MuDnStat.type = MU_GET;
	MuDnStat.fd = rcv_fd;
	ret = Mu_HttpPlus(query, MU_QUERY, (Mu_DnStatPtr)(&MuDnStat));
	Mu_Free(query);

	//if need to reconnect to another server
	//we must change to the next server point
	if(ret == MUECNNT){
		if(server->cur->next)
			server->cur = server->cur->next;
		else
			server->cur = server->next;

		goto again;
	}

	if(ret != MUOK){
		MuFTAD_ErrorProc(ret);
		return MUEERO;
	}

	//parse Fddf content about the file description
	if((ret = Mu_Parse(PARSE_EYEWEAR_FDDF, rcv_fd,
						NULL, &MuFddfFile)) < 0){
			Mu_ErrorPrint();
			return MUEERO;
	}

	snprintf(temp_1, 256, "%s.xml", MuFddfFile->name);

	//build the file description of a program
	if((xmlfd = open(temp_1, O_RDWR|O_CREAT, 0644)) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}
	if(Mu_BuildXML(BUILD_FILE_DES, xmlfd, MuFddfFile, NULL) < 0){
		Mu_ErrorPrint();
		Mu_Close(xmlfd);
		return MUEERO;
	}
	Mu_Close(xmlfd);

	//parse Fddf Content about the segment
	if((ret = Mu_Parse(PARSE_EYEWEAR_FDDF, rcv_fd,
					"//payload", &MuSegmentHead)) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}

	if(MuFTAD_RecordResum(MuResume, MuFddfFile->type, MuFddfFile->name, &i) < 0){
		fprintf(stdout, "There are No Space to Store the Information for Resume\n");
		return MUEERO;
	}
		
	while(Ptr_1){
		do{
			//check out the offset of segment
			while(Ptr_2){
				if(Ptr_2->index < Ptr_1->index)
					offset += Ptr_2->size;
				Ptr_2 = Ptr_2->next;
			}

			//the downlocad function checksum the segment 
			//offset is the place, download function write into
			if(MuFTAD_Download2XX(dnfd, offset, Ptr_1, resume) < 0){
				resume = 1;
				count ++;
			}
			else
				break;

			if(count == 10)
				break;
		}while(1);
		
		count = 0;
		resume = resume_back;
		MuSegmentHead->total --;
		Ptr_1 = Ptr_1->next;
	}

	if(Mu_Md5File(dnfd, MuFileInfo->md5) < 0){
		Mu_ErrorPrint();
		goto error;
	}

	Mu_Close(dnfd);
	snprintf(temp_1, 256,  "%s.mu", MuFddfFile->name);
	rename(temp_1, MuFddfFile->name);
	
	if(MuFTAD_UpSRV121((void *)MuFddfFile->databaseinfo, 902, 0) < 0){//the third value is download succesfull
			Mu_ErrorPrint();
			return MUEERO;
	}
	
	MuFTAD_DeleteResum(MuResume, i);

	return MUOK;
	
error:
	Mu_Close(dnfd);
	
	if(MuFTAD_UpSRV121((void *)MuFddfFile->databaseinfo, 902, 1) < 0){//the third value is download failued
			Mu_ErrorPrint();
			return MUEERO;
	}
	return MUEERO;
	
}

int MuFTAD_Download2XX(int fd, off_t offset, SegmentNodePtr Ptr, int resume)
{
	int ret;
	char *query = NULL;
	char temp[256];
	char *addr = NULL;
	ServerHeadPtr server = MuServiceInfo->server;

	//if need to resume, we must check the segment first
	if(resume && (Mu_Md5Segment(fd, offset, Ptr->size, Ptr->md5) == MUOK))
		return MUOK;

again:
	snprintf(temp, 256, "%s/%s", MuFddfFile->serverfilepath, Ptr->name);

	if(MuFTAD_MakeURL(&query, MuFddfFile->Authorizemethod, temp) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}

	//query server 
	Mu_InitDnStat(&MuDnStat);
	MuDnStat.type = MU_GET;
	MuDnStat.fd = fd;
	MuDnStat.resume = resume;
	MuDnStat.startops = offset;
	MuDnStat.length = Ptr->size;

	ret = Mu_HttpPlus(query, MU_DOWNLOAD, (Mu_DnStatPtr)(&MuDnStat));
	MuFTAD_Free(query);

	if(ret == MUECNNT){
		if(server->cur->next)
			server->cur = server->cur->next;
		else
			server->cur = server->next;

		goto again;
	}

	if(ret != MUOK){
		MuFTAD_ErrorProc(ret);
		return MUEERO;
	}

	if(Mu_Md5Segment(fd, offset, Ptr->size, Ptr->md5) < 0)
		return MUECKS;

	return MUOK;

}

int MuFTAD_Download4XX(const char *url, int type, int authmethod)
{
	char *query = NULL;
	char *addr = NULL;
	char temp[256];
	char *Ptr = NULL;
	
	int i;
	int fd;
	int ret;
	ServerHeadPtr server = MuServiceInfo->server;

	if(MuFTAD_MakeURL(&query, authmethod, url) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}

	Ptr = strrchr(url, '/');
	snprintf(temp, 256, "%s.mu\0", Ptr+1);
	if((fd = open(temp, O_RDWR|O_CREAT, 0644)) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}

	if(MuFTAD_RecordResum(MuResume, type, Ptr+1, &i) < 0){
		fprintf(stdout, "There are No Space to Store the Information for Resume\n");
		return MUEERO;
	}
	
again:
	//query server
	Mu_InitDnStat(&MuDnStat);
	MuDnStat.type = MU_GET;
	MuDnStat.fd = fd;
	MuDnStat.resume = 0;
	ret = Mu_HttpPlus(query, MU_DOWNLOAD, (Mu_DnStatPtr)(&MuDnStat));
	Mu_Free(query);

	if(ret == MUECNNT){
		if(server->cur->next)
			server->cur = server->cur->next;
		else
			server->cur = server->next;

		goto again;
	}

	if(ret != MUOK){
		MuFTAD_ErrorProc(ret);
		return MUEERO;
	}

	rename(temp, Ptr+1);
	MuFTAD_DeleteResum(MuResume, i);
	return MUOK;

}
/***************************************************************
Description:
		process the resumes in the arrary MuResume
		To 2XX: call MuFTAD_Resume2XX
		To 4xx : call MuFTAD_Resume4xx
Input:
		NONE
Output:
		MUOK: no error
		MUEERO: error happened
Lock:
		NONE
****************************************************************/
int MuFTAD_Resume(void)
{
	int fd;
	int i;
	int count = 0;

	if((fd = open(RESUME_FILE, O_RDONLY)) < 0
		||Mu_Parse(PARSE_RESUME_INFO, fd, NULL, &MuResume) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}

	for(i = 0; i < 10; i++){
		if(CHECK_2XX(MuResume[i].type))
			MuFTAD_Resume2XX(i);
		else if(CHECK_4XX(MuResume[i].type))
			MuFTAD_Resume4XX(i);
	}

	//if the Resuming file is overflow, shall stop the program
	for(i = 0; i < 10; i ++){
		if(MuResume[i].type != 0)
			count ++;
	}

	if(count == 10){
		fprintf(stdout, "There are A LOT OF programms can't be Download, Please Delete Them, Or restart the Program!\n");
		return MUEDNC;
	}

	return MUOK;
}


int MuFTAD_Resume2XX(int i)
{
	int ret;
	char temp[256];
	DatabaseInfoPtr Ptr;

	//for delete the programms which can't download completely
	snprintf(temp, 256, "rm -rf %s.*", MuResume[i].name);
	//create the FddfStatusNode

	//check the programms in the MuResume
	ret = MuFTAD_Resume2XXCheck(&Ptr, MuResume[i].name);

	switch(-ret){
		case -MUOK:
			//if the program is MUOK
			//we need to delete the info from MuResume
			MuFTAD_DeleteResum(MuResume, i);
			MuFTAD_Free(Ptr);
			return MUOK;
			
		case -MUEDWN: //resume
			break;
			
		case -MUEXML:
		case -MUEERO:
		default:
			Mu_ErrorPrint();
			system(temp); //delete the programms, because we can't resume it for ever
			MuFTAD_Free(Ptr);
			return MUEERO;
	}

	//resume the program
	MuFTAD_Query111((void *)Ptr, 902, 1);

	//don't free the space of the pointer point to
	//because the Ptr have no REAL space which point to
	MuFTAD_Free(Ptr);

	return MUOK;
}

int MuFTAD_Resume4XX(int i)
{
	int fd;
	int urllen;
	int ret;
	char temp[256];
	char url[1024];
	
	snprintf(temp, 256, "%s.jw", MuResume[i].name);
	if(access(temp, F_OK) == -1){
		Mu_ErrorPrint();
		goto error;
	}

	if((fd = open(temp, O_RDONLY)) < 0){
		Mu_ErrorPrint();
		goto error;
	}

	if(read(fd, &urllen, sizeof(int)) < 0){
		Mu_ErrorPrint();
		goto error;
	}

	//read the url from the file ,and compare the url
	//make sure the address resuming is the same
	if(read(fd, url, urllen) <=0){
		Mu_ErrorPrint();
		goto error;
	}
	Mu_Close(fd);

	snprintf(temp, 256, "%s.mu", MuResume[i].name);
	if(access(temp, F_OK) == -1){
		if(access( MuResume[i].name, F_OK) == 0)
			goto error_1;

		if((fd = open(temp, O_RDWR|O_CREAT, 0644)) < 0){
			Mu_ErrorPrint();
			goto error;
		}
	}else if((fd = open(temp, O_RDWR)) < 0){
		Mu_ErrorPrint();
		goto error;
	}

	Mu_InitDnStat(&MuDnStat);
	MuDnStat.type = MU_GET;
	MuDnStat.fd = fd;
	MuDnStat.resume = 1;

	ret = Mu_HttpPlus(url, MU_DOWNLOAD, (Mu_DnStatPtr)(&MuDnStat));

	if(ret < 0){
		Mu_Close(fd);
		goto error;
	}

	Mu_Close(fd);
	MuFTAD_DeleteResum(MuResume, i);
	return MUOK;

error:
	snprintf(temp, 256, "rm -rf %s %s.*", MuResume[i].name, MuResume[i].name);
	system(temp);

error_1:
	MuFTAD_DeleteResum(MuResume, i);
	return MUEERO;
}

