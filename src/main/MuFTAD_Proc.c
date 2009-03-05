/*******************************************************************
* 					====Microunit Techonogy Co,.LTD.====
* File Name:
*
* 		MuFTAD_Init.c
*
* Description:
*
* 		This file contain Contain the POST method and error process
*
* Revision History:
*
* 		6-5-2008 ver1.0
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
#include "include/Mu_error.h"
#include "include/MuFTAD_Init.h"
#include "include/MuFTAD_App.h"
#include "include/MuFTAD_Proc.h"
/*****************************************************
*Description:
		POST EyewearADP to server, the ADP is a signal
		which NOT contain the payload;
		In function, build the signal with MuEyewearADPsnd
		structure; 

		in MuEyewearADPsnd the members is the laster;
Input:
		opccode: operation Code,
		reference to protocol of franch Telecom.
		<doc00_SoftProtocol_0.1.0_RC1.doc>

		the value if opccode maybe
		LOGIN
		LOGOUT101
		LOGOUT102
		QUERY110
		UPSTAT120
Output:
		return value ,when no error happened
		return MUOK;
		0 No ERROR
		<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/

int MuFTAD_EyeADPPost(int opccode)
{
	int ret = MUOK;
	MuEyewearADPsnd->ProtocolOPC = opccode;

	//build the XML , is the signal of query 
	if(Mu_BuildXML(BUILD_SIGNAL_ADP, snd_fd, MuEyewearADPsnd, NULL) < 0
		||MuFTAD_QueryPost(opccode) < 0){

		MuFTAD_ErrorProc(ret);
		return MUEERO;
	}

	return MUOK;
}
/*****************************************************
*Description:
		the same to MuFTAD_EyeADPPost,
		BUT the opccode of this function is QUERY111
Input:
		Ptr: FddfListNodePtr structure store the info for 
			fill MUIIfo or others

		type: it's the classic of the payload,when
			query Fddf List from Server
			the value if type maybe
			902
			201-204
Output:
		return value ,when no error happened
		return MUOK;
			0 No ERROR
			<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/
int MuFTAD_QueryListPost(void *Ptr, int type)
{
	MuEyewearADPsnd->ProtocolOPC = QUERY111;

	//build Post XML
	if(type == 902){
		//requery the server for Downloaded files
		// 2: Re-Send Address when Query
		if(MuFTAD_CopyInfoToDBUI((DatabaseInfoPtr)Ptr, MuDBUInfo, 2) < 0
			||Mu_BuildXML(BUILD_SIGQUE_902, snd_fd, MuEyewearADPsnd, MuDBUInfo)< 0){

			Mu_DBUInfoPtrInit(MuDBUInfo);
			Mu_ErrorPrint();
			return MUEBLD;
		}

		//after build the Info, clear the MuDBUInfo
		Mu_DBUInfoPtrInit(MuDBUInfo);
	}
	else if(CHECK_2XX(type)){
		//for new download
		if(MuFTAD_CopyInfoToDBII((FddfStatusNodePtr)Ptr, MuDBIInfo) < 0
			||Mu_BuildXML(BUILD_SIGQUE_20X, snd_fd, MuEyewearADPsnd, MuDBIInfo) < 0){
							
			Mu_DBIInfoPtrInit(MuDBIInfo);//clear the buffer
			Mu_ErrorPrint();
			return MUEBLD;
		}

		//after build the signal, clear the MuDBIInfo
		Mu_DBIInfoPtrInit(MuDBIInfo);
	}
	else
		return MUETYP;

	//post to server
	if(MuFTAD_QueryPost(QUERY111) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}

	return MUOK;
}
/*****************************************************
*Description:
		the same to MuFTAD_EyeADPPost,
		BUT the opccode of this function is UPDN121
		and UPFILE122
Input:
		Ptr: FddfListNodePtr structure store the info for 
			fill MUIIfo or others
		opccode: the type of the OPCCODE
			including: UPDN121, and UPFILE122
		type: it's the classic of the payload,when
			query Fddf List from Server
			the value if type maybe 902 903;
			when the opccode is UPFILE122, the type is unusefull!
		suc:
			1.if the opccode is UPDN121, suc is mean: successfull abd failure
			2.if the opccode is UPFILE122 ,suc is the rate
Output:
		MUOK:no error
		MUEBLD: error of building XML
		MUETYP: the type is not match
		MUEERO: error
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/
int MuFTAD_UpDnPost(void *Ptr, int opccode, int type, int suc)
{
	int ret = MUOK;
	MuEyewearADPsnd->ProtocolOPC = opccode;

	//Init XML
	if(opccode == UPDN121){
		if(type == 902){
			if(MuFTAD_CopyInfoToDBUI((DatabaseInfoPtr)Ptr, MuDBUInfo, suc) < 0
				||Mu_BuildXML(BUILD_SIGUP_DN902, snd_fd, MuEyewearADPsnd, MuDBUInfo)< 0){
				
				Mu_DBUInfoPtrInit(MuDBUInfo) ;
				Mu_ErrorPrint();
				return MUEBLD;
			}
			
			Mu_DBUInfoPtrInit(MuDBUInfo) ;
		}else if(type == 903){
			if(MuFTAD_CopyInfoToDBUD((DirStatusNodePtr)Ptr, MuDBUDirt, suc) < 0
				||Mu_BuildXML(BUILD_SIGUP_DN903, snd_fd, MuEyewearADPsnd, MuDBUDirt)< 0){
				
				Mu_DBUDirtPtrInit(MuDBUDirt);
				Mu_ErrorPrint();
				return MUEBLD;
			}
			
			Mu_DBUDirtPtrInit(MuDBUDirt);
		}else
			return MUETYP;

	}else if(opccode == UPFILE122){
		if(MuFTAD_CopyInfoToDBUR((DatabaseInfoPtr)Ptr, MuDBURate, suc) < 0
			||Mu_BuildXML(BUILD_SIGUP_FILE, snd_fd, MuEyewearADPsnd, MuDBURate) < 0){
			
				Mu_DBURatePtrInit(MuDBURate);
				Mu_ErrorPrint();
				return MUEBLD;
		}
		Mu_DBURatePtrInit(MuDBURate);
		
	}else
		return MUETYP;

	if(MuFTAD_QueryPost(opccode) < 0){
		MuFTAD_ErrorProc(ret);
		return MUEERO;
	}

	return MUOK;
}
/*****************************************************
*Description:
		the function process the statuse of the return of a POST
		to server, when the return OPCCODE is 200, function will be 
		return MUOK;
		when the opccode is others, function will go ahead, until the 
		return opccode is 200;
		if the opccode is not 200 for ever, the function will NOT return
		for ever;
Input:
		statcode:
			the opccode returned from server, including 200, 301,300
			and others
Output:
		return value ,when no error happened
		return MUOK;
			0 200 opccode
			<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/
int MuFTAD_XmlProc(int statcode)
{
	int ret = MUOK;
	int fd;

	//the first time, we must parse the file stored the XML, 
	//which maybe rcvfd, listfd, or rcvfd;
	//when the statcode if 30X or 405,the statcode is ONLY rcvfd;
	if(0 == (fd = MuFTAD_GetStoreFd(statcode))){
		Mu_ErrorPrint();
		return MUEERO;
	}

	while(1){
		if((ret = Mu_Parse(PARSE_EYEWEAR_ADP,
						  fd, NULL, &MuEyewearADPrcv)) < 0)
			break;

		switch(MuEyewearADPrcv->ProtocolOPC){
			case 200:
				if((ret = MuFTAD_Proc200(statcode) ) < 0){
					Mu_ErrorPrint();
					return MUEERO;
				}
				need_retry --;
				break;

			case 300:
				//MUST to update the Info
				if((ret = MuFTAD_Proc300()) < 0)
					break;
				fd = rcv_fd;
				need_retry = 1;
				continue;

			case 301:
				if((ret = MuFTAD_Proc301()) < 0)
					break;
				fd = rcv_fd;
				need_retry = 1;
				continue;

			case 405:
				//MUST to relogin
				if((ret = MuFTAD_EyeADPPost(LOGIN)) < 0)
					break;
				fd = rcv_fd;
				need_retry = 1;
				continue;

			default:
				return MUEERO;
		}
	}
	
	return ret;
}
/*****************************************************
*Description:
		the function update the Time and the ID number ,c onversation
		IDs, when Device Loginto the server;
Input:
		void
Output:
		return value ,when no error happened
		return MUOK;
			0 No ERROR
			<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/
int MuFTAD_ProcLogin200(void)
{
	DeviceInfoPtr Ptr_device = MuEyewearADPrcv->device_info;
	ConversationPtr Ptr_conv = MuEyewearADPrcv->conversation;
	
	//updateTimer
	if(Ptr_device->SRVDateTime){
		if(MuFTAD_UpdateTime(Ptr_device->SRVDateTime) < 0){
			Mu_ErrorPrint();
			return MUEERO;
		}
	}
	
	//update ID
	if(Ptr_device->DeviceIDInt 
		|| Ptr_device->DeviceIDApp){ 
		if(MuFTAD_UpdateID(Ptr_device->DeviceIDInt,  Ptr_device->DeviceIDApp) < 0){
			Mu_ErrorPrint();
			return MUEERO;
		}
	}
	
	//update conversation
	if(Ptr_conv->ConversationID
		|| Ptr_conv->AuthUsername
		|| Ptr_conv->AuthPassword){
		if(MuFTAD_UpdateConv (&Ptr_conv) < 0){
			Mu_ErrorPrint();
			return MUEERO;
		}
	}

	return MUOK;
}
/*****************************************************
*Description:
		the function call other functios to process the 200 Opccdoe;
Input:
		opccode:
			method of the QUERY, including:
			LOGIN & QUERY110
Output:
		return value ,when no error happened
		return MUOK;
			0 No ERROR
			<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/

int MuFTAD_Proc200(int opccode)
{
	int ret = MUOK;

	switch(opccode){
		case LOGIN:
			ret = MuFTAD_ProcLogin200();
			break;

		case QUERY110:
			ret = MuFTAD_ProcQueryStat200();
			break;

		default:
			break;
	}
	return ret;
}
/*****************************************************
*Description:
		the function call other functios to process the 300 Opccdoe;
Input:
		void
Output:
		return value ,when no error happened
		return MUOK;
			0 No ERROR
			<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/
int MuFTAD_Proc300(void)
{
	DeviceInfoPtr Ptr = MuEyewearADPrcv->device_info;
	//update the device information
	if(Ptr->DeviceIDInt|| Ptr ->DeviceIDApp) {
		if(MuFTAD_UpdateID(Ptr ->DeviceIDInt, Ptr->DeviceIDApp) < 0){
			Mu_ErrorPrint();
			return MUEERO;
		}
	}

	//post to server for the update!
	if(MuFTAD_EyeADPPost(UPSTAT120) < 0){
		Mu_ErrorPrint();
		return MUEERO;
	}

	return MUOK;
}
/*****************************************************
*Description:
		the function call other functios to process the 301 Opccdoe;
Input:
		void
Output:
		return MUOK
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/

int MuFTAD_Proc301(void)
{

	return MUOK;
}

/*****************************************************
*Description:
		the function call other functios to process the Update 120;
Input:
		void
Output:
		return value ,when no error happened
		return MUOK;
			0 No ERROR
			<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/
int MuFTAD_UpSRV120Post(void)
{
	int ret ;
	
	do{
		if((ret = MuFTAD_EyeADPPost(UPSTAT120)) < 0
			||(ret = MuFTAD_XmlProc(UPSTAT120)) < 0){
		
			Mu_ErrorPrint();
			return ret;
		}
	//0: 200
	// 1: error, need to retry
	}while(need_retry >= 0);

	return MUOK;
}

/*****************************************************
*Description:
		the function call other functios to update username
		and password;
Input:
		void
Output:
		return value ,when no error happened
		return MUOK;
			0 No ERROR
			<0 Error happened
LOCK:
		NONE
Modify:
 		ssw(fzqing@gamil.com 6-5-2008)
******************************************************/
int MuFTAD_ProcQueryStat200(void)
{
	ConversationPtr Ptr = MuEyewearADPrcv->conversation;
	
	if(Ptr->AuthUsername|| Ptr ->AuthPassword){
		if(MuFTAD_UpdateConv (&Ptr) < 0){
				Mu_ErrorPrint();
				return MUEERO;
		}
	}
	
	return MUOK;
}

