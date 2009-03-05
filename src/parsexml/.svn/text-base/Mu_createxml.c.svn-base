#include "../include/Mu_error.h"
#include "../include/Mu_xml_parse.h"
#include "../include/Mu_Creatxml.h"
#include "../include/Mu_Struct_main.h"
#include <unistd.h>

/*write int to xmlfile*/
int Mu_WriteIntToXML(int class, int fp, const char *element, int val) 
{ 
	char temp[51]; 
	char temp2[1024];
	memset(temp, 0, 51); 
	memset(temp2, 0, 1024); 
 	//add tabstop 
  	while(--class) 
   		strncat(temp, "\t", 1); 
    	if(strlen(temp) >= 50) 
     		return MUNBUF; 
      	if(val) 
	{
       		sprintf(temp2, "%s<%s>%d</%s>\n", temp, element, val, element); 
		write(fp, temp2, strlen(temp2));
	}
        else 
	{
         	sprintf(temp2, "%s<%s></%s>\n", temp, element, element); 
		write(fp, temp2, strlen(temp2));
        }

        return MUOK; 
} 

/*write string to xmlfile*/
int Mu_WriteStrToXML(int class, int fp, const char *element, const char *val) 
{ 
	char temp[51]; 
	char temp2[1024];
	memset(temp, 0, 51); 
	memset(temp2, 0, 1024); 
 	//add tabstop 
 	while(--class) 
	{
 		strncat(temp, "\t", 1); 
	}
 	if((strlen(temp)) >= 50) 
	{
 		return MUNBUF; 
	}
 	sprintf(temp2, "%s<%s>%s</%s>\n", temp, element, (val?val:""), element); 
	write(fp, temp2, strlen(temp2));

 	return MUOK; 
} 

/*initialize  DBIInfoPtr*/
int Mu_DBIInfoPtrInit(DBIInfoPtr Ptr)
{
	Ptr->text = NULL;
	Ptr->type = 0;
	Ptr->newer = 0;
    	Ptr->older = 0;
    	Ptr->program = NULL;
    	Ptr->group = NULL;
    	Ptr->owner = NULL;
    	Ptr->priority = 0;

	return MUOK;
}

/*free  DBIInfoPtr*/
int Mu_DBIInfoPtrFree(DBIInfoPtr Ptr)
{
	Mu_Free(Ptr->text);
    	Mu_Free(Ptr->program);
    	Mu_Free(Ptr->group);
    	Mu_Free(Ptr->owner);
	free(Ptr);
	return MUOK;
}

/*create DBIInfoPtr and initialize it*/
int Mu_DBIInfoPtrCreate(DBIInfoPtr * Ptr)
{
    	*Ptr = (DBIInfoPtr)malloc(sizeof(DBIInfo));
    	if(NULL ==  *Ptr)
    	{
        	Mu_ErrorPrint();
		return MUNBUF;
	}
	//init
	Mu_DBIInfoPtrInit(*Ptr);
	return MUOK;
}

/*initialize DBURatePtr*/
int Mu_DBURatePtrInit(DBURatePtr Ptr)
{
    	Ptr->filerate = 0;
	Ptr->index = 0;
    	Ptr->program = NULL;
    	Ptr->group = NULL;
    	Ptr->owner = NULL;

	return MUOK;
}

/*initialize DBURatePtr*/
int Mu_DBURatePtrFree(DBURatePtr Ptr)
{
    	Mu_Free(Ptr->program);
    	Mu_Free(Ptr->group);
    	Mu_Free(Ptr->owner);
	free(Ptr);
	return MUOK;
}

/*create DBURatePtr and initialize it*/
int Mu_DBURatePtrCreate(DBURatePtr * Ptr)
{
    	*Ptr = (DBURatePtr)malloc(sizeof(DBURate));
    	if(NULL == *Ptr)
   	{
        	Mu_ErrorPrint();
		return MUNBUF;
	}
	//init
	Mu_DBURatePtrInit(*Ptr);
	return MUOK;
}

