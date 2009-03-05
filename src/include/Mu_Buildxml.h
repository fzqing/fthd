#ifndef __Mu_CREATXML_H__
#define __Mu_CREATXML_H__
#include "Mu_Struct_main.h"
#include "Mu_Creatxml.h"
#include <linux/unistd.h>

#define BUILD_SERV_INFO 1 
#define BUILD_UPDATE_INFO 2 
#define BUILD_DEVICE_INFO 3 
#define BUILD_FILE_DES 4 
#define BUILD_SIGNAL_ADP     23
//use to  mark construct normal non-payload

#define BUILD_SIGUP_FILE 39
#define BUILD_SIGUP_DN902 159 
#define BUILD_SIGUP_DN903 823
//use to  mark  construct status update

#define BUILD_SIGQUE_20X 1095
#define BUILD_SIGQUE_902 567                    
//use to mark  construct query

#define IS_SERVINFO(x) (((x)&0x0F)==1) 
#define IS_UPDATEINFO(x) (((x)&0x0F)==2) 
#define IS_DEVICEINFO(x) (((x)&0x0F)==3) 
#define IS_FILEDES(x) (((x)&0x0F)==4) 
#define IS_SIGNAL(x) (((x)&0x0F)==7) 
//use to detect the type of xml user want to create

#define IS_SIGADP(x) ((((x)&0x0F0)>>4)==1) 
#define IS_SIGUPFILE(x) (((((x)&0x0F0)>>4))==2) 
#define IS_SIGUPDN(x) ((((x)&0x0F0)>>4)==3) 
#define IS_SIGQUE(x) ((((x)&0x0F0)>>4)==4) 
//use to detect the type user want to create

#define IS_SIG901(x) ((((x)&0x0F00)>>8)==1) 
#define IS_SIG902(x) ((((x)&0x0F00)>>8)==2) 
#define IS_SIG903(x) ((((x)&0x0F00)>>8)==3) 
#define IS_SIG20X(x) ((((x)&0x0F00)>>8)==4) 
//use to detect the type of payload user want to create

#define MU_WRITE_ELEMENT(fp, string) \
	do{\
 		write(fp, string, strlen(string));\
	}while(0) 
//create a empty node mark

#define WRITE_HEAD(fp)\
	do{\
 		write(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n", 40);\
 	}while(0) 
//the declaration of xml top

//20X xml file structure

//fuction interface

/*write int to xmlfile*/
int Mu_WriteIntToXML(int class, int fp, const char *element, int val) ;
/*write string to xmlfile*/
int Mu_WriteStrToXML(int class, int fp, const char *element, const char *val);
/*select the manner to create xmlfile*/
int Mu_BuildXML(int type, int fp, void *Ptr1, void *Ptr2);
/*the interface of create devicexml,input the value of Ptr1 to devicexml*/
int Mu_BuildDeviceInfo(int fp, DeviceInfoPtr Ptr1);
/*the interface of create updatexml,input the value of Ptr1 to updatexml*/
int Mu_BuildUpdateInfo(int fp, UpdateInfoPtr Ptr1);
/*the interface of create fileinfo xml,input the value of Ptr1 to fileinfo xml*/
int Mu_BuildFileDes(int fp, FddfFilePtr Ptr1) ;
/*the interface of create resume xml,input the value of Ptr1 to resume xml*/
int Mu_BuildResume(int fp, ResumeInfoPtr Ptr1);
/*select the right create function according to type*/
int Mu_BuildSignal(int type, int fp, void * Ptr1, void * Ptr2);
/*create a xml file struct except payload segment*/
int Mu_BuildSignalADP(int fp, EyewearADPPtr Ptr2);
/*select the right create function according to type*/
int Mu_BuildSignalQuery(int type, int fp, void *Ptr1, void *Ptr2);
/*select the right create function according to type*/
int Mu_BuildSignalUpDn(int type, int fp, void *Ptr1, void *Ptr2);
/*create 902 xml file*/
int Mu_BuildSignalUpDn902(int fp, EyewearADPPtr Ptr1, DBUInfoPtr Ptr2);
/*create 903 xml file*/
int Mu_BuildSignalUpDn903(int fp, EyewearADPPtr Ptr1, DBUDirtPtr Ptr2);
/*create 20X xml file*/
int Mu_BuildSignalQuery20X(int fp, EyewearADPPtr Ptr1, DBIInfoPtr Ptr2);
/*create 901 xml file*/
int Mu_BuildSignalUpFile(int fp, EyewearADPPtr Ptr1, DBURatePtr Ptr2);


#endif
