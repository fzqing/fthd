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
#include "include/Mu_NetIO.h"
#include "include/Mu_error.h"
#include "include/Mu_Creatxml.h"
#include "include/MuFTAD_Init.h"
#include "include/MuFTAD_App.h"

/*********************************************************************
Description:
		the error process to return values of all the Functions
Input:
		ret : return value;
Output:
		NONE;
LOCK:
		NONE;
**********************************************************************/
void MuFTAD_ErrorProc(int ret)
{
	switch(-ret){
		case -MUEBLD:
			fprintf(stdout, "when build the XML error happened!\n");
			break;
		case -MUECNNT:
			fprintf(stdout, "connect to server error, U should reconnect to server!\n");
			break;
		case -MUEERO:
			fprintf(stdout, "error happend!\n");
			break;
		case -MUEFSY:
			fprintf(stdout, "file system error!\n");
			break;
		case -MUENSP:
			fprintf(stdout, "there are no space for the download!\n");
			break;
		case -MUEREFUSED:
			fprintf(stdout, "server refused the connection!\n");
			break;
		case -MUESSL:
			fprintf(stdout, "SSL error!\n");
			break;
		case -MUETIMEOUT:
			fprintf(stdout, "timeout, when connect to server!\n");
			break;

		case -MUFLEN:
			fprintf(stdout, "the error of the length!\n");
			break;
		default:
			fprintf(stdout, "error happened!\n");
			break;
		}

		return;
		
}

/********************************************************************
Description:
		copy FddfStatuseNode to DBIInfo Structure
Input:
		Ptr: FddfStausNode
		DbiInfo: the structure transfer into
Output:
		MUOK: no error
		MUNBUF: no buffer
Lock:
		NONE
*********************************************************************/
int MuFTAD_CopyInfoToDBII(FddfStatusNodePtr Ptr, DBIInfoPtr DbiInfo)
{
	//Init  buffer of the DBIInof used, But not Free the string pointed by the member
	Mu_DBIInfoPtrInit(DbiInfo);

	DbiInfo->type = Ptr->type;
	DbiInfo->text = Ptr->text;
	DbiInfo->newer = Ptr->newer;
	DbiInfo->older =  Ptr->older;
	DbiInfo->priority =  Ptr->priority;

	DbiInfo->group = Ptr->group;
	DbiInfo->owner = Ptr->owner;
	DbiInfo->program = Ptr->program;

	return MUOK;

}
/********************************************************************
Description:
		copy FddfStatuseNode to DBUDirt Structure
		reference the 903
Input:
		Ptr: FddfStausNode
		Dbudirt: the structure transfer into
Output:
		MUOK: no error
		MUNBUF: no buffer
Lock:
		NONE
*********************************************************************/
int MuFTAD_CopyInfoToDBUD(DirStatusNodePtr Ptr, DBUDirtPtr Dbudirt, int suc)
{
	//Init  buffer of the DBIInof used, 
	//But not Free the string pointed by the member
	Mu_DBUDirtPtrInit(Dbudirt);

	Dbudirt->status = suc;
	Dbudirt->download=  Ptr->DownloadID;

	return MUOK;
}
/********************************************************************
Description:
		copy FddfStatuseNode to DBURate Structure
Input:
		Ptr: FddfStausNode
		Dburate: the structure transfer into
Output:
		MUOK: no error
		MUNBUF: no buffer
Lock:
		NONE
*********************************************************************/
int MuFTAD_CopyInfoToDBUR(DatabaseInfoPtr Ptr, DBURatePtr Dburate, int rate)
{
	Mu_DBURatePtrInit(Dburate);

	Dburate->filerate = rate;
	Dburate->index = Ptr->index;

	Dburate->group = Ptr->group;
	Dburate->owner = Ptr->owner;
	Dburate->program = Ptr->program;

	return MUOK;

}

/********************************************************************
Description:
		copy FddfStatuseNode to DBUInfo Structure
Input:
		Ptr: FddfStausNode
		DbuInfo: the structure transfer into
Output:
		MUOK: no error
		MUNBUF: no buffer
Lock:
		NONE
*********************************************************************/
int MuFTAD_CopyInfoToDBUI(DatabaseInfoPtr Ptr, DBUInfoPtr DbuInfo, int suc)
{
	//all of the strings in structure DbuInfo are in others structure
	//we need to do is just to Init it, in case other function free it
	Mu_DBUInfoPtrInit(DbuInfo);
	
	DbuInfo->newer = Ptr->index;
	DbuInfo->older = Ptr->index;
	DbuInfo->program = Ptr->program;
	DbuInfo->owner = Ptr->owner;
	DbuInfo->group = Ptr->group;

	DbuInfo->status = suc;
    return MUOK;
}