/*initialize DBUInfoPtr*/
int Mu_DBUInfoPtrInit(DBUInfoPtr Ptr)
{
	Ptr->newer = 0;
	Ptr->older = 0;
	Ptr->program = NULL;
	Ptr->group = NULL;
	Ptr->owner = NULL;
	Ptr->status = 0;

	return MUOK;
}

/*free DBUInfoPtr*/
int Mu_DBUInfoPtrFree(DBUInfoPtr Ptr)
{
	Mu_Free(Ptr->program);
	Mu_Free(Ptr->group);
	Mu_Free(Ptr->owner);
	free(Ptr);
	return MUOK;
}

/*create DBUInfoPtr and initialize it*/
int Mu_DBUInfoPtrCreate(DBUInfoPtr * Ptr)
{
    	*Ptr = (DBUInfoPtr)malloc(sizeof(DBUInfo));
    	if(NULL == *Ptr)
    	{
        	Mu_ErrorPrint();
		return MUNBUF;
	}
	//init
	Mu_DBUInfoPtrInit(*Ptr);
	return MUOK;
}

/*initialize DBUDirtPtr*/
int Mu_DBUDirtPtrInit(DBUDirtPtr Ptr)
{
	Ptr->download = NULL;
	Ptr->dat2 = NULL;
	Ptr->dat3 = NULL;
	Ptr->dat4 = NULL;
	Ptr->dat5 = NULL;
	Ptr->status = 0;
	
	return MUOK;
}

/*free DBUDirtPtr*/
int Mu_DBUDirtPtrFree(DBUDirtPtr Ptr)
{
	Mu_Free(Ptr->download);
	Mu_Free(Ptr->dat2);
	Mu_Free(Ptr->dat3);
	Mu_Free(Ptr->dat4);
	Mu_Free(Ptr->dat5);
	free(Ptr);
	return MUOK;
}

/*create DBUDirtPtr and initialize it*/
int Mu_DBUDirtPtrCreate(DBUDirtPtr *Ptr)
{
	*Ptr = (DBUDirtPtr)malloc(sizeof(DBUDirt));
    	if(NULL == *Ptr)
    	{
        	Mu_ErrorPrint();
		return MUNBUF;
	}
	//init
    	Mu_DBUDirtPtrInit(*Ptr);
    	return MUOK;
}

/*select the manner to create xmlfile*/
int Mu_BuildXML(int type, int fp, void *Ptr1, void *Ptr2)
{ 
	int retval = MUOK; 
	if(IS_FILEDES(type))
	{
 		retval = Mu_BuildFileDes(fp, (FddfFilePtr)Ptr1); 

 		return retval; 
	}
	else if(IS_DEVICEINFO(type))
	{ 
 		retval = Mu_BuildDeviceInfo(fp, (XmlDeviceInfoPtr)Ptr1); 

 		return retval; 
 	}
	else if(IS_UPDATEINFO(type))
	{ 
 		retval = Mu_BuildUpdateInfo(fp, (UpdateInfoPtr)Ptr1); 

 		return retval; 
 	}
	else if(IS_RESUMEINFO(type))
	{
		retval =Mu_BuildResume(fp, (ResumeInfoPtr)Ptr1);
		
		return retval;
	}
	else if(IS_SIGNAL(type))
	{ 
 		retval = Mu_BuildSignal(type, fp,Ptr1,Ptr2); 

 		return retval; 
 	}
	else 
	{	
 		return MUEERO;
	} 
} 

/*the interface of create devicexml,input the value of Ptr1 to devicexml*/
int Mu_BuildDeviceInfo(int fp, XmlDeviceInfoPtr Ptr1) 
{ 
	if(NULL == Ptr1)
	{ 
 		Mu_ErrorPrint();

 		return MUNBUF;
	} 
	//seek to the start of the file 
	ftruncate(fp, 0);
	lseek(fp, 0, SEEK_SET); 
 	WRITE_HEAD(fp); 
 	MU_WRITE_ELEMENT( fp, "<DeviceInfo>\n"); 

 	if((Mu_WriteStrToXML(3, fp, "Device_ID_Int", Ptr1->IdInt) < 0)
 	  ||(Mu_WriteStrToXML(3, fp, "Device_ID_App", Ptr1->App) < 0) 
 	  ||(Mu_WriteIntToXML(3, fp, 
 	  "Device_Fireversion", Ptr1->Hardwareversion)< 0))
	{ 
 		fprintf(stdout, "build device info error!\n"); 

 		return MUEERO;
 	} 
	MU_WRITE_ELEMENT(fp, "</DeviceInfo>"); 

	return MUOK; 
}

