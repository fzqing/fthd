#ifndef __MU_STRUCT_MAIN_H__
#define __MU_STRUCT_MAIN_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <iconv.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#define SIZE 4
#define MU_NAME_MAX 256
#define MAX_ADDR 50

typedef struct _FddfStatusNode{
	int type;		//mark the Item type of the FDDF
	char *text;		//fddf node text
	int size;		//fddf node size
	int newer;		//index new
	int older;		//index older
	char *program;		//ID-program
	char *group;		//ID-group
	char *owner;		//ID-owner
	int priority;		//Download Priority
	struct _FddfStatusNode *next;	
}FddfStatusNode,*FddfStatusNodePtr;

typedef struct {
	int type;			//Item Type of the FDDF
	int total;			//total number of this Item Type
	FddfStatusNodePtr next;
}FddfStatusHead,*FddfStatusHeadPtr;

typedef struct _DirDownloadAddr{
	char *addr;			//pointer, point to some heap, store the address
	struct _DirDownloadAddr *next;
}DirDownloadAddr,*DirDownloadAddrPtr;

typedef struct _DirStatusNode{
	int type;			//Item Type of the DIR download
	int authmethod;			//Authorization Method
	int priority;			//Download priority
	char *DownloadID;		//ID-download
	int total;			//total numbers of address
	DirDownloadAddrPtr  addrnext;	
	DirDownloadAddrPtr  addrlast;
	struct _DirStatusNode * next;
}DirStatusNode,*DirStatusNodePtr;

typedef struct{
	int type;			//item type
	int total;			//total number of the type(DirStatusNode)
	DirStatusNodePtr nodenext;
}DirStatusHead,*DirStatusHeadPtr;

typedef struct _ServerAddr{
	char *addr;			//pointer, point to some heap, store the address
	struct _ServerAddr * next;	
}ServerNode,*ServerNodePtr;

typedef struct _ServerHead{
	int total;	//total number of the address
	ServerNodePtr cur;
	ServerNodePtr next;
	ServerNodePtr last;
}ServerHead, *ServerHeadPtr;	

typedef struct _Apipathlist{
	char *prelogin;		//apipathlist prelogin
	char *login;			//apipathlist login
	char *querystatus;		//apipathlist querystatus
	char *queryfddflist;		//apipathlist queryfddflist
	char *updatestatus;		//apipathlist updatestatus
	char *logout;			//apipathlist logout
}ApiPathList,*ApiPathListPtr;

typedef struct _ServiceInfo{
	int Protocolver;		//serviceinfo protocolversion
	int stepping;			//serviceinfo setpping
	ServerHeadPtr server;		
	char *lastserver;		//serviceinfo lastserver
	char *ping;			//serviceinfo ping
	ApiPathListPtr apipathlist;
}ServiceInfo,*ServiceInfoPtr;

typedef struct _FddfListNode{
	char *addr;			//FddflistNode addr
	struct _FddfListNode * next;
}FddfListNode,*FddfListNodePtr;

typedef struct _FddfListHead{
	int type;			//FddfListHead type
	char *text;			//FddfListHead text
	int total;			//FddfListHead node num
	int authmethod;			//FddfListHead authmethod
	int priority;			//FddfListHead priority
	int size;
	FddfListNodePtr next;		
	FddfListNodePtr last;
}FddfListHead,*FddfListHeadPtr;

typedef struct _SegmentNode{
	int index;			//SegmentNode index
	unsigned long size;		//SegmentNode size
	char *name;			//SegmentNode name
	char *md5;			//SegmentNode md5
	char *crc;			//SegmentNode crc
	struct _SegmentNode *next;
}SegmentNode,*SegmentNodePtr;

typedef struct _SegmentHead{
	int total;			//SegmentHead Node num
	SegmentNodePtr next;
	SegmentNodePtr last;
}SegmentHead,*SegmentHeadPtr;