int MuFTAD_QueryPost(int querytype)
{
	char *addr;
	int querylen;
	char *query = NULL;
	char *api = NULL;
	int ret = MUOK;

	ServerHeadPtr server = MuServiceInfo->server;
	if(NULL == (api=(MuFTAD_GetAPI(querytype)))){
		Mu_ErrorPrint();
		return MUNAPI;
	}

	if(NULL == server->cur){
		Mu_ErrorPrint();
		return MUEERO;
	}

again:
	if(NULL == (addr = server->cur->addr)){
		Mu_ErrorPrint();
		return MUEERO;
	}

	//build the query addr
	querylen = snprintf(NULL, 0, "https://%s/%s", addr, api);
	querylen ++;
	
	if(NULL == (query = malloc(querylen))){
		Mu_ErrorPrint();
		return MUNBUF;
	}
	snprintf(query, querylen, "https://%s/%s", addr, api);

	//query server 
	Mu_InitDnStat(&MuDnStat);
	MuDnStat.type = MU_POST;
	if(!(MuDnStat.fd = MuFTAD_GetStoreFd(querytype))){
		Mu_ErrorPrint();
		return MUEERO;
	}
	MuDnStat.xmlfd = snd_fd;
	
	ret = Mu_HttpPlus(query, MU_QUERY, (Mu_DnStatPtr)(&MuDnStat));
	MuFTAD_Free(query);

	//reconnect to server
	if(ret == MUECNNT){
		if(server->cur->next)
			server->cur = server->cur->next;
		else
			server->cur = server->next;

		goto again;
	}

	return ret;
}