/*the interface of create updatexml,input the value of Ptr1 to updatexml*/
int Mu_BuildUpdateInfo(int fp, UpdateInfoPtr Ptr1)
{
	if(NULL == Ptr1)
	{
		Mu_ErrorPrint();
	
		return MUNBUF;
	}
	//seek to the start of the file 
	ftruncate(fp, 0);
	lseek(fp, 0, SEEK_SET);
	WRITE_HEAD(fp);
	MU_WRITE_ELEMENT( fp, "<Update_Information>\n");
	MU_WRITE_ELEMENT( fp, "\t");
	MU_WRITE_ELEMENT( fp, "<SRV_SvrInf>\n");

	if((Mu_WriteIntToXML(5, fp, "SvrInfo_version", Ptr1->srvsvrinfo->version)<0)
	  ||(Mu_WriteStrToXML(5, fp, "SvrInfo_checksum", Ptr1->srvsvrinfo->checksum<0)))
	{
		Mu_ErrorPrint();

		return MUNNOD;
	}
	MU_WRITE_ELEMENT( fp, "\t");
	MU_WRITE_ELEMENT( fp, "</SRV_SvrInf>\n");
	MU_WRITE_ELEMENT( fp, "\t");
	MU_WRITE_ELEMENT( fp, "<SRV_FirmSF>\n");

	if((Mu_WriteIntToXML(5, fp, "Fireware_version", Ptr1->srvfirmsf->firmwareversion)<0)
	  ||(Mu_WriteStrToXML(5, fp, "Fireware_checksum", Ptr1->srvfirmsf->firmwarechecksum)<0)
	  ||(Mu_WriteIntToXML(5, fp, "Hardware_version", Ptr1->srvfirmsf->hardwareversion)<0))
	{
		Mu_ErrorPrint();

		return MUNNOD;
	}
	MU_WRITE_ELEMENT( fp, "\t");
	MU_WRITE_ELEMENT( fp, "</SRV_FirmSF>\n");
	MU_WRITE_ELEMENT( fp, "</Update_Information>"); 

	return MUOK; 	
}	 

/*the interface of create fileinfo xml,input the value of Ptr1 to fileinfo xml*/
int Mu_BuildFileDes(int fp, FddfFilePtr Ptr1) 
{
	
	if(NULL == Ptr1)
	{
		Mu_ErrorPrint();
	
		return MUNBUF;
	}
	//seek to the start of the file 
	ftruncate(fp, 0);
	lseek(fp, 0, SEEK_SET);
	WRITE_HEAD(fp);
	
 	MU_WRITE_ELEMENT( fp, "<File_Infomation>\n"); 

	if((Mu_WriteStrToXML(2, fp, "File_Name", Ptr1->name)<0)
	  ||(Mu_WriteIntToXML(2, fp, "File_Size", Ptr1->size)<0)
	  ||(Mu_WriteStrToXML(2, fp, "File_HMD5", Ptr1->md5)<0)
	  ||(Mu_WriteStrToXML(2, fp, "File_CCRC", Ptr1->crc)<0)
	  ||(Mu_WriteIntToXML(2, fp, "File_Type", Ptr1->type)<0)
	  ||(Mu_WriteIntToXML(2, fp, "File_Rate", Ptr1->rate)<0)
	  ||(Mu_WriteStrToXML(2, fp, "FileDisplayName", Ptr1->displayname)<0)
	  ||(Mu_WriteStrToXML(2, fp, "FileDescription", Ptr1->description)<0)
	  ||(Mu_WriteStrToXML(2, fp, "ValidBeforeDate", Ptr1->validbeforedate)<0)
	  ||(Mu_WriteStrToXML(2, fp, "ValidAfterWatch", Ptr1->validafterwatch)<0))
	{
		Mu_ErrorPrint();

		return MUNNOD;
	}
	MU_WRITE_ELEMENT( fp, "\t");
	MU_WRITE_ELEMENT( fp, "<Database_Info>\n");

	if((Mu_WriteIntToXML(3, fp, "ID_IDX", Ptr1->databaseinfo->index)<0)
	  ||(Mu_WriteStrToXML(3, fp, "ID_PRG", Ptr1->databaseinfo->program)<0)
	  ||(Mu_WriteStrToXML(3, fp, "ID_GRP", Ptr1->databaseinfo->group)<0)
	  ||(Mu_WriteStrToXML(3, fp, "ID_OWN", Ptr1->databaseinfo->owner)<0))
	{
		Mu_ErrorPrint();

		return MUNNOD;
	}
	MU_WRITE_ELEMENT( fp, "\t");
	MU_WRITE_ELEMENT( fp, "</Database_Info>\n");
 	MU_WRITE_ELEMENT( fp, "</File_Infomation>"); 

	return MUOK;
}