typedef struct _DatabaseInfo{
	int index;			//DatabaseInfo index
	char *program;			//DatabaseInfo program
	char *group;			//DatabaseInfo group
	char *owner;			//DatabaseInfo owner
}DatabaseInfo,*DatabaseInfoPtr;

typedef struct _EyewareFDDF{
	char *name;			//FddfFile name
	unsigned long size;		//FddfFile size
	char *md5;			//FddfFile md5
	char *crc;			//FddfFile crc
	int type;			//FddfFile type
	int rate;			//FddfFile rate
	char *displayname;		//FddfFile displayname
	char *description;		//FddfFile description
	char *validbeforedate;		//FddfFile validbeforedate
	char *validafterwatch;		//FddfFile validafterwatch
	DatabaseInfoPtr databaseinfo;	
	char *NonDefaultServer;	//FddfFile NonDefaultServer
	int Authorizemethod;		//FddfFile Authorizemethod
	char *serverfilepath;		//FddfFile serverfilepath
	int filesegmentnum;		//FddfFile filesegment
}FddfFile,*FddfFilePtr;

typedef struct _PreLogin{
	int encryptionmethod;		//PreLogin encryptionmethod
	char *challenge;		//PreLogin challenge
}PreLogin,*PreLoginPtr;

typedef struct _DeviceInfo{
	char *DeviceIDInt;		//DeviceInfo DeviceIDInt
	char *DeviceIDApp;		//DeviceInfo DeviceIDApp	
	int devicestatus;		//DeviceInfo devicestatus
	int trigresource;		//DeviceInfo trigresource
	char *SRVDateTime;		//DeviceInfo SRVDateTime
	int firmwareversion;		//DeviceInfo firmwareversion
	int hardwareversion;		//DeviceInfo hardwareversion
	int serverinfostep;		//DeviceInfo serverinfostep
}DeviceInfo,*DeviceInfoPtr;

typedef struct _Coversation{
	char *ConversationID;		//Conversation ConversationID
	char *AuthUsername;		//Conversation AuthUsername
	char *AuthPassword;		//Conversation AuthPassword
}Conversation,*ConversationPtr;

typedef struct _EyewareADP{
	int Protocolver;		//EyewearADP Protocolver
	int ProtocolOPC;		//EyewearADP ProtocolOPC
	PreLoginPtr prelogin;		
	DeviceInfoPtr device_info;
	ConversationPtr conversation;	
}EyewearADP,*EyewearADPPtr;

typedef struct _SRVSvrInfo{
	int version;			//SRVSvrInfo version
	char *checksum;		//SRVSvrInfo checksum
}SRVSvrInfo,*SRVSvrInfoPtr;

typedef struct _SRVFirmSF{
	int firmwareversion;		//SRVFirmSF firmwareversion
	char *firmwarechecksum;	//SRVFirmSF firmwarechecksum
	int hardwareversion;		//SRVFirmSF hardwareversion
}SRVFirmSF,*SRVFirmSFPtr;

typedef struct _UpdateInfo{
	SRVSvrInfoPtr srvsvrinfo;
	SRVFirmSFPtr srvfirmsf;
}UpdateInfo,*UpdateInfoPtr;

typedef struct _XmlDeviceInfo{
	char *IdInt;			//XmlDeviceInfo IdInt
	char *App;			//XmlDeviceInfo App
	int Hardwareversion;		//XmlDeviceInfo Hardwareversion
}XmlDeviceInfo,*XmlDeviceInfoPtr;

typedef struct _ResumeInfo{
	int type;			//ResumeInfo type
	char name[MU_NAME_MAX+1];	//ResumeInfo resumename
}ResumeInfo,*ResumeInfoPtr;

typedef struct _FileInfo{
	char *name;			//FileInfo name
	unsigned long size;		//FileInfo size
	char *md5;			//FileInfo md5
	char *crc;			//FileInfo crc
	int type;			//FileInfo type
	int rate;			//FileInfo rate
	char *displayname;		//FileInfo displayname
	char *description;		//FileInfo description
	char *validbeforedate;		//FileInfo validbeforedate
	char *validafterwatch;		//FileInfo validafterwatch
	DatabaseInfoPtr datebaseinfo;	//FileInfo datebaseinfo
	int userrate;			//FileInfo userrate
	int userfreq;			//FileInfo userfreq
}FileInfo,*FileInfoPtr;