int MuFTAD_GetStoreFd(int opccode)
{
	int fd = 0;

	//get the api method
	switch(opccode){
		case QUERY110:
			fd = stat_fd;
			break;
		case QUERY111:
			fd = list_fd;
			break;
		case LOGIN:
		case UPSTAT120:
		case UPDN121:
		case UPFILE122:
		case LOGOUT101:
		case LOGOUT102:
			fd = rcv_fd;
			break;
		default:
			break;
	}

	return fd;
}

char *MuFTAD_GetAPI(int opccode)
{
	char *api = NULL;

	//get the api method
	switch(opccode){
		case LOGIN:
			api = MuServiceInfo->apipathlist->login;
			break;
		case QUERY110:
			api = MuServiceInfo->apipathlist->querystatus;
			break;
		case QUERY111:
			api = MuServiceInfo->apipathlist->queryfddflist;
			break;
		case UPSTAT120:
		case UPDN121:
		case UPFILE122:
			api = MuServiceInfo->apipathlist->updatestatus;
			break;
		case LOGOUT101:
		case LOGOUT102:
			api = MuServiceInfo->apipathlist->prelogin;
			break;
		default:
			return NULL;
	}

	return api;
}

int MuFTAD_MakeURL(char **query, int method, char *path)
{
	int querylen;
	ServerHeadPtr server = MuServiceInfo->server;
	char *addr = server->cur->addr;
	char *MuUsername = MuEyewearADPsnd ->conversation->AuthUsername;
	char *MuPassword = MuEyewearADPsnd->conversation->AuthPassword;
	
	//build the query addr
	switch(method){
		case 0:
			querylen = snprintf(NULL, 0, "https://%s/%s", addr, path);
			querylen++;
			if(NULL == (*query = malloc(querylen))){
				Mu_ErrorPrint();
				return MUNBUF;
			}
			snprintf(*query, querylen,
					"https://%s/%s", addr, path);
			break;
		case 1:
			querylen = snprintf(NULL, 0,
					"https://%s:%s@%s/%s",
					MuUsername, MuPassword,
					addr, path);
			querylen ++;
			if(NULL == (*query = malloc(querylen))){
				Mu_ErrorPrint();
				return MUNBUF;
			}
			snprintf(*query, querylen,
					"https://%s:%s@%s/%s",
					MuUsername, MuPassword,
					addr, path);
			break;
		case 2:
			querylen = snprintf(NULL, 0,
							"http://%s/%s",addr, path);
			querylen ++;
			if(NULL == (*query = malloc(querylen))){
				Mu_ErrorPrint();
				return MUNBUF;
			}
			snprintf(*query, querylen,
					"http://%s/%s", addr, path);
			break;
		case 3:
			querylen = snprintf(NULL, 0,
							"http://%s:%s@%s/%s",
							MuUsername, MuPassword,
							addr, path);
			querylen ++;
			if(NULL == (*query = malloc(querylen))){
				Mu_ErrorPrint();
				return MUNBUF;
			}
			snprintf(*query, querylen,
					"http://%s:%s@%s/%s",
					MuUsername, MuPassword,
					addr, path);
			break;
		default:
			return MUEMEH;
		}
	return MUOK;
}