/************************************************************************
Description:
		record the resume into the array of the Resume
		the number of the array is returned by i
Input:
		resume: the gobal array pointer of MuResume
		type: the typs of the programm
		name: the name of the programm
		*i: return the number
Output:
		MUOK: no error
		MUEERO: error happened
Lock:
		NONE
*************************************************************************/
int MuFTAD_RecordResum(ResumeInfoPtr resume, int type, const char *name, int *i)
{
	int num;

	for(num= 0;  num < 10; num++){
		if(resume[num].type == 0)
			break;
	}
    
	if(num == 10){
		Mu_ErrorPrint();
		return MUEERO;
	}

	resume[num].type = type;
	snprintf(resume[num].name, MU_NAME_MAX, "%s", name);

	*i = num;
	return MUOK;

}


int MuFTAD_DeleteResum(ResumeInfoPtr resume, int i)
{

	resume[i].type = 0;
	//MuFTAD_Free(resume[i].name);

	return MUOK;

}
/***********************************************************
Description:
		update the time of the hardclock
Input:
		time: the string of the time
Output:
		MUOK: no error
		MUEERO: error happened 
Lock:
		NONE
example:
		Time: 20080218162030
		year:2008
		month: 02
		day: 18
		hour: 16
		min: 20
		sec: 30
************************************************************/
int MuFTAD_UpdateTime(char *Time)
{
	char temp[256];
	snprintf(temp, 256, "hwclock --set  --date=%s/%s/%s %s:%s", 
			Time+2, Time+4, Time+6, Time+8, Time+10);

	system(temp);
	system("hwclock --hctosys");
	return MUOK;
}

/**********************************************************
Description:
		update the ID information about DeviceIDInit
		& DeviceIDApp
		In the function, we will update two structures
		1.DeviceInfo
		2.MuEyewearADPsnd
Input:
		IdInit: update IdInit
		IdApp: update IdApp
Output:
		MUOK: no error
		MUEERO: error happened
Lcok:
		NONE
************************************************************/
int MuFTAD_UpdateID(const char *IdInit,  const char *IdApp)
{
	char *buf = NULL;
	int fd = 0;
	
	MuFTAD_Free(MuDeviceInfo->IdInt);
	MuFTAD_Free(MuDeviceInfo->App);

	//Update the member IDinit in MuDeviceinfo
	if(NULL == (buf = malloc(strlen(IdInit) + 1))){
		Mu_ErrorPrint();
		return MUNBUF;
	}
	snprintf(buf, strlen(IdInit), "%s", IdInit);
	MuDeviceInfo->IdInt =  buf;
	MuEyewearADPsnd->device_info->DeviceIDInt =  buf;

	//update the member IdApp in deviceinfo,
	if(NULL == (buf = malloc(strlen(IdApp) + 1))){
		Mu_ErrorPrint();
		return MUNBUF;
	}
	snprintf(buf, strlen(IdApp), "%s", IdApp);
	MuDeviceInfo->App =  buf;
	MuEyewearADPsnd->device_info->DeviceIDApp =  buf;
	
	buf = NULL;
	
	//fllush the updates into the file of deviceinfo.xml
	//Error MAY BE happened! WHEN update the file
	if(((fd = open(DEVICEINFO, O_RDWR|O_CREAT)) < 0)
		||Mu_BuildXML(BUILD_DEVICE_INFO, fd, MuDeviceInfo, NULL) < 0){

		Mu_ErrorPrint();
		Mu_Close(fd);
		return MUEERO;
	}

	Mu_Close(fd);
	return MUOK;

}
/************************************************************************
Description:
		Update the Conversation structure 
Input:
		ID: the id string
		Username: username set for device by server
		password: password set for device by server
Output:
		MUOK: no error
		MUEERO: error happened
Lock:
		NONE
*************************************************************************/
int MuFTAD_UpdateConv (ConversationPtr *Ptr)
{
	//free the members, ID, username, password,eg. in the MuEyewearADPsnd	
	MuFTAD_Free(MuEyewearADPsnd->conversation->ConversationID);
	MuFTAD_Free(MuEyewearADPsnd->conversation->AuthUsername);
	MuFTAD_Free(MuEyewearADPsnd->conversation->AuthPassword);
	MuFTAD_Free(MuEyewearADPsnd->conversation);

	MuEyewearADPsnd->conversation = *Ptr;

	//set the MuEyewearADPrcv->conversation = NULL, 
	//in case Free(MuEyewearADPrcv->conversation) error
	*Ptr = NULL;
	return MUOK;
}
			