typedef struct	_SRVDeviceInfo{
    	int type;		//Item Payload type
	char *text;
	int size;
    	int method;		//Authorization Method
    	char *ServerInfo;	//File Path and Name (@Server)
    	char *version1;		//File Stepping	or Firmware version
    	char *version2;		//Reserved number or Hardware Version
    	char *Md5Num;		//Checksum (MD5)
}SRV_SvrInf,SRV_FirmSF;
typedef SRV_SvrInf *SRV_SvrInfPtr;
typedef SRV_FirmSF *SRV_FirmSFPtr;

//20X xml file structure
typedef struct _DBIInfo{
	int type;
	char *text;
	int newer;
	int older;
	char *program;
	char *group;
	char *owner;
	int priority;
}DBIInfo, *DBIInfoPtr;

//901 xml file structure
typedef struct _DBURate{
	int filerate;
	int index;
	char *program;
	char *group;
	char *owner;
}DBURate, *DBURatePtr;

//902 xml file structure
typedef struct _DBUInfo{
	int newer;
	int older;
	char *program;
	char *group;
	char *owner;
	int status;
}DBUInfo, *DBUInfoPtr;

//903 xml file structure
typedef struct _DBUDirt{
	char *download; //ID-Download
	char *dat2; 	//Reserved		
	char *dat3; 	//Reserved
	char *dat4; 	//Reserved
	char *dat5; 	//Reserved
	int status; 	//status flag
}DBUDirt, *DBUDirtPtr;

#define Mu_Free(note)\
do{\
        if(NULL != note)\
	{\
        	free(note);\
        	note = NULL;\
	}\
}while(0)

#define Mu_StrncpyInt(note, string, name, place)\
do{\
	if(!xmlStrcmp(name, place))\
		note = atoi((char*)string);\
}while(0) 

#define Mu_StrncpyLong(note, string, name, place)\
do{\
	if(!xmlStrcmp(name, place))\
		note = (unsigned long)atoi((char*)string);\
}while(0) 

#define Mu_StrncpyStr(note, string, name, place)\
do{\
	if(!xmlStrcmp(name, place))\
	{\
 		if(NULL != ((note = malloc(xmlStrlen(string) + 1))))\
		{\
			memset(note, '\0', (xmlStrlen(string) + 1));\
			strncpy(note, (char*)string, xmlStrlen(string));\
		}\
	}\
}while(0) 

#define CHECK_FDDF_LIST(num)\
	((num == 301)||(num == 302)||(num == 303)||(num == 304))


/*the list of reinitialize*/
int Mu_SRV_SvrInfClear(SRV_SvrInfPtr Ptr);
int Mu_FddfStatusHeadClear(FddfStatusHeadPtr Ptr);
int Mu_FddfStatusNodeClear(FddfStatusNodePtr Ptr);
int Mu_DirStatusHeadClear(DirStatusHeadPtr Ptr);
int Mu_DirStatusNodeClear(DirStatusNodePtr Ptr);
int Mu_DirDownloadAddrClear(DirDownloadAddrPtr Ptr);
int Mu_ServiceInfoClear(ServiceInfoPtr Ptr);
int Mu_ServerHeadClear(ServerHeadPtr Ptr);
int Mu_ServerNodeClear(ServerNodePtr Ptr);
int Mu_FddfListHeadClear(FddfListHeadPtr Ptr);
int Mu_FddfListNodeClear(FddfListNodePtr Ptr);
int Mu_SegmentHeadClear(SegmentHeadPtr Ptr);
int Mu_SegmentNodeClear(SegmentNodePtr Ptr);
int Mu_FddfFileClear(FddfFilePtr Ptr);
int Mu_EyewearADPClear(EyewearADPPtr Ptr);
int Mu_UpdateInfoClear(UpdateInfoPtr Ptr);
int Mu_FileInfoClear(FileInfoPtr Ptr);
int Mu_XmlDeviceInfoClear(XmlDeviceInfoPtr Ptr);