/*the interface of create resume xml,input the value of Ptr1 to resume xml*/
int Mu_BuildResume(int fp, ResumeInfoPtr Ptr1) 
{
	if(NULL == Ptr1)
	{
		Mu_ErrorPrint();
	
		return MUNBUF;
	}
	//seek to the start of the file 
	ftruncate(fp, 0);
	lseek(fp, 0, SEEK_SET);
	WRITE_HEAD(fp);
 	MU_WRITE_ELEMENT( fp, "<resuming>\n"); 
 	MU_WRITE_ELEMENT( fp, "\t\t");
	MU_WRITE_ELEMENT( fp, "<uncomplete>\n");

	if((Mu_WriteIntToXML(6, fp, "type", Ptr1->type)<0)
	  ||(Mu_WriteStrToXML(6, fp, "name", Ptr1->name)<0))
	{
		Mu_ErrorPrint();
	
		return MUNNOD;
	}

 	MU_WRITE_ELEMENT(fp, "\t\t");
	MU_WRITE_ELEMENT(fp,"</uncomplete>\n");
	MU_WRITE_ELEMENT(fp, "</resuming>");

	return MUOK;
}	

/*select the right create function according to type*/
int Mu_BuildSignal(int type, int fp, void *Ptr1, void *Ptr2) 
{ 
 	int retval = MUOK; 
	if(IS_SIGADP(type))
	{ 
 			retval = Mu_BuildSignalADP(fp, (EyewearADPPtr)Ptr1); 
 			return retval; 
 	}
	else if(IS_SIGUPFILE(type))//901
	{ 
 		retval = Mu_BuildSignalUpFile(fp, (EyewearADPPtr)Ptr1, (DBURatePtr)Ptr2); 
 		return retval; 
 	}
	else if(IS_SIGUPDN(type))
	{
 		retval = Mu_BuildSignalUpDn(type, fp, Ptr1, Ptr2); 
 		return retval; 
 	}
	else if(IS_SIGQUE(type))
	{ 
 		retval = Mu_BuildSignalQuery(type, fp, Ptr1, Ptr2); 
 		return retval; 
 	}
	else 
		 return MUEERO;

} 

