/****************************************************************
* ====Microunit Techonogy Co,.LTD.====
* File Name:
*
* main.c
*
* Description:
*
* First: to parse the pointed xml file and get data(use XPath and xmlDocPtr manners) 
* Second:create xml file according to the data
*
* Revision History:
*
* 13-5-2008 ver1.0
*
* Author:wyj(wyjun@microunit.com.cn)
* Modify:
*
*				***PROTECTED BY COPYRIGHT***
****************************************************************/
#include "../include/Mu_Struct_main.h"
#include "../include/Mu_Error.h"
#include "../include/Mu_xml_parse.h"
#include "../include/Mu_Creatxml.h"
#include <fcntl.h>

/*use the data which not exist will segment error*/
int  main(int argc,char ** argv)
{
	int fd,fd1;

	DirDownloadAddrPtr addrptr;
	DirStatusNodePtr DirNode;	
	DirStatusHeadPtr  DirHead;
	FddfStatusHeadPtr HeadPtr;
	FddfStatusNodePtr Head;
	XmlDeviceInfoPtr Devicepp;
	ResumeInfoPtr Resumepp;
	UpdateInfoPtr updatepp;
	FddfFilePtr filepp;
	ServiceInfoPtr servicepp;
	EyewearADPPtr querypp;
	SRV_SvrInfPtr srvinfo;
	SRV_FirmSFPtr firmsf;
	SegmentHeadPtr segptr;
	SegmentNodePtr nodeptr;
	FileInfoPtr back;
	FddfListHeadPtr listhead;
	FddfListNodePtr fnodeptr;
	DBURatePtr dburate;
	DBUInfoPtr dbuinfoptr;
	DBIInfoPtr dbiptr;
	DBUDirtPtr dbudirt;
	
	fd = open("02_Before_Login.xml", O_RDWR);			//xmlParse1
 	if (fd < 0)  perror("Error fd");
	Mu_EyewearADPCreate(&querypp);
	Mu_Parse(PARSE_EYEWEAR_ADP, fd, NULL, &querypp);
	printf("%d\n", querypp->ProtocolOPC);				
	printf("%d\n", querypp->Protocolver);				
	printf("%s\n", querypp->device_info->DeviceIDInt);	
	printf("%s\n", querypp->prelogin->challenge);	

//	fd = open("07_After_QueryFDDF.xml", O_RDWR);			//xmlParse2
//  	if (fd < 0)  perror("Error fd");
//	Mu_FddfListHeadCreate(&listhead);
//	Mu_Parse(PARSE_FDDF_LIST, fd, "//Item", &listhead);
//	printf("%d\n", listhead->size);		//size will changes with addr increasing
//	printf("%s\n", listhead->next->addr);
//	listhead->next = listhead->next->next;
//	printf("%s\n", listhead->next->addr);
//	listhead->next = listhead->next->next;
//	printf("%s\n", listhead->next->addr);

//	fd = open("update SRV.xml", O_RDWR);				//xmlParse3
//	if (fd < 0)  perror("Error fd");
//	Mu_UpdateInfoCreate(&updatepp);		
//	Mu_Parse(PARSE_UPDATE_INFO, fd, NULL, &updatepp);		
//	printf("%d\n", updatepp->srvsvrinfo->version);
//	printf("%s\n", updatepp->srvsvrinfo->checksum);	
//	printf("%d\n", updatepp->srvfirmsf->firmwareversion);	
//	printf("%s\n", updatepp->srvfirmsf->firmwarechecksum);	
//	printf("%d\n", updatepp->srvfirmsf->hardwareversion);	

//	fd = open("05_After_Query.xml", O_RDWR);		    	//xmlParse4  	
//  	if (fd < 0)  perror("Error fd");
//	Mu_FddfStatusHeadCreate(&HeadPtr);	                    	//2XXXXXXX
//	Mu_Parse(PARSE_MULTI_2XX, fd, "//Item", &HeadPtr);	
//	printf("%d\n", HeadPtr->next->size);
//	printf("%d\n", HeadPtr->next->type);
//	printf("%s\n", HeadPtr->next->text);
//	Mu_FddfStatusNodeDelete(HeadPtr, HeadPtr->next);
//	printf("%d\n", HeadPtr->next->size);
//	printf("%d\n", HeadPtr->next->type);
//	printf("%s\n", HeadPtr->next->text);
//	printf("%d\n", HeadPtr->total);
//	printf("%d\n", HeadPtr->next->next->size);
//	printf("%d\n", HeadPtr->next->next->type);
//	printf("%s\n", HeadPtr->next->next->text);                   
//	printf("%d\n", HeadPtr->next->next->next->size);
//	printf("%d\n", HeadPtr->next->next->next->type);
//	printf("%s\n", HeadPtr->next->next->next->text);                   

//	Mu_DirStatusHeadCreate(&DirHead);			    	//4XXXXXX	
//	Mu_Parse(PARSE_MULTI_4XX,fd, "//Item", &DirHead);
//	printf("%d\n", DirHead->type);
//	printf("%d\n", DirHead->nodenext->type);
//	printf("%d\n", DirHead->nodenext->next->type);
//	printf("%d\n", DirHead->nodenext->priority);
//	printf("%d\n", DirHead->nodenext->next->priority);
//	printf("%s\n", DirHead->nodenext->DownloadID);
//	printf("%s\n", DirHead->nodenext->next->DownloadID);		
//	printf("%s\n", DirHead->nodenext->next->addrnext->addr);		
//	printf("%s\n", DirHead->nodenext->addrnext->addr);
//	printf("%s\n", DirHead->nodenext->addrnext->next->addr);	     

//	fd = open("FDDF_2.xml", O_RDWR);		    	  	//xmlParse5
//  	if (fd < 0)  perror("Error fd");
//	Mu_FddfFileCreate(&filepp);					
//	Mu_Parse(PARSE_EYEWEAR_FDDF, fd,NULL, &filepp);
//	printf("TYPE %d\n", filepp->type);			
//	printf("NAME%s\n", filepp->name);			
//	printf("DISPLAYNAME%s\n", filepp->displayname);	
//	printf("IDX%d\n", filepp->databaseinfo->index);
//	printf("IDX%s\n", filepp->databaseinfo->program);	
	
//	fd = open("FDDF_2.xml", O_RDWR);		 		//xmlParse6   
//	if (fd < 0)  perror("Error fd");
//	Mu_SegmentHeadCreate(&segptr);
//	Mu_Parse(PARSE_SEGMENT, fd, "//Segment", &segptr);
//	printf("%d\n", segptr->total);
//	printf("%d\n", segptr->next->index);
//	printf("%d\n", segptr->next->next->index);		
//	printf("%d\n", segptr->next->next->next->index);		

//	fd = open("00_ServerInfo.xml", O_RDWR);				//xmlParse8
//  	if (fd < 0)  perror("Error fd");
//	Mu_ServiceInfoCreate(&servicepp); 
//	Mu_Parse(PARSE_SERVICE_INFO, fd, NULL, &servicepp);
//	printf("%d\n", servicepp->Protocolver);
//	printf("%s\n", servicepp->ping);
//	printf("%s\n", servicepp->lastserver);
//	printf("%d\n", servicepp->server->total);
//	printf("%s\n", servicepp->server->next->addr);
//	printf("%s\n", servicepp->server->next->next->addr);
//	printf("%s\n", servicepp->server->next->next->next->addr);
//	printf("%s\n", servicepp->apipathlist->login);

//	fd = open("update SRV.xml", O_RDWR);				//xmlParse9
//  	if (fd < 0)  perror("Error fd");
//	Mu_UpdateInfoCreate(&updatepp);		
//	Mu_Parse(PARSE_UPDATE_INFO, fd, NULL, &updatepp);
//	printf("%d\n", updatepp->srvsvrinfo->version);
//	printf("%s\n", updatepp->srvsvrinfo->checksum);	
//	printf("%d\n", updatepp->srvfirmsf->firmwareversion);	
//	printf("%s\n", updatepp->srvfirmsf->firmwarechecksum);
//	printf("%d\n", updatepp->srvfirmsf->hardwareversion);	

//	fd = open("resuming.xml", O_RDWR);				//xmlParse10
//  	if (fd < 0)  perror("Error fd");
//	Mu_ResumeInfoCreate(&Resumepp, 3);
//	Mu_Parse(PARSE_RESUME_INFO, fd, NULL, &Resumepp);
//	printf("%d\n", Resumepp->type);
//	printf("%s\n", Resumepp->name);
//	Resumepp++;
//	printf("%d\n", Resumepp->type);
//	printf("%s\n", Resumepp->name);
//	Resumepp++;
//	printf("%d\n", Resumepp->type);
//	printf("%s\n", Resumepp->name);

//	fd = open("file_info.xml", O_RDWR);				//xmlParse11
//  	if (fd < 0)  perror("Error fd");
//	Mu_FileInfoCreate(&back);
//	Mu_Parse(PARSE_FILE_INFO, fd, NULL, &back);
//	printf("%d\n", back->type);			
//	printf("%d\n", back->rate);			
//	printf("%s\n", back->displayname);		
//	printf("%d\n", back->datebaseinfo->index);	

//	fd = open("deviceinformation.xml", O_RDWR);			//xmlParse12
//  	if (fd < 0)  perror("Error fd");
//	Mu_XmlDeviceInfoCreate(&Devicepp);
//	Mu_Parse(PARSE_DEVICE_INFO, fd, NULL, &Devicepp);
//	printf("%s\n", Devicepp->IdInt);			
//	printf("%s\n", Devicepp->App);			
//	printf("%d\n", Devicepp->Hardwareversion);	

//	fd = open("03_After_Login.xml",O_RDWR);				//xmlParse13
//  	if (fd < 0)  perror("Error fd");
//	Mu_SRV_SvrInfCreate(&srvinfo);
//	Mu_Parse(PARSE_SRV_SVRINF, fd, NULL, &srvinfo);
//	printf("%d\n", srvinfo->type);			
//	printf("%s\n", srvinfo->ServerInfo);		
//	printf("%d\n", srvinfo->version1);
//	Mu_SRV_SvrInfFree(srvinfo);	


//	fd1=open("xmlcleart1", O_RDWR|O_CREAT);			//xmlCreate1	
//  	if (fd1 < 0)  perror("Error fd");
//	Mu_BuildXML(BUILD_SIGNAL_ADP, fd1, querypp, NULL);

//	fd1=open("xml2", O_RDWR|O_CREAT);			//xmlCreate2
//  	if (fd1 < 0)  perror("Error fd");
//	Mu_BuildXML(BUILD_FILE_DES, fd1, filepp, NULL); 
	
//	fd1=open("xml3", O_RDWR|O_CREAT);			//xmlCreate3	
//  	if (fd1 < 0)  perror("Error fd");
//	Mu_DBUInfoPtrCreate(&dbuinfoptr);
//	Mu_BuildXML(BUILD_SIGQUE_902, fd1, querypp, dbuinfoptr);

//	fd1=open("xml4", O_RDWR|O_CREAT);			//xmlCreate4	
//  	if (fd1 < 0)  perror("Error fd");
//	Mu_DBIInfoPtrCreate(&dbiptr);
//	Mu_BuildXML(BUILD_SIGQUE_20X, fd1, querypp, dbiptr);

//	fd1=open("xml6", O_RDWR|O_CREAT);			//xmlCreate6
//  	if (fd1 < 0)  perror("Error fd");
//	Mu_DBUDirtPtrCreate(&dbudirt);
//	Mu_BuildXML(BUILD_SIGUP_DN903, fd1, querypp, dbudirt);	

//	fd1=open("xml7", O_RDWR|O_CREAT);			//xmlCreate7	
//  	if (fd1 < 0)  perror("Error fd");
//	Mu_DBURatePtrCreate(&dburate);
//	Mu_BuildXML(BUILD_SIGUP_FILE, fd1, querypp, dburate);
					
//	fd1=open("xml8", O_RDWR|O_CREAT);			//xmlCreate8
//  	if (fd1 < 0)  perror("Error fd");
//	Mu_BuildXML(BUILD_DEVICE_INFO, fd1, Devicepp, NULL);	
	
//	fd1=open("xml9", O_RDWR|O_CREAT);			//xmlCreate9
//  	if (fd1 < 0)  perror("Error fd");
//	Mu_BuildXML(BUILD_UPDATE_INFO, fd1, updatepp, NULL);	

	close(fd);
//	close(fd1);
	printf("completely finish\n");
	return 0;
}