/*create SRV_SvrInfPtr and init it*/
int Mu_SRV_SvrInfCreate(SRV_SvrInfPtr *Ptr);
/*init SRV_SvrInfPtr*/
int Mu_SRV_SvrInfinit(SRV_SvrInfPtr Ptr);
/*create SRV_FirmSFPtr and init it*/
int Mu_SRV_FirmSFCreate(SRV_FirmSFPtr *Ptr);
/*init SRV_FirmSFPtr*/
int Mu_SRV_FirmSFinit(SRV_FirmSFPtr Ptr);
/*create FddfStatusHeadPtr and init it*/
int Mu_FddfStatusHeadCreate(FddfStatusHeadPtr *Ptr);
/*free FddfStatusHeadPtr*/
int Mu_FddfStatusHeadFree(FddfStatusHeadPtr Ptr); 
/*copy payload inforamation from xml*/
int Mu_SRV_SvrInfStrncpy(SRV_SvrInfPtr Node, const xmlChar *string, const xmlChar *name);
/*initialize FddfStatusHeadPtr*/
int Mu_FddfStatusHeadInit(FddfStatusHeadPtr Ptr);
/*create a new FddfStatusNodePtr and initialize it*/
int Mu_FddfStatusNodeCreate(FddfStatusNodePtr *Ptr);
/*free FddfStatusNodePtr*/
int Mu_FddfStatusNodeFree(FddfStatusNodePtr Ptr);
/*initialize FddfStatusNodePtr*/
int Mu_FddfStatusNodeInit(FddfStatusNodePtr Ptr);
/*insert a FddfStatusNodePtr into FddfStatusHeadPtr */
int Mu_FddfStatusNodeInsert(FddfStatusHeadPtr Head, FddfStatusNodePtr Node);
/*delete a FddfStatusNodePtr*/
int Mu_FddfStatusNodeDelete(FddfStatusHeadPtr Head,FddfStatusNodePtr Node);
/*copy the relevant data from relevant xml to FddfStatusNodePtr*/
int Mu_FddfStatusNodeStrncpy(FddfStatusNodePtr Node, const xmlChar *string, const xmlChar *name);
/*create a new DirStatusHeadPtr and initialize it*/
int Mu_DirStatusHeadCreate(DirStatusHeadPtr *Ptr);
/*free DirStatusHeadPtr*/
int Mu_DirStatusHeadFree(DirStatusHeadPtr Ptr);
/*initialize DirStatusHeadPtr*/
int Mu_DirStatusHeadInit(DirStatusHeadPtr Ptr);
/*create a new DirStatusNodePtr and initialize it*/
int Mu_DirStatusNodeCreate(DirStatusNodePtr *Ptr);
/*initialize DirStatusNodePtr*/
int Mu_DirStatusNodeInit(DirStatusNodePtr Ptr);
/*insert DirStatusNodePtr into DirStatusHeadPtr*/
int Mu_DirStatusNodeInsert(DirStatusHeadPtr head,DirStatusNodePtr node);
/*create a new DirDownloadAddrPtr and initialize it*/
int Mu_DirStatusAddrCreate(DirDownloadAddrPtr *Ptr);
/*initialize DirDownloadAddrPtr*/
int Mu_DirDownloadAddrInit(DirDownloadAddrPtr Ptr);
/*insert DirDownloadAddrPtr into DirStatusNodePtr*/
int Mu_DirDownloadAddrInsert(DirStatusNodePtr Node, DirDownloadAddrPtr addr);
/*delete a DirStatusNodePtr*/
int Mu_DirDownloadAddrDeleteone(DirStatusNodePtr Node,DirDownloadAddrPtr addrs);
/*delete all DirStatusNodePtr except the headnode */
int Mu_DirDownloadAddrDeleteList(DirStatusNodePtr Node);
/*delete a DirStatusNodePtr*/
int Mu_DirStatusNodeDelete(DirStatusHeadPtr Head, DirStatusNodePtr Node);
/*copy the relevant data from the relevant xml file to DirStatusNodePtr*/
int Mu_DirStatusNodeStrncpy(DirStatusNodePtr Node, const xmlChar *string,const xmlChar *name);
/*copy addr to DirDownloadAddrPtr*/
int Mu_DirDownloadAddrStrncpy(DirDownloadAddrPtr Node,xmlChar *string);
/*create a new ServiceInfoPtr and initialize it*/
int Mu_ServiceInfoCreate(ServiceInfoPtr *Ptr);
/*free ServiceInfoPtr*/
int Mu_ServiceInfoFree(ServiceInfoPtr Ptr);
/*initialize ServiceInfoPtr*/
int Mu_ServiceInfoInit(ServiceInfoPtr Ptr);
/*initialize ServerHeadPtr*/
int Mu_ServerHeadInit(ServerHeadPtr Ptr);
/*create a new ServerNodePtr and initialize it*/
int Mu_ServerNodeCreate(ServerNodePtr *Ptr);
/*initialize ServerNodePtr*/
int Mu_ServerNodeInit(ServerNodePtr Ptr);
/*insert ServerNodePtr into ServerHeadPtr */
int Mu_ServerNodeInsert(ServerHeadPtr Ptr,ServerNodePtr Node);
/*copy the relevant data from ServerInfo.XML to ServiceInfoPtr*/
int Mu_ServiceInfoStrncpy(ServiceInfoPtr Ptr,const xmlChar *string,const xmlChar *name);
/*copy addr from ServerInfo.XML to ServerNodePtr*/
int Mu_ServerNodeStrncpy(ServerNodePtr Ptr,const xmlChar *string,const xmlChar *name);
/*create a new FddfListHeadPtr and initialize it*/
int Mu_FddfListHeadCreate(FddfListHeadPtr *Ptr);
/*free FddfListHeadPtr*/
int Mu_FddfListHeadFree(FddfListHeadPtr Ptr);
/*create a new FddfListNodePtr and initialize it*/
int Mu_FddfListNodeCreate(FddfListNodePtr *Ptr);
/*initialize FddfListHeadPtr */
int Mu_FddfListHeadInit(FddfListHeadPtr Ptr);
/*initialize FddfListNodePtr*/
int Mu_FddfListNodeInit(FddfListNodePtr Ptr);
/*insert FddfListNodePtr into FddfListHeadPtr*/
int Mu_FddfListNodeInsert(FddfListHeadPtr Head, FddfListNodePtr Node);
/*delete FddfListHeadPtr from firsthead*/
int Mu_FddfListNodeDelete(FddfListHeadPtr Head);
/*copy the relevant data from after_queryFDDF.xml to FddfListHeadPtr*/
int Mu_FddfLisitHeadStrncpy(FddfListHeadPtr Head, const xmlChar *string, const xmlChar *name);
/*copy the relevant data from after_queryFDDF.xml to FddfListNodePtr*/
int Mu_FddfListNodeStrncpy(FddfListNodePtr Node, xmlChar *string, const xmlChar *name);
/*create a new FddfFilePtr and initialize it*/
int Mu_FddfFileCreate(FddfFilePtr *Ptr);
 /*free FddfFilePtr*/