/*create a xml file struct except payload segment*/
int Mu_BuildSignalADP(int fp, EyewearADPPtr Ptr1)
{
	if((NULL ==Ptr1))
	{ 
 		Mu_ErrorPrint();
 		return MUNBUF; 
 	} 
	ftruncate(fp, 0);
 	lseek(fp, 0, SEEK_SET); 
 	WRITE_HEAD(fp); 
	MU_WRITE_ELEMENT(fp, "<Eyewear_ADP>\n"); 
 	if(Mu_WriteIntToXML(2, fp, "Protocol_Ver", Ptr1->Protocolver) < 0 
 	  ||Mu_WriteIntToXML(2,fp, "Protocol_OPC", Ptr1->ProtocolOPC) < 0) 
	{
 		Mu_ErrorPrint();
		return MUNNOD;
	}
 	MU_WRITE_ELEMENT(fp, "\t<PreLogin>\n");
 	if(Mu_WriteIntToXML(3, fp, 
	  "EncryptionMethod", Ptr1->prelogin->encryptionmethod)< 0 
	  ||Mu_WriteStrToXML(3,fp, "Challenge", Ptr1->prelogin->challenge)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</PreLogin>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Device_Info>\n"); 

 	if(Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_Int", Ptr1->device_info->DeviceIDInt) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_APP", Ptr1->device_info->DeviceIDApp) < 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Device_Status", Ptr1->device_info->devicestatus)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Trig_Resource", Ptr1->device_info->trigresource) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "SRV_Date_Time", Ptr1->device_info->SRVDateTime)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
	  "Fireware_Version", Ptr1->device_info->firmwareversion) < 0 
	  ||Mu_WriteIntToXML(3, fp, 
 	  "Hardware_Version", Ptr1->device_info->hardwareversion) < 0
	  ||Mu_WriteIntToXML(3, fp,
	  "ServerInfo_Step", Ptr1->device_info->serverinfostep) < 0)
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</Device_Info>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Conversation>\n"); 

	if(Mu_WriteStrToXML(3, fp, 
	  "ConversationID", Ptr1->conversation->ConversationID)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthUsername", Ptr1->conversation->AuthUsername)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthPassword", Ptr1->conversation->AuthPassword)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</Conversation>\n"); 
 	MU_WRITE_ELEMENT(fp, "</Eyewear_ADP>"); 
  
 	return MUOK; 
}
/*select the right create function according to type*/
int Mu_BuildSignalQuery(int type, int fp, void *Ptr1, void *Ptr2) 
{ 
	int retval = MUOK; 
 	if(IS_SIG902(type))
	{ 
 		retval = Mu_BuildSignalUpDn902(fp, (EyewearADPPtr)Ptr1, (DBUInfoPtr)Ptr2); 
 		return retval; 
 	}
	else if(IS_SIG20X(type))
	{ 
 		retval = Mu_BuildSignalQuery20X(fp, (EyewearADPPtr)Ptr1, (DBIInfoPtr)Ptr2); 
 		return retval; 
 	}
	else 

 		return MUEERO;
} 

/*select the right create function according to type*/
int Mu_BuildSignalUpDn(int type, int fp, void *Ptr1, void *Ptr2) 
{ 
	int retval = MUOK; 
 	if(IS_SIG902(type))
	{ 
 		retval = Mu_BuildSignalUpDn902(fp, (EyewearADPPtr)Ptr1, (DBUInfoPtr)Ptr2); 
 		return retval; 
 	}
	else if(IS_SIG903(type))
	{ 
 		retval = Mu_BuildSignalUpDn903(fp, (EyewearADPPtr)Ptr1, (DBUDirtPtr)Ptr2); 
 		return retval; 
 	}
	else 

 		return MUEERO;
} 