int MuFTAD_Resume2XXCheck(DatabaseInfoPtr *Ptr, const char *name)
{
	int ret;
	int fd;
	char temp[256];

	snprintf(temp, 256, "%s.xml", name);
	if(access(temp, F_OK) == -1){
		Mu_ErrorPrint();
		return MUEXML;
	}

	Mu_FileInfoClear(&MuFileInfo);

	//if the update xml is exist , then read it , 
	//parse  its contents to structure!
	if((fd = open(temp, O_RDONLY)) < 0){

		Mu_ErrorPrint();
		return MUEFSY;
	}
	
	if(Mu_Parse(PARSE_FILE_INFO, fd, NULL, &MuFileInfo) < 0){

		Mu_ErrorPrint();
		Mu_Close(fd);
		return MUEXML;
	}
	Mu_Close(fd);

	*Ptr = MuFileInfo->datebaseinfo;
	
	if((ret = access(name, F_OK))== -1){//if the name.mv is exist, shall exit with MUOK

		//check the middle file== name.mu
		//if it exist, we shall resume it
		snprintf(temp, 256, "%s.mu", name);
		if((fd = open(temp, O_RDONLY)) < 0){
			Mu_ErrorPrint();
			return MUEFSY;
		}
		if(Mu_Md5File(fd, MuFileInfo->md5) < 0){

			Mu_ErrorPrint();
			Mu_Close(fd);
			return MUEDWN; //need to resume
		}

		//if thte file Md5 check sum is ok, we shall rename it
		rename(temp, name);
	}else if(ret == 0)
		return MUOK;


	return MUOK; //the file is ok, needn't to resume
}

		
	