int Mu_FddfFileFree(FddfFilePtr Ptr);
/*create a new SegmentHeadPtr and initialize it*/
int Mu_SegmentHeadCreate(SegmentHeadPtr *Ptr);
/*free SegmentHeadPtr*/
int Mu_SegmentHeadFree(SegmentHeadPtr Ptr);
/*create a new SegmentNodePtr and initialize it*/
int Mu_SegmentNodeCreate(SegmentNodePtr *Ptr);
/*initialize FddfFilePtr*/
int Mu_FddfFileInit(FddfFilePtr Ptr);
/*initialize SegmentHeadPtr*/
int Mu_SegmentHeadInit(SegmentHeadPtr Ptr);
/*initialize SegmentNodePtr*/
int Mu_SegmentNodeInit(SegmentNodePtr Ptr);
/*insert SegmentNodePtr into SegmentHeadPtr*/
int Mu_SegmentNodeInsert(SegmentHeadPtr Head, SegmentNodePtr Node);
/*delete SegmentHeadPtr*/
int Mu_SegmentNodeDelete(SegmentHeadPtr Head);
/*copy the relevant from file_info.xml to FddfFilePtr*/
int Mu_FddfFileStrncpy(FddfFilePtr Ptr, const xmlChar *string, const xmlChar *name);
/*copy the relevant data from the Segment of FDDF2.xml to SegmentNodePtr*/
int Mu_SegmentNodeStrncpy(SegmentNodePtr Ptr, const xmlChar *string, const xmlChar *name);
/*create a new EyewearADPPtr and initialize it*/
int Mu_EyewearADPCreate(EyewearADPPtr *Ptr);
/*free EyewearADPPtr*/
int Mu_EyewearADPFree(EyewearADPPtr Ptr);
/*initialize EyewearADPPtr*/
int Mu_EyewearADPInit(EyewearADPPtr Ptr);
/*copy the relevant data from login.xml to EyewearADPPtr*/
int Mu_EyewearADPStrncpy(EyewearADPPtr Ptr, const xmlChar *string, const xmlChar *name);
/*create a new UpdateInfoPtr and initialize it*/
int Mu_UpdateInfoCreate(UpdateInfoPtr *Ptr);
/*free UpdateInfoPtr*/
int Mu_UpdateInfoFree(UpdateInfoPtr Ptr); 
/*delete MuUpdateInfo*/
int Mu_UpdateInfoDelete(UpdateInfoPtr Ptr);
/*copy the relevant data from updateSRV.xml to UpdateInfoPtr*/
int Mu_UpdateInfoStrncpy(UpdateInfoPtr Ptr, const xmlChar *string, const xmlChar *name);
/*copy the relevant data from deviecinformation.xml to XmlDeviceInfoPtr*/
int Mu_DeviceInfoStrncpy(XmlDeviceInfoPtr Ptr, const xmlChar *string, const xmlChar *name);
/*initialize ResumeInfoPtr*/
int Mu_ResumeInfoInit(ResumeInfoPtr Ptr, int num);
/*create ResumeInfoPtr and init it*/
int Mu_ResumeInfoCreate(ResumeInfoPtr * Ptr,int num);
/*insert and copy data to ResumeInfoPtr*/
int Mu_ResumeInsert(ResumeInfoPtr Ptr,int type,xmlChar *name,int num,const xmlChar *string);
/*create a new FileInfoPtr and initialize it*/
int Mu_FileInfoCreate(FileInfoPtr *Ptr);
/*free FileInfoPtr*/
int Mu_FileInfoFree(FileInfoPtr Ptr);
/*copy the relevant data from fileinfo.xml to FileInfoPtr*/
int Mu_FileInfoStrncpy(FileInfoPtr Ptr, xmlChar *string, const xmlChar *name);
/*initialize XmlDeviceInfoPtr*/
int Mu_XmlDeviceInfoInit(XmlDeviceInfoPtr Ptr);
/*create a new XmlDeviceInfoPtr and initialize it*/
int Mu_XmlDeviceInfoCreate(XmlDeviceInfoPtr *Ptr);
/*free XmlDeviceInfoPtr*/
int Mu_XmlDeviceInfoFree(XmlDeviceInfoPtr Ptr);
/*free SRV_SvrInfPtr*/
int Mu_SRV_SvrInfFree(SRV_SvrInfPtr Ptr);

#endif