/*create 902 xml file*/
int Mu_BuildSignalUpDn902(int fp, EyewearADPPtr Ptr1, DBUInfoPtr Ptr2) 
{ 
	if((NULL ==Ptr1) || (NULL == Ptr2))
	{ 
 		Mu_ErrorPrint();
 		return MUNBUF; 
 	} 
	ftruncate(fp, 0);
 	lseek(fp, 0, SEEK_SET); 
 	WRITE_HEAD(fp); 
	MU_WRITE_ELEMENT(fp, "<Eyewear_ADP>\n"); 

 	if(Mu_WriteIntToXML(2, fp, "Protocol_Ver", Ptr1->Protocolver) < 0 
 	||Mu_WriteIntToXML(2,fp, "Protocol_OPC", Ptr1->ProtocolOPC) < 0) 
	{
 		Mu_ErrorPrint();
		return MUNNOD;
	}
 	MU_WRITE_ELEMENT(fp, "\t<PreLogin>\n"); 

 	if(Mu_WriteIntToXML(3, fp, 
	  "EncryptionMethod", Ptr1->prelogin->encryptionmethod)< 0 
	  ||Mu_WriteStrToXML(3,fp, "Challenge", Ptr1->prelogin->challenge)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</PreLogin>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Device_Info>\n"); 

 	if(Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_Int", Ptr1->device_info->DeviceIDInt) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_APP", Ptr1->device_info->DeviceIDApp) < 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Device_Status", Ptr1->device_info->devicestatus)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Trig_Resource", Ptr1->device_info->trigresource) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "SRV_Date_Time", Ptr1->device_info->SRVDateTime)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
	  "Fireware_Version", Ptr1->device_info->firmwareversion) < 0 
	  ||Mu_WriteIntToXML(3, fp, 
 	  "Hardware_Version", Ptr1->device_info->hardwareversion) < 0
	  ||Mu_WriteIntToXML(3, fp,
	  "ServerInfo_Step", Ptr1->device_info->serverinfostep) < 0)
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</Device_Info>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Conversation>\n"); 

	if(Mu_WriteStrToXML(3, fp, 
	  "ConversationID", Ptr1->conversation->ConversationID)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthUsername", Ptr1->conversation->AuthUsername)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthPassword", Ptr1->conversation->AuthPassword)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</Conversation>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Payload>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t\t<Item>\n"); 

	if(Mu_WriteIntToXML(5, fp, "Type", 902)< 0 
 	  ||Mu_WriteStrToXML(5, fp, "Text", "DBU_Info") < 0 
 	  ||Mu_WriteIntToXML(5, fp, "Dat1", Ptr2->newer)<0 
 	  ||Mu_WriteIntToXML(5, fp, "Dat2", Ptr2->older)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat3", Ptr2->program)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat4", Ptr2->group)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat5", Ptr2->owner)<0 
 	  ||Mu_WriteIntToXML(5, fp, "Dat6", Ptr2->status)<0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t\t</Item>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t</Payload>\n"); 
 	MU_WRITE_ELEMENT(fp, "</Eyewear_ADP>"); 
  
 	return MUOK; 
} 

/*create 903 xml file*/
int Mu_BuildSignalUpDn903(int fp, EyewearADPPtr Ptr1, DBUDirtPtr Ptr2) 
{ 
	if((NULL ==Ptr1) || (NULL == Ptr2))
	{ 
 		Mu_ErrorPrint();
 		return MUNBUF; 
 	} 
	ftruncate(fp, 0);
 	lseek(fp, 0, SEEK_SET); 
 	WRITE_HEAD(fp); 
	MU_WRITE_ELEMENT(fp, "<Eyewear_ADP>\n"); 
 	if(Mu_WriteIntToXML(2, fp, "Protocol_Ver", Ptr1->Protocolver) < 0 
 	  ||Mu_WriteIntToXML(2,fp, "Protocol_OPC", Ptr1->ProtocolOPC) < 0) 
	{
 		Mu_ErrorPrint();
		return MUNNOD;
	}
 	MU_WRITE_ELEMENT(fp, "\t<PreLogin>\n"); 

 	if(Mu_WriteIntToXML(3, fp, 
	  "EncryptionMethod", Ptr1->prelogin->encryptionmethod)< 0 
	  ||Mu_WriteStrToXML(3,fp, "Challenge", Ptr1->prelogin->challenge)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</PreLogin>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Device_Info>\n"); 

 	if(Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_Int", Ptr1->device_info->DeviceIDInt) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_APP", Ptr1->device_info->DeviceIDApp) < 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Device_Status", Ptr1->device_info->devicestatus)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Trig_Resource", Ptr1->device_info->trigresource) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "SRV_Date_Time", Ptr1->device_info->SRVDateTime)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
	  "Fireware_Version", Ptr1->device_info->firmwareversion) < 0 
	  ||Mu_WriteIntToXML(3, fp, 
 	  "Hardware_Version", Ptr1->device_info->hardwareversion) < 0
	  ||Mu_WriteIntToXML(3, fp,
	  "ServerInfo_Step", Ptr1->device_info->serverinfostep) < 0)
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</Device_Info>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Conversation>\n"); 

	if(Mu_WriteStrToXML(3, fp, 
	  "ConversationID", Ptr1->conversation->ConversationID)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthUsername", Ptr1->conversation->AuthUsername)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthPassword", Ptr1->conversation->AuthPassword)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</Conversation>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Payload>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t\t<Item>\n"); 

	if(Mu_WriteIntToXML(5, fp, "Type", 903)< 0 
 	  ||Mu_WriteStrToXML(5, fp, "Text", "DBU_Dirt") < 0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat1", Ptr2->download)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat2", Ptr2->dat2)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat3", Ptr2->dat3)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat4", Ptr2->dat4)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat5", Ptr2->dat5)<0 
 	  ||Mu_WriteIntToXML(5, fp, "Dat6", Ptr2->status)<0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t\t</Item>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t</Payload>\n"); 
 	MU_WRITE_ELEMENT(fp, "</Eyewear_ADP>"); 
  
 	return MUOK; 
} 

/*create 20X xml file*/
int Mu_BuildSignalQuery20X(int fp, EyewearADPPtr Ptr1, DBIInfoPtr Ptr2) 
{ 
	if((NULL ==Ptr1) || (NULL == Ptr2))
	{ 
 		Mu_ErrorPrint();
 		return MUNBUF; 
 	} 
	ftruncate(fp, 0);
 	lseek(fp, 0, SEEK_SET); 
 	WRITE_HEAD(fp); 
	MU_WRITE_ELEMENT(fp, "<Eyewear_ADP>\n"); 

 	if(Mu_WriteIntToXML(2, fp, "Protocol_Ver", Ptr1->Protocolver) < 0 
 	  ||Mu_WriteIntToXML(2,fp, "Protocol_OPC", Ptr1->ProtocolOPC) < 0) 
	{
 		Mu_ErrorPrint();
		return MUNNOD;
	}
 	MU_WRITE_ELEMENT(fp, "\t<PreLogin>\n"); 

 	if(Mu_WriteIntToXML(3, fp, 
	  "EncryptionMethod", Ptr1->prelogin->encryptionmethod)< 0 
	  ||Mu_WriteStrToXML(3,fp, "Challenge", Ptr1->prelogin->challenge)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</PreLogin>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Device_Info>\n"); 

 	if(Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_Int", Ptr1->device_info->DeviceIDInt) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_APP", Ptr1->device_info->DeviceIDApp) < 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Device_Status", Ptr1->device_info->devicestatus)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Trig_Resource", Ptr1->device_info->trigresource) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "SRV_Date_Time", Ptr1->device_info->SRVDateTime)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
	  "Fireware_Version", Ptr1->device_info->firmwareversion) < 0 
	  ||Mu_WriteIntToXML(3, fp, 
 	  "Hardware_Version", Ptr1->device_info->hardwareversion) < 0
	  ||Mu_WriteIntToXML(3, fp,
	  "ServerInfo_Step", Ptr1->device_info->serverinfostep) < 0)
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</Device_Info>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Conversation>\n"); 

	if(Mu_WriteStrToXML(3, fp, 
	  "ConversationID", Ptr1->conversation->ConversationID)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthUsername", Ptr1->conversation->AuthUsername)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthPassword", Ptr1->conversation->AuthPassword)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</Conversation>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Payload>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t\t<Item>\n"); 
    
	if(Mu_WriteIntToXML(5, fp, "Type", Ptr2->type)< 0 
 	  ||Mu_WriteStrToXML(5, fp, "Text", Ptr2->text) <0
 	  ||Mu_WriteIntToXML(5, fp, "Dat1", Ptr2->newer)<0 
 	  ||Mu_WriteIntToXML(5, fp, "Dat2", Ptr2->older)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat3", Ptr2->program)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat4", Ptr2->group)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat5", Ptr2->owner)<0 
 	  ||Mu_WriteIntToXML(5, fp, "Dat6", Ptr2->priority)<0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t\t</Item>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t</Payload>\n"); 
 	MU_WRITE_ELEMENT(fp, "</Eyewear_ADP>"); 
  
 	return MUOK; 
}

/*create 901 xml file*/
int Mu_BuildSignalUpFile(int fp, EyewearADPPtr Ptr1, DBURatePtr Ptr2) 
{ 
	if((NULL ==Ptr1) || (NULL == Ptr2))
	{ 
 		Mu_ErrorPrint();
 		return MUNBUF; 
 	} 
	ftruncate(fp, 0);
 	lseek(fp, 0, SEEK_SET); 
 	WRITE_HEAD(fp); 
	MU_WRITE_ELEMENT(fp, "<Eyewear_ADP>\n"); 

 	if(Mu_WriteIntToXML(2, fp, "Protocol_Ver", Ptr1->Protocolver) < 0 
 	  ||Mu_WriteIntToXML(2,fp, "Protocol_OPC", Ptr1->ProtocolOPC) < 0) 
	{
 		Mu_ErrorPrint();
		return MUNNOD;
	}
 	MU_WRITE_ELEMENT(fp, "\t<PreLogin>\n"); 

 	if(Mu_WriteIntToXML(3, fp, 
	  "EncryptionMethod", Ptr1->prelogin->encryptionmethod)< 0 
	  ||Mu_WriteStrToXML(3,fp, "Challenge", Ptr1->prelogin->challenge)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</PreLogin>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Device_Info>\n"); 

 	if(Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_Int", Ptr1->device_info->DeviceIDInt) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "Device_ID_APP", Ptr1->device_info->DeviceIDApp) < 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Device_Status", Ptr1->device_info->devicestatus)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
 	  "Trig_Resource", Ptr1->device_info->trigresource) < 0 
 	  ||Mu_WriteStrToXML(3, fp, 
 	  "SRV_Date_Time", Ptr1->device_info->SRVDateTime)< 0 
 	  ||Mu_WriteIntToXML(3, fp, 
	  "Fireware_Version", Ptr1->device_info->firmwareversion) < 0 
	  ||Mu_WriteIntToXML(3, fp, 
 	  "Hardware_Version", Ptr1->device_info->hardwareversion) < 0
	  ||Mu_WriteIntToXML(3, fp,
	  "ServerInfo_Step", Ptr1->device_info->serverinfostep) < 0)
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 		
 	MU_WRITE_ELEMENT(fp, "\t</Device_Info>\n"); 
 	MU_WRITE_ELEMENT(fp, "\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Conversation>\n"); 

	if(Mu_WriteStrToXML(3, fp, 
	  "ConversationID", Ptr1->conversation->ConversationID)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthUsername", Ptr1->conversation->AuthUsername)< 0 
 	  ||Mu_WriteStrToXML(3, fp, 
	  "AuthPassword", Ptr1->conversation->AuthPassword)< 0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	} 
		
 	MU_WRITE_ELEMENT(fp, "\t</Conversation>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t<Payload>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t\t<Item>\n"); 

	if((Mu_WriteIntToXML(5, fp, "Type", 901)< 0) 
 	  ||Mu_WriteStrToXML(5, fp, "Text", "DBU_Rate") < 0 
 	  ||Mu_WriteIntToXML(5, fp, "Dat1", Ptr2->filerate)<0 
 	  ||Mu_WriteIntToXML(5, fp, "Dat2", Ptr2->index)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat3", Ptr2->program)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat4", Ptr2->group)<0 
 	  ||Mu_WriteStrToXML(5, fp, "Dat5", Ptr2->owner)<0) 
	{
 		Mu_ErrorPrint();
 		return MUNNOD; 
 	}
 		
 	MU_WRITE_ELEMENT(fp, "\t\t</Item>\n"); 
 	MU_WRITE_ELEMENT(fp, "\t</Payload>\n"); 
 	MU_WRITE_ELEMENT(fp, "</Eyewear_ADP>"); 
  
 	return MUOK; 
}
