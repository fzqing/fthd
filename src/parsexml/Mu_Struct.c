#include "../include/Mu_Struct_main.h"
#include "../include/Mu_error.h"
#include <iconv.h>
#include "../include/Mu_Util.h"

static char s_strBufOut[1024];
/*convert character nanner*/
char *d_ConvertCharset(char *cpEncodeFrom, char *cpEncodeTo, char *cpInput)
{
	char *cpOut;
	size_t iInputLen, iOutLen, iReturn;
	iconv_t c_pt;

 	if ((c_pt = iconv_open(cpEncodeTo, cpEncodeFrom)) == (iconv_t)-1)
 	{
   		Mu_ErrorPrint();
   		return NULL;
  	}
 	iconv(c_pt, NULL, NULL, NULL, NULL);

 	iInputLen = strlen(cpInput) + 1;
 	iOutLen = 1024;
 	cpOut = s_strBufOut;
 	iReturn = iconv(c_pt, &cpInput, &iInputLen, &cpOut, &iOutLen);

 	if (iReturn == -1) 
 	{
   		return NULL;
 	}
  
 	iconv_close(c_pt);
 	return s_strBufOut;
}

/*convert num to string*/
char * Convert(int n, char *strResult)  
{  
  	char *p = strResult;  

  	if(n / 10 != 0)  
	{
  		p = Convert(n/10,   p);  
	}

  	*p = n % 10 + '0';  
  	++p;  
  	*p = '\0';  
  	return p;  
}
	
/*reinitialize*/
int Mu_SRV_SvrInfClear(SRV_SvrInfPtr Ptr)
{
	Ptr->type = 0;
	Ptr->method = 0;
	Ptr->size = 0;
	Mu_Free(Ptr->text);
	Mu_Free(Ptr->ServerInfo);
	Mu_Free(Ptr->version1);   
	Mu_Free(Ptr->version2);   
	Mu_Free(Ptr->Md5Num);

	return MUOK;
}

/*free SRV_SvrInfPtr*/
int Mu_SRV_SvrInfFree(SRV_SvrInfPtr Ptr)
{
	Mu_Free(Ptr->text);
	Mu_Free(Ptr->ServerInfo);
	Mu_Free(Ptr->version1);   
	Mu_Free(Ptr->version2);   
	Mu_Free(Ptr->Md5Num);
	free(Ptr);

	return MUOK;
}

/*initialize SRV_SvrInfPtr*/
int Mu_SRV_SvrInfInit(SRV_SvrInfPtr Ptr)
{
	Ptr->type = 0;
	Ptr->size = 0;
	Ptr->text = NULL;
	Ptr->method = 0;
	Ptr->ServerInfo = NULL;
	Ptr->version1 = NULL;   
	Ptr->version2 = NULL;   
	Ptr->Md5Num = NULL;

	return MUOK;
}

/*create SRV_SvrInfPtr and init it*/
int Mu_SRV_SvrInfCreate(SRV_SvrInfPtr *Ptr)
{
	*Ptr = (SRV_SvrInfPtr)malloc(sizeof(SRV_SvrInf));

	if(NULL == *Ptr)
	{
		Mu_ErrorPrint();
		return MUNBUF;
	}
	//init
	Mu_SRV_SvrInfInit(*Ptr);
	
	return MUOK;
}

/*initialize SRV_FirmSFPtr*/
int Mu_SRV_FirmSFinit(SRV_FirmSFPtr Ptr)
{
	Ptr->type = 0;
	Ptr->size = 0;
	Ptr->text = NULL;
	Ptr->method = 0;
	Ptr->ServerInfo = NULL;
	Ptr->version1 = NULL;   
	Ptr->version2 = NULL;   
	Ptr->Md5Num = NULL;

	return MUOK;
}

/*create SRV_FirmSFPtr and init it*/
int Mu_SRV_FirmSFCreate(SRV_FirmSFPtr *Ptr)
{
	*Ptr = (SRV_FirmSFPtr)malloc(sizeof(SRV_FirmSF));

	if(NULL == *Ptr)
	{
		Mu_ErrorPrint();
		return MUNBUF;
	}
	//init
	Mu_SRV_FirmSFinit(*Ptr);
	
	return MUOK;
}

/*reinitialize*/
int Mu_FddfStatusHeadClear(FddfStatusHeadPtr Ptr)
{
//	int i;
//	for(i = 0;i < 4;i++)
//	{
		if(NULL == Ptr)
		{
			Mu_ErrorPrint();	
			return MUEERO;
		}
		Ptr->type = 201 + 0;
		Ptr->total = 0;
		Mu_Free(Ptr->next);
//		Ptr++;
//	}

	return MUOK;
}

/*initialize FddfStatusHeadPtr*/
int Mu_FddfStatusHeadInit(FddfStatusHeadPtr Ptr)
{
//	int i;
//	for(i = 0;i < 4;i++)
//	{
		if(NULL == Ptr)
		{
			Mu_ErrorPrint();	
			return MUEERO;
		}
		Ptr->type = 201 + 0;
		Ptr->total = 0;
		Ptr->next = NULL;
//		Ptr++;
//	}

	return MUOK;
}

/*free FddfStatusHeadPtr*/
int Mu_FddfStatusHeadFree(FddfStatusHeadPtr Ptr)
{
//	int i;
	FddfStatusNodePtr Node1 = NULL;
	FddfStatusNodePtr Node2 = NULL;
//	for(i = 0;i < 4;i++)
//	{
		if(NULL == Ptr)
		{
			Mu_ErrorPrint();	
			return MUEERO;
		}
		Node1 = Ptr->next;
		while(Node1 != NULL)
		{
			Node2 = Node1;
			Node1 = Node1->next;
			Mu_Free(Node2->text);
			Mu_Free(Node2->program); 
			Mu_Free(Node2->group); 
			Mu_Free(Node2->owner); 
			Mu_Free(Node2);
		}
//		Mu_Free(Ptr->next);
		free(Ptr);
//		Ptr++;
//	}

	return MUOK;
}

/*create FddfStatusHeadPtr and init it*/
int Mu_FddfStatusHeadCreate(FddfStatusHeadPtr * Ptr) 
{ 
	*Ptr = (FddfStatusHeadPtr)malloc(sizeof(FddfStatusNode) * 4); 

	if(NULL == *Ptr)
	{ 
			Mu_ErrorPrint();
			return MUNBUF; 
 	}	 
 	//init the head 
	Mu_FddfStatusHeadInit(*Ptr); 

  	return MUOK; 
}

/*copy payload inforamation from xml*/ 
int Mu_SRV_SvrInfStrncpy(SRV_SvrInfPtr Node, const xmlChar * string, const xmlChar * name)
{ 
	if((NULL == string)||(NULL == name) || (NULL == Node)) 
	{ 
		Mu_ErrorPrint();
		return MUEERO;
	}
 
	Mu_StrncpyInt(Node->type,string,name,(const xmlChar*)"Type");
	Mu_StrncpyInt(Node->size,string,name,(const xmlChar*)"Size");
	Mu_StrncpyStr(Node->text,string,name,(const xmlChar*)"Text");
	Mu_StrncpyInt(Node->method,string,name,(const xmlChar*)"Dat1");
	Mu_StrncpyStr(Node->ServerInfo,string,name,(const xmlChar*)"Dat2");
	Mu_StrncpyStr(Node->version1,string,name,(const xmlChar*)"Dat3");
	Mu_StrncpyStr(Node->version2,string,name,(const xmlChar*)"Dat4");
	Mu_StrncpyStr(Node->Md5Num,string,name,(const xmlChar*)"Dat5");
	
	return MUOK;
}

/*reinitialize*/
int Mu_FddfStatusNodeClear(FddfStatusNodePtr Ptr)
{
	Ptr->type = 0;
	Ptr->size = 0;
	Ptr->newer = 0;
	Ptr->older = 0;

	Mu_Free(Ptr->text);
	Mu_Free(Ptr->program); 
	Mu_Free(Ptr->group); 
	Mu_Free(Ptr->owner); 
	Ptr->priority = 0;
	Mu_Free(Ptr->next); 

	return MUOK; 
}

/*initialize FddfStatusNodePtr*/
int Mu_FddfStatusNodeInit(FddfStatusNodePtr Ptr)
{
	Ptr->type = 0;
	Ptr->size = 0;
	Ptr->newer = 0;
	Ptr->older = 0;

	Ptr->text = NULL;
	Ptr->program = NULL; 
	Ptr->group = NULL; 
	Ptr->owner = NULL; 
	Ptr->priority = 0; 
	Ptr->next = NULL; 

	return MUOK; 
}
 
/*free FddfStatusNodePtr*/
int Mu_FddfStatusNodeFree(FddfStatusNodePtr Ptr)
{
	Mu_Free(Ptr->text);
	Mu_Free(Ptr->program); 
	Mu_Free(Ptr->group); 
	Mu_Free(Ptr->owner); 
	Mu_Free(Ptr->next); 
	free(Ptr);

	return MUOK; 
}

/*create a new FddfStatusNodePtr and initialize it*/
int Mu_FddfStatusNodeCreate(FddfStatusNodePtr *Ptr)
{
	int back;
	*Ptr = (FddfStatusNodePtr)malloc(sizeof(FddfStatusNode));

	if(NULL	== *Ptr)
	{
		Mu_ErrorPrint();
		return MUNBUF;
	}

	back = Mu_FddfStatusNodeInit(*Ptr);


	return back;
}
	
/*insert a FddfStatusNodePtr into FddfStatusHeadPtr */
int Mu_FddfStatusNodeInsert(FddfStatusHeadPtr Head, FddfStatusNodePtr Node)
{ 
	FddfStatusNodePtr Primer = Head->next; 
	FddfStatusNodePtr back = NULL;
	if((NULL == Node) ||(NULL == Head)) 
	{
		Mu_ErrorPrint();
		return MUNNOD; 
	}

//	Head += (Node->type-201); 

	if(NULL == Primer) 
	{
	   	Head->next = Node;
		Head->total ++; 
	   	return MUOK;	
	}

	while(Primer != NULL)
	{ 
		back = Primer;
		Primer = Primer->next; 
	} 

	Node->next = back->next; 
	back->next = Node; 
	Head->total ++; 
	return MUOK; 
} 

/*delete a FddfStatusNodePtr*/
int Mu_FddfStatusNodeDelete(FddfStatusHeadPtr Head,FddfStatusNodePtr Node) 
{ 
	FddfStatusNodePtr Primer = Head->next; 
	FddfStatusNodePtr back = NULL; 

	if(Primer == Node)
	{
		Mu_Free(Node->text); 
		Mu_Free(Node->program); 
		Mu_Free(Node->group); 
		Mu_Free(Node->owner); 

		free(Node); 
		Head->next = Head ->next->next;
		return MUOK;
	}

	while((NULL != Primer) && (Primer != Node))
	{ 
		back = Primer; 
		Primer= Primer->next; 
 	} 

	if(NULL == Primer)
	{ 
		Mu_ErrorPrint();
		return MUEERO;
	} 

	back->next = Node->next; 
	Mu_Free(Node->text); 
	Mu_Free(Node->program); 
	Mu_Free(Node->group); 
	Mu_Free(Node->owner); 

	free(Node); 

	return MUOK; 
} 

/*copy the relevant data from relevant xml to FddfStatusNodePtr*/
int Mu_FddfStatusNodeStrncpy(FddfStatusNodePtr Node, const xmlChar *string, const xmlChar *name)
{ 
	if((NULL == string)||(NULL == name) || (NULL == Node)) 
	{ 
		Mu_ErrorPrint();
		return MUEERO;
	} 

	Mu_StrncpyInt(Node->type,string,name,(const xmlChar*)"Type");
	Mu_StrncpyStr(Node->text,string,name,(const xmlChar*)"Text");
	Mu_StrncpyInt(Node->size,string,name,(const xmlChar*)"Size");
	Mu_StrncpyInt(Node->newer,string,name,(const xmlChar*)"Dat1");
	Mu_StrncpyInt(Node->older,string,name,(const xmlChar*)"Dat2");
	Mu_StrncpyStr(Node->program,string,name,(const xmlChar*)"Dat3");
	Mu_StrncpyStr(Node->group,string,name,(const xmlChar*)"Dat4");
	Mu_StrncpyStr(Node->owner,string,name,(const xmlChar*)"Dat5");
	Mu_StrncpyInt(Node->priority,string,name,(const xmlChar*)"Dat6");
	return MUOK; 
}

/*reinitialize*/
int Mu_DirStatusHeadClear(DirStatusHeadPtr Ptr) 
{ 
//	int i;
//	for(i= 0; i < 4; i++)
//	{ 
		if(NULL == Ptr)
		{ 
			Mu_ErrorPrint();
			return MUEERO;
		} 
		Ptr->type = 401 + 0; 
		Ptr->total = 0; 
		Mu_Free(Ptr->nodenext); 
//		Ptr ++; 
//	} 

	return MUOK; 
} 

/*initialize DirStatusHeadPtr*/
int Mu_DirStatusHeadInit(DirStatusHeadPtr Ptr) 
{ 
//	int i;
//	for(i= 0; i < 4; i++)
//	{ 
		if(NULL == Ptr)
		{ 
			Mu_ErrorPrint();
			return MUEERO;
		} 
		Ptr->type = 401 + 0; 
		Ptr->total = 0; 
		Ptr->nodenext = NULL; 
//		Ptr ++; 
//	} 

	return MUOK; 
} 

/*free DirStatusHeadPtr*/ 
int Mu_DirStatusHeadFree(DirStatusHeadPtr Ptr) 
{ 
//	int i;
//	for(i= 0; i < 4; i++)
//	{ 
	DirStatusNodePtr Node1 = Ptr->nodenext;
	DirStatusNodePtr Node2 = NULL;
	DirDownloadAddrPtr Addr1 = NULL;
	DirDownloadAddrPtr Addr2 = NULL;
		
		if(NULL == Ptr)
		{ 
			Mu_ErrorPrint();
			return MUEERO;
		} 
		while(Node1 != NULL)
		{
			Node2 = Node1;
			Node1 = Node1->next;
			Addr1 = Node2->addrnext;
			while(Addr1 != NULL)
			{
				Addr2 = Addr1;
				Addr1 = Addr1->next;
				Mu_Free(Addr2->addr);
				Mu_Free(Addr2);
			}
			Mu_Free(Node2->DownloadID); 
	//		Mu_Free(Node2->addrnext); 
	//		Mu_Free(Node2->addrlast); 
			Mu_Free(Node2);
		}	
		free(Ptr);
//		Ptr ++; 
//	} 

	return MUOK; 
} 
/*create a new DirStatusHeadPtr and initialize it*/
int Mu_DirStatusHeadCreate(DirStatusHeadPtr *Ptr)
{
	*Ptr = (DirStatusHeadPtr)malloc(sizeof(DirStatusHead) * 4);

	if(NULL == *Ptr)
	{
		Mu_ErrorPrint();
		return MUNBUF;
	}
	//init head
	Mu_DirStatusHeadInit(*Ptr);

	return MUOK; 
}

/*reinitialize*/
int Mu_DirStatusNodeClear(DirStatusNodePtr Ptr) 
{ 
	if(NULL == Ptr) 
	{
		Mu_ErrorPrint();
		return MUNNOD; 
	}

	Ptr->type = 0; 
	Ptr->authmethod = 0; 
	Ptr->priority = 0; 
	Mu_Free(Ptr->DownloadID); 
	Ptr->total = 0; 
	Mu_Free(Ptr->addrnext); 
	Mu_Free(Ptr->addrlast); 
	Mu_Free(Ptr->next); 

	return MUOK; 
}

/*initialize DirStatusNodePtr*/
int Mu_DirStatusNodeInit(DirStatusNodePtr Ptr) 
{ 
	if(NULL == Ptr) 
	{
		Mu_ErrorPrint();
		return MUNNOD; 
	}

	Ptr->type = 0; 
	Ptr->authmethod = 0; 
	Ptr->priority = 0; 
	Ptr->DownloadID = NULL; 
	Ptr->total = 0; 
	Ptr->addrnext = NULL; 
	Ptr->addrlast = NULL; 
	Ptr->next = NULL; 

	return MUOK; 
}
 
/*create a new DirStatusNodePtr and initialize it*/
int Mu_DirStatusNodeCreate(DirStatusNodePtr * Ptr)	
{
	int back;
	*Ptr = (DirStatusNodePtr)malloc(sizeof(DirStatusNode));

	if(NULL == *Ptr)
	{
		Mu_ErrorPrint();
		return MUNBUF;
	}

	back = Mu_DirStatusNodeInit(*Ptr);

	return back;
}	

/*insert DirStatusNodePtr into DirStatusHeadPtr*/
int Mu_DirStatusNodeInsert(DirStatusHeadPtr head,DirStatusNodePtr node)
{
	DirStatusNodePtr p = head->nodenext; 

	if((NULL == node) ||(NULL == head)) 
	{
		Mu_ErrorPrint();
		return MUNNOD; 
	}


	if(NULL == p) 
	{
	   	head->nodenext = node;
		head->total ++; 
	   	return MUOK;	
	}

	node->next=p->next;		
	p->next = node; 
	head->total ++; 

	return MUOK; 
}

/*reinitialize*/
int Mu_DirDownloadAddrClear(DirDownloadAddrPtr Ptr) 
{ 
	if(NULL == Ptr)
	{ 
		Mu_ErrorPrint();
		return MUNNOD; 
	} 

	Mu_Free(Ptr->addr); 
	Mu_Free(Ptr->next); 

	return MUOK; 
}

/*initialize DirDownloadAddrPtr*/
int Mu_DirDownloadAddrInit(DirDownloadAddrPtr Ptr) 
{ 
	if(NULL == Ptr)
	{ 
		Mu_ErrorPrint();
		return MUNNOD; 
	} 

	Ptr->addr = NULL; 
	Ptr->next = NULL; 

	return MUOK; 
}

/*create a new DirDownloadAddrPtr and initialize it*/
int Mu_DirStatusAddrCreate(DirDownloadAddrPtr *Ptr)
{
	int back;
	*Ptr = (DirDownloadAddrPtr)malloc(sizeof(DirDownloadAddr));

	if(NULL == *Ptr)
	{
        	Mu_ErrorPrint();
		return MUNBUF;
	}

	back = Mu_DirDownloadAddrInit(*Ptr);

	return back;
}

/*insert DirDownloadAddrPtr into DirStatusNodePtr*/
int Mu_DirDownloadAddrInsert(DirStatusNodePtr Node, DirDownloadAddrPtr addr)
{ 
	DirDownloadAddrPtr Primer = Node->addrnext; 
	DirDownloadAddrPtr back = NULL;
	if((NULL == Node) ||(NULL == addr)) 
	{
		Mu_ErrorPrint();
		return MUNNOD; 
	}

	if(NULL == Primer) 
	{
	   	Node->addrnext = addr;
		Node->addrlast = addr;
		Node->total ++; 
	   	return MUOK;	
	}

	while(Primer != NULL)
	{
		back = Primer;
		Primer = Primer->next;
	}

	addr->next = back->next;
	back->next = addr; 
	Node->total ++; 	

	return MUOK;
}

/*delete a DirStatusNodePtr*/
int Mu_DirDownloadAddrDeleteone(DirStatusNodePtr Node,DirDownloadAddrPtr addrs) 
{ 
	DirDownloadAddrPtr p = Node->addrnext;
	DirDownloadAddrPtr back = NULL;

	if(p == addrs)
	{
		Node->addrnext = Node->addrnext->next;
		Mu_Free(addrs->addr);
		free(addrs);
	
		return MUOK;
	}
	
	while((NULL != p)&&(p !=  addrs))
	{
		back = p;
		p = p->next;
	}

	if(NULL == p)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	
	}

	back->next = addrs->next;
	Mu_Free(addrs->addr);
	free(addrs);
	return MUOK; 
}
 
/*delete all DirStatusNodePtr except the headnode */
int Mu_DirDownloadAddrDeleteList(DirStatusNodePtr Node) 
{ 
	DirDownloadAddrPtr address = NULL; 

	while(Node->addrnext != NULL)
	{ 
		if((address= Node->addrnext) == NULL) 
		{
			Mu_ErrorPrint();
			return MUNNOD; 
		}

		Node->addrnext = address->next; 

		if(address == Node->addrlast) 
		{
			Node->addrlast = NULL; 
		}

		Mu_Free(address->addr); 
		free(address); 
	} 

	return MUOK; 
} 

/*delete a DirStatusNodePtr*/ 
int Mu_DirStatusNodeDelete(DirStatusHeadPtr Head, DirStatusNodePtr Node) 
{ 
	DirStatusNodePtr Primer = Head->nodenext; 
	DirStatusNodePtr back = NULL; 
	Head += (Node->type -400); 

	if(Primer == Node)
	{
		Mu_Free(Node->DownloadID); 
		free(Node); 
		Head->nodenext = Head->nodenext->next;

		return MUOK;
	}
		
	while((Primer != NULL) && (Primer != Node))
	{
		back = Primer; 
		Primer= Primer->next; 
	}	
 
	if(Primer == NULL)
	{ 
		Mu_ErrorPrint();
		return MUNNOD; 
	} 

	back->next = Node ->next; 
	Mu_DirDownloadAddrDeleteList(Node); 
	Mu_Free(Node->DownloadID); 
	free(Node); 

	return MUOK; 
}

/*copy the relevant data from the relevant xml file to DirStatusNodePtr*/
int Mu_DirStatusNodeStrncpy(DirStatusNodePtr Node,const xmlChar * string,const xmlChar *name)
{
	if((NULL == string)||(NULL == name)||(NULL ==Node))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	Mu_StrncpyInt(Node->type,string,name,(const xmlChar*)"Type");
	Mu_StrncpyInt(Node->authmethod,string,name,(const xmlChar*)"Dat1");
	Mu_StrncpyInt(Node->priority,string,name,(const xmlChar*)"Dat2");
	Mu_StrncpyStr(Node->DownloadID,string,name,(const xmlChar*)"Dat3");
	
	return MUOK;
}

/*copy addr to DirDownloadAddrPtr*/ 
int Mu_DirDownloadAddrStrncpy(DirDownloadAddrPtr Node,xmlChar * string)
{
	if((NULL == string)||(NULL == Node))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	if(NULL == ((Node->addr=malloc(xmlStrlen(string)+1))))
	{
		Mu_ErrorPrint();
		return MUEERO;			
	}

	memset(Node->addr,0,xmlStrlen(string)+1);
	strncpy(Node->addr,(char*)string,xmlStrlen(string));

	return MUOK;
}

/*reinitialize*/
int Mu_ServiceInfoClear(ServiceInfoPtr Ptr) 
{ 
	/*Init*/ 
	Ptr->Protocolver = 0; 
	Ptr->stepping = 0; 
	Mu_Free(Ptr->lastserver); 
	Mu_Free(Ptr->ping); 
	/*ServerHead Head */
	Ptr->server->total = 0; 
	Mu_Free(Ptr->server->next); 
	Mu_Free(Ptr->server->last); 
	/*ApiPathList*/ 
	Mu_Free(Ptr->apipathlist->prelogin); 
	Mu_Free(Ptr->apipathlist->login); 
	Mu_Free(Ptr->apipathlist->querystatus); 
	Mu_Free(Ptr->apipathlist->updatestatus); 
	Mu_Free(Ptr->apipathlist->queryfddflist); 
	Mu_Free(Ptr->apipathlist->logout); 

	return MUOK; 
}

/*initialize ServiceInfoPtr*/
int Mu_ServiceInfoInit(ServiceInfoPtr Ptr) 
{ 
	/*Init*/ 
	Ptr->Protocolver = 0; 
	Ptr->stepping = 0; 
	Ptr->lastserver = NULL; 
	Ptr->ping = NULL; 
	/*ServerHead Head */
	Ptr->server->total = 0; 
	Ptr->server->next = NULL; 
	Ptr->server->last = NULL; 
	/*ApiPathList*/ 
	Ptr->apipathlist->prelogin = NULL; 
	Ptr->apipathlist->login = NULL; 
	Ptr->apipathlist->querystatus = NULL; 
	Ptr->apipathlist->updatestatus = NULL; 
	Ptr->apipathlist->queryfddflist = NULL; 
	Ptr->apipathlist->logout = NULL; 

	return MUOK; 
}

/*free ServiceInfoPtr*/
int Mu_ServiceInfoFree(ServiceInfoPtr Ptr) 
{ 
	ServerNodePtr Node1 = NULL;
	ServerNodePtr Node2 = NULL;

	Mu_Free(Ptr->lastserver); 
	Mu_Free(Ptr->ping); 
	Node1 = Ptr->server->next;
	while(Node1 != NULL)
	{
		Node2 = Node1;
		Node1 = Node1->next;
		Mu_Free(Node2->addr);
		Mu_Free(Node2);
	}	
	Mu_Free(Ptr->server); 
	Mu_Free(Ptr->apipathlist->prelogin); 
	Mu_Free(Ptr->apipathlist->login); 
	Mu_Free(Ptr->apipathlist->querystatus); 
	Mu_Free(Ptr->apipathlist->updatestatus); 
	Mu_Free(Ptr->apipathlist->queryfddflist); 
	Mu_Free(Ptr->apipathlist->logout); 
	Mu_Free(Ptr->apipathlist); 
	free(Ptr);
	return MUOK; 
}

/*create a new ServiceInfoPtr and initialize it*/
int Mu_ServiceInfoCreate(ServiceInfoPtr * Ptr)
{
	ServerHeadPtr head = NULL;
	ApiPathListPtr api = NULL;
	head = (ServerHeadPtr)malloc(sizeof(ServerHead));

	if(NULL == head)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	
	}

	api = (ApiPathListPtr)malloc(sizeof(ApiPathList));

	if(NULL == api)
	{
		Mu_ErrorPrint();
		return MUNBUF;
	}

	*Ptr = (ServiceInfoPtr)malloc(sizeof(ServiceInfo));

	if(NULL == *Ptr)
	{
		Mu_ErrorPrint();
		return MUNBUF;
	}

	(*Ptr)->server = head;
	(*Ptr)->apipathlist = api;

	Mu_ServiceInfoInit(*Ptr);

	return MUOK;
}

/*reinitialize*/
int Mu_ServerHeadClear(ServerHeadPtr Ptr)
{
	//init ServerHead
	Ptr->total = 0;
	Mu_Free(Ptr->next);
	Mu_Free(Ptr->last);
	Mu_Free(Ptr->cur);

	return MUOK;
}

/*initialize ServerHeadPtr*/
int Mu_ServerHeadInit(ServerHeadPtr Ptr)
{
	//init ServerHead
	Ptr->total = 0;
	Ptr->next = NULL;
	Ptr->last = NULL;
	Ptr->cur = NULL;

	return MUOK;
}

/*reinitialize*/
int Mu_ServerNodeClear(ServerNodePtr Ptr)
{
	Mu_Free(Ptr->addr);
	Mu_Free(Ptr->next);

	return MUOK;
}

/*initialize ServerNodePtr*/
int Mu_ServerNodeInit(ServerNodePtr Ptr)
{
	Ptr->addr = NULL;
	Ptr->next = NULL;

	return MUOK;
}
	
/*create a new ServerNodePtr and initialize it*/
int Mu_ServerNodeCreate(ServerNodePtr * Node)
{
	*Node = (ServerNodePtr)malloc(sizeof(ServerNode));
	if(NULL == *Node)
	{
		Mu_ErrorPrint();
		return MUNBUF;
	}

	Mu_ServerNodeInit(*Node);

	return MUOK;
}

/*insert ServerNodePtr into ServerHeadPtr */
int Mu_ServerNodeInsert(ServerHeadPtr Ptr,ServerNodePtr Node)
{
	ServerNodePtr Primer = Ptr->next; 
	ServerNodePtr back = NULL;
	if((NULL == Node) ||(NULL == Ptr)) 
	{
		Mu_ErrorPrint();
		return MUNNOD; 
	}


	if(NULL == Primer) 
	{
	   	Ptr->next = Node;
		Ptr->last = Node;
		Ptr->total ++; 
	   	return MUOK;	
	}
	
	while(Primer != NULL)
	{
		back = Primer;
		Primer = Primer->next;
	}

	Node->next = back->next;
	back->next = Node; 
//	Ptr->cur = back;
	Ptr->last = Node;
	Ptr->total ++; 
	return MUOK; 
}

/*copy the relevant data from ServerInfo.XML to ServiceInfoPtr*/
int Mu_ServiceInfoStrncpy(ServiceInfoPtr Ptr,const xmlChar * string,const xmlChar * name)
{
	
	if((NULL == string)||(NULL == name)||(NULL == Ptr))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	Mu_StrncpyInt(Ptr->Protocolver,string,name,(const xmlChar*)"Protocol_Ver");
	Mu_StrncpyInt(Ptr->stepping,string,name,(const xmlChar*)"Stepping");
	Mu_StrncpyStr(Ptr->ping,string,name,(const xmlChar*)"Ping");
	Mu_StrncpyStr(Ptr->lastserver,string,name,(const xmlChar*)"Last");
	Mu_StrncpyStr(Ptr->apipathlist->prelogin,string,name,(const xmlChar*)"Pre_Login");
	Mu_StrncpyStr(Ptr->apipathlist->login,string,name,(const xmlChar*)"Login");
	Mu_StrncpyStr(Ptr->apipathlist->querystatus,string,name,(const xmlChar*)"Query_Status");
	Mu_StrncpyStr(Ptr->apipathlist->queryfddflist,string,name,(const xmlChar*)"Query_FDDF_List");
	Mu_StrncpyStr(Ptr->apipathlist->updatestatus,string,name,(const xmlChar*)"Update_Status");
	Mu_StrncpyStr(Ptr->apipathlist->logout,string,name,(const xmlChar*)"Logout");
	
	return MUOK;
}

/*copy addr from ServerInfo.XML to ServerNodePtr*/
int Mu_ServerNodeStrncpy(ServerNodePtr Ptr,const xmlChar * string,const xmlChar * name)
{
	if((NULL ==string)||(NULL ==name)||(NULL == Ptr))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	Mu_StrncpyStr(Ptr->addr,string,name,(const xmlChar*)"Addr");

	return MUOK;
}

/*reinitialize*/
int Mu_FddfListHeadClear(FddfListHeadPtr Ptr) 
{ 
	Ptr->type = 0; 
	Ptr->text = 0; 
	Ptr->total = 0; 
	Ptr->size = 0;
	Ptr->authmethod= 0; 
	Ptr->priority = 0; 
	Mu_Free(Ptr->next); 
	Mu_Free(Ptr->last); 

	return MUOK; 
}

/*initialize FddfListHeadPtr */
int Mu_FddfListHeadInit(FddfListHeadPtr Ptr) 
{ 
	Ptr->type = 0; 
	Ptr->text = 0; 
	Ptr->total = 0; 
	Ptr->size = 0;
	Ptr->authmethod= 0; 
	Ptr->priority = 0; 
	Ptr->next = NULL; 
	Ptr->last = NULL; 

	return MUOK; 
}

/*free FddfListHeadPtr*/
int Mu_FddfListHeadFree(FddfListHeadPtr Ptr) 
{
	FddfListNodePtr temp = Ptr->next;
	FddfListNodePtr temp2 = NULL;
	
	while(temp != NULL)
	{
		temp2 = temp;
		temp = temp->next;
		Mu_Free(temp2->addr);
		free(temp2);
	}
	Mu_Free(Ptr->text);
	free(Ptr);
	return MUOK;
}

/*create a new FddfListHeadPtr and initialize it*/
int Mu_FddfListHeadCreate(FddfListHeadPtr *Ptr) 
{ 
	*Ptr = (FddfListHeadPtr)malloc(sizeof(FddfListHead)); 
	if(NULL == *Ptr)
	{ 
		Mu_ErrorPrint();
		return MUNNOD; 
	} 
	
	Mu_FddfListHeadInit(*Ptr);
	return MUOK; 
} 

/*reinitialize*/
int Mu_FddfListNodeClear(FddfListNodePtr Ptr) 
{ 
	Mu_Free(Ptr->addr);
        Mu_Free(Ptr->next);

	return MUOK;
}
 
/*initialize FddfListNodePtr*/
int Mu_FddfListNodeInit(FddfListNodePtr Ptr) 
{ 
	//Init
	Ptr->addr = NULL;
        Ptr->next = NULL;

	return MUOK;
}
 
/*create a new FddfListNodePtr and initialize it*/
int Mu_FddfListNodeCreate(FddfListNodePtr *Ptr) 
{ 
	int back;
	*Ptr=(FddfListNodePtr)malloc(sizeof(FddfListNode));

	if(NULL == *Ptr)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}	
 	 
	back=Mu_FddfListNodeInit(*Ptr); 

	return back;
} 

/*insert FddfListNodePtr into FddfListHeadPtr*/
int Mu_FddfListNodeInsert(FddfListHeadPtr Head, FddfListNodePtr Node) 
{ 
	FddfListNodePtr p = Head->next;
	FddfListNodePtr q = NULL;
	if((NULL == Node) ||(NULL == Head)) 
	{
		Mu_ErrorPrint();
		return MUNNOD; 
	}


	if(NULL == p)
	{
		Head->next = Node;
		Head->last = Node;
		Head->total++;
		return MUOK;
	}

	while(p != NULL)
	{
		q = p;
		p = p->next;
	}

	Node->next = q->next; 
	q->next = Node; 
	Head->total ++; 

	return MUOK;  
} 

/*delete FddfListHeadPtr from firsthead*/
int Mu_FddfListNodeDelete(FddfListHeadPtr Head) 
{ 
	FddfListNodePtr Node = Head->next; 
	FddfListNodePtr p = NULL;

	if(Node == NULL) 
	{
		printf("no node to del\n");
		return MUNNOD; 
	}

	if((Node != NULL) && (Node->next == NULL))
	{
		Mu_Free(Node->addr); 
		free(Node); 

		return MUOK; 
	}
	
	while(Node->next != NULL)
	{	
		p = Node;
		Node = Node->next;
	}

	p->next = NULL;
	Mu_Free(Node->addr); 
	free(Node); 

	return MUOK; 
} 

/*copy the relevant data from after_queryFDDF.xml to FddfListHeadPtr*/
int Mu_FddfLisitHeadStrncpy(FddfListHeadPtr Head, const xmlChar *string, const xmlChar *name) 
{ 
	if((NULL == Head)||(NULL == string)||(NULL == name))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	Mu_StrncpyInt(Head->type,string,name,(const xmlChar*)"Type");
	Mu_StrncpyStr(Head->text,string,name,(const xmlChar*)"Text");
	Mu_StrncpyInt(Head->size,string,name,(const xmlChar*)"Size");
	Mu_StrncpyInt(Head->authmethod,string,name,(const xmlChar*)"Dat1");
	Mu_StrncpyInt(Head->priority,string,name,(const xmlChar*)"Dat2");
	
	return MUOK;		
} 

/*copy the relevant data from after_queryFDDF.xml to FddfListNodePtr*/
int Mu_FddfListNodeStrncpy(FddfListNodePtr Node, xmlChar *string, const xmlChar *name) 
{
	char ss[7];
	static int i = 5;
	memset(ss,0,7);
//	static int i=0;
	sprintf(ss,"%s%d","Dat",i++);
//	char FDDFaddr[100][5] = {"Dat5","Dat6","Dat7","Dat8","Dat9","Dat10","Dat11","Dat12","Dat13","Dat14","Dat15","Dat16","Dat17","Dat18","Dat19","Dat20","Dat21","Dat22","Dat23","Dat24","Dat25","Dat26","Dat27","Dat28"};
	if((NULL == string)||(NULL == name) || (NULL == Node)) 
	{ 
		Mu_ErrorPrint();
		return MUNNOD; 
	} 
	
//	Mu_StrncpyStr(Node->addr, string, name, FDDFaddr[i]);
	Mu_StrncpyStr(Node->addr, string, name, (const xmlChar*)ss);	
//	i++;
	if(i >= MAX_ADDR)
		i = 5;		
	return MUOK; 
} 

/*free FddfFilePtr*/ 
int Mu_FddfFileFree(FddfFilePtr Ptr) 
{ 
	Mu_Free(Ptr->name); 
	Mu_Free(Ptr->md5); 
	Mu_Free(Ptr->crc); 
	Mu_Free(Ptr->displayname); 
	Mu_Free(Ptr->description); 
	Mu_Free(Ptr->validbeforedate); 
	Mu_Free(Ptr->description); 
	Mu_Free(Ptr->NonDefaultServer); 
	Mu_Free(Ptr->serverfilepath); 
	Mu_Free(Ptr->databaseinfo->program); 
	Mu_Free(Ptr->databaseinfo->group); 
	Mu_Free(Ptr->databaseinfo->owner); 
	Mu_Free(Ptr->databaseinfo); 
	free(Ptr);

	return MUOK; 
}
 
/*create a new FddfFilePtr and initialize it*/
int Mu_FddfFileCreate(FddfFilePtr *Ptr) 
{ 
	DatabaseInfoPtr database = NULL; 
	database = (DatabaseInfoPtr)malloc(sizeof(DatabaseInfo)); 
	if(NULL == database)
	{ 
		Mu_ErrorPrint();
		return MUNBUF; 
	} 

	*Ptr = (FddfFilePtr)malloc(sizeof(FddfFile)); 
	if(NULL == *Ptr)
	{ 
		Mu_ErrorPrint();
		return MUNBUF; 
	} 
	(*Ptr)->databaseinfo = database;
 
	Mu_FddfFileInit(*Ptr);
	return MUOK; 
} 

/*reinitialize*/
int Mu_SegmentHeadClear(SegmentHeadPtr Ptr) 
{ 
	Ptr->total = 0; 
	Mu_Free(Ptr->next); 
	Mu_Free(Ptr->last); 

	return MUOK; 
} 

/*initialize SegmentHeadPtr*/
int Mu_SegmentHeadInit(SegmentHeadPtr Ptr) 
{ 
	Ptr->total = 0; 
	Ptr->next = NULL; 
	Ptr->last = NULL; 

	return MUOK; 
} 

/*free SegmentHeadPtr*/
int Mu_SegmentHeadFree(SegmentHeadPtr Ptr) 
{ 
	SegmentNodePtr Node1 = Ptr->next;
	SegmentNodePtr Node2 = NULL;
	while(Node1 != NULL)
	{
		Node2 = Node1;
		Node1 = Node1->next;
		Mu_Free(Node2->name); 
		Mu_Free(Node2->md5); 
		Mu_Free(Node2->crc); 
		Mu_Free(Node2);
	}

	free(Ptr);
	return MUOK; 
}
 
/*create a new SegmentHeadPtr and initialize it*/
int Mu_SegmentHeadCreate(SegmentHeadPtr *Ptr) 
{ 
	*Ptr = (SegmentHeadPtr)malloc(sizeof(SegmentHead)); 
	if(NULL == *Ptr)
	{ 
		Mu_ErrorPrint();
		return MUNBUF; 
	} 
	//init 
 	Mu_SegmentHeadInit(*Ptr); 

 	return MUOK; 
} 
 
/*reinitialize*/
int Mu_SegmentNodeClear(SegmentNodePtr Ptr) 
{ 
	Ptr->index = 0; 
	Ptr->size = 0; 
	Mu_Free(Ptr->name); 
	Mu_Free(Ptr->md5); 
	Mu_Free(Ptr->crc); 
	Mu_Free(Ptr->next); 

	return MUOK; 
}

/*initialize SegmentNodePtr*/
int Mu_SegmentNodeInit(SegmentNodePtr Ptr) 
{ 
	Ptr->index = 0; 
	Ptr->size = 0; 
	Ptr->name = NULL; 
	Ptr->md5 = NULL; 
	Ptr->crc = NULL; 
	Ptr->next = NULL; 

	return MUOK; 
}
 
/*create a new SegmentNodePtr and initialize it*/
int Mu_SegmentNodeCreate(SegmentNodePtr *Ptr) 
{ 
	*Ptr = (SegmentNodePtr)malloc(sizeof(SegmentNode)); 
	if(NULL == *Ptr)
	{ 
		Mu_ErrorPrint();
		return MUNBUF; 
 	} 
	//init 
 	Mu_SegmentNodeInit(*Ptr); 

 	return MUOK; 
}

/*initialize FddfFilePtr*/
int Mu_FddfFileInit(FddfFilePtr Ptr) 
{ 
	Ptr->name = NULL; 
	Ptr->size = 0; 
	Ptr->md5 = NULL; 
	Ptr->crc = NULL; 
	Ptr->type = 0; 
	Ptr->rate = 0; 
	Ptr->displayname = NULL; 
	Ptr->description = NULL; 
	Ptr->validbeforedate = NULL; 
	Ptr->description = NULL; 
	Ptr->NonDefaultServer = NULL; 
	Ptr->Authorizemethod = 0; 
	Ptr->serverfilepath = NULL; 
	Ptr->filesegmentnum = 0; 
	Ptr->databaseinfo->index = 0; 
	Ptr->databaseinfo->program = NULL; 
	Ptr->databaseinfo->group = NULL; 
	Ptr->databaseinfo->owner = NULL; 

	return MUOK; 
}

/*reinitialize*/ 
int Mu_FddfFileClear(FddfFilePtr Ptr) 
{ 
	Mu_Free(Ptr->name); 
	Ptr->size = 0; 
	Mu_Free(Ptr->md5); 
	Mu_Free(Ptr->crc); 
	Ptr->type = 0; 
	Ptr->rate = 0; 
	Mu_Free(Ptr->displayname); 
	Mu_Free(Ptr->description); 
	Mu_Free(Ptr->validbeforedate); 
	Mu_Free(Ptr->description); 
	Mu_Free(Ptr->NonDefaultServer); 
	Ptr->Authorizemethod = 0; 
	Mu_Free(Ptr->serverfilepath); 
	Ptr->filesegmentnum = 0; 
	Ptr->databaseinfo->index = 0; 
	Mu_Free(Ptr->databaseinfo->program); 
	Mu_Free(Ptr->databaseinfo->group); 
	Mu_Free(Ptr->databaseinfo->owner); 

	return MUOK; 
} 

/*insert SegmentNodePtr into SegmentHeadPtr*/
int Mu_SegmentNodeInsert(SegmentHeadPtr Head, SegmentNodePtr Node) 
{ 
	SegmentNodePtr p = Head->next;
	SegmentNodePtr q = NULL;
	if((NULL == Node) ||(NULL == Head)) 
	{
		Mu_ErrorPrint();
		return MUNNOD; 
	}

	if(NULL == p)
	{
		Head->next = Node;
		Head->last = Node;
		Head->total++;
		return MUOK;
	}

	while(p != NULL)
	{
		q = p;
		p = p->next;
	}
	Node->next = q->next; 
	q->next = Node; 
	Head->total ++; 
	
	return MUOK; 
} 

/*delete SegmentHeadPtr*/
int Mu_SegmentNodeDelete(SegmentHeadPtr Head) 
{
	SegmentNodePtr p = Head->next;
	SegmentNodePtr Node = NULL; 

	if(p == NULL) 
	{
		printf("no node to del\n");
		return MUNNOD; 
	}

	if((p != NULL) && (p->next == NULL))
	{
		Mu_Free(p->name); 
		Mu_Free(p->md5); 
		Mu_Free(p->crc); 
		free(p); 

		return MUOK; 
	}
	
	while(p->next != NULL)
	{
		Node = p;
		p = p->next;
	}	

	Node->next = NULL;

	Mu_Free(p->name); 
	Mu_Free(p->md5); 
	Mu_Free(p->crc); 
	free(p); 

	return MUOK; 
} 

/*copy the relevant from file_info.xml to FddfFilePtr*/
int Mu_FddfFileStrncpy(FddfFilePtr Ptr, const xmlChar *string, const xmlChar *name) 
{
	if((NULL == Ptr)||(NULL == string)||(NULL == name))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	
	}

	Mu_StrncpyStr(Ptr->name,string,name,(const xmlChar*)"File_Name");
	Mu_StrncpyLong(Ptr->size,string,name,(const xmlChar*)"File_Size");
	Mu_StrncpyStr(Ptr->md5,string,name,(const xmlChar*)"File_HMD5");
	Mu_StrncpyStr(Ptr->crc,string,name,(const xmlChar*)"File_CCRC");
	Mu_StrncpyInt(Ptr->type,string,name,(const xmlChar*)"File_Type");
	Mu_StrncpyInt(Ptr->rate,string,name,(const xmlChar*)"File_Rate");
	Mu_StrncpyStr(Ptr->displayname,string,name,(const xmlChar*)"FileDisplayName");
	Mu_StrncpyStr(Ptr->description,string,name,(const xmlChar*)"FileDescription");
	Mu_StrncpyStr(Ptr->validbeforedate,string,name,(const xmlChar*)"ValidBeforeDate");
	Mu_StrncpyStr(Ptr->validafterwatch,string,name,(const xmlChar*)"ValidAfterWatch");
	Mu_StrncpyStr(Ptr->NonDefaultServer,string,name,(const xmlChar*)"NonDefaultServer");
	Mu_StrncpyInt(Ptr->Authorizemethod,string,name,(const xmlChar*)"Authorize_Method");
	Mu_StrncpyStr(Ptr->serverfilepath,string,name,(const xmlChar*)"Server_File_Path");
	Mu_StrncpyInt(Ptr->filesegmentnum,string,name,(const xmlChar*)"File_Segment_Num");
	Mu_StrncpyInt(Ptr->databaseinfo->index,string,name,(const xmlChar*)"ID_IDX");
	Mu_StrncpyStr(Ptr->databaseinfo->program,string,name,(const xmlChar*)"ID_PRG");
	Mu_StrncpyStr(Ptr->databaseinfo->group,string,name,(const xmlChar*)"ID_GRP");
	Mu_StrncpyStr(Ptr->databaseinfo->owner,string,name,(const xmlChar*)"ID_OWN");
	return MUOK; 
} 

/*copy the relevant data from the Segment of FDDF2.xml to SegmentNodePtr*/
int Mu_SegmentNodeStrncpy(SegmentNodePtr Ptr, const xmlChar *string, const xmlChar *name)
{ 
	if((NULL == string)||(NULL == name) || (NULL == Ptr)) 
	{ 
		Mu_ErrorPrint();
		return MUNNOD; 
	} 

	Mu_StrncpyInt(Ptr->index, string, name, (const xmlChar*)"Index");
	Mu_StrncpyInt(Ptr->size, string, name, (const xmlChar*)"Size");
	Mu_StrncpyStr(Ptr->name, string, name, (const xmlChar*)"Name");
	Mu_StrncpyStr(Ptr->md5, string, name, (const xmlChar*)"HMD5");
	Mu_StrncpyStr(Ptr->crc, string, name, (const xmlChar*)"CCRC");
	return MUOK; 
} 

/*initialize EyewearADPPtr*/
int Mu_EyewearADPInit(EyewearADPPtr Ptr) 
{ 
	//init 
	Ptr->prelogin->encryptionmethod = 0; 
	Ptr->prelogin->challenge = NULL; 
	//init device 
	Ptr->device_info->DeviceIDInt = NULL; 
	Ptr->device_info ->DeviceIDApp = NULL; 
	Ptr->device_info ->devicestatus = 0; 
	Ptr->device_info ->trigresource = 0; 
	Ptr->device_info ->SRVDateTime = NULL; 
	Ptr->device_info ->firmwareversion = 0; 
	Ptr->device_info ->hardwareversion = 0; 
	Ptr->device_info ->serverinfostep = 0; 
	//init
	Ptr->conversation->ConversationID = NULL; 
	Ptr->conversation ->AuthUsername = NULL; 
	Ptr->conversation ->AuthPassword = NULL; 
	Ptr->Protocolver = 0; 
	Ptr->ProtocolOPC = 0; 

	return MUOK; 
}
 
/*reinitialize*/
int Mu_EyewearADPClear(EyewearADPPtr Ptr) 
{ 
	//init 
	Ptr->prelogin->encryptionmethod = 0; 
	Mu_Free(Ptr->prelogin->challenge); 
	//init device 
	Mu_Free(Ptr->device_info->DeviceIDInt ); 
	Mu_Free(Ptr->device_info ->DeviceIDApp ); 
	Ptr->device_info ->devicestatus = 0; 
	Ptr->device_info ->trigresource = 0; 
	Mu_Free(Ptr->device_info ->SRVDateTime ); 
	Ptr->device_info ->firmwareversion = 0; 
	Ptr->device_info ->hardwareversion = 0; 
	Ptr->device_info ->serverinfostep = 0; 
	//init
	Mu_Free(Ptr->conversation->ConversationID); 
	Mu_Free(Ptr->conversation ->AuthUsername); 
	Mu_Free(Ptr->conversation ->AuthPassword); 
	Ptr->Protocolver = 0; 
	Ptr->ProtocolOPC = 0; 

	return MUOK; 
}
 
/*free EyewearADPPtr*/
int Mu_EyewearADPFree(EyewearADPPtr Ptr) 
{ 
	Mu_Free(Ptr->prelogin->challenge); 
	Mu_Free(Ptr->device_info->DeviceIDInt); 
	Mu_Free(Ptr->device_info->DeviceIDApp); 
	Mu_Free(Ptr->device_info->SRVDateTime); 
	Mu_Free(Ptr->conversation->ConversationID); 
	Mu_Free(Ptr->conversation->AuthUsername); 
	Mu_Free(Ptr->conversation->AuthPassword); 
	Mu_Free(Ptr->prelogin);
	Mu_Free(Ptr->device_info);
	Mu_Free(Ptr->conversation);
	free(Ptr);
	
	return MUOK; 
}

/*create a new EyewearADPPtr and initialize it*/
int Mu_EyewearADPCreate(EyewearADPPtr *Ptr) 
{ 
	PreLoginPtr Pre_log = NULL; 
	DeviceInfoPtr device = NULL; 
	ConversationPtr conv = NULL; 
	Pre_log = (PreLoginPtr)malloc(sizeof(PreLogin)); 

	if(NULL == Pre_log)
	{ 
		Mu_ErrorPrint();
		return MUNBUF; 
	} 

	device = (DeviceInfoPtr)malloc(sizeof(DeviceInfo)); 

	if(NULL == device)
	{ 
		Mu_ErrorPrint();
 		return MUNBUF; 
	} 

	conv = (ConversationPtr)malloc(sizeof(Conversation)); 

	if(NULL == conv)
	{ 
		Mu_ErrorPrint();
		return MUNBUF; 
	} 

	*Ptr = (EyewearADPPtr)malloc(sizeof(EyewearADP)); 

 	if(NULL == *Ptr)
	{ 
		Mu_ErrorPrint();
		return MUNBUF; 
	} 

	(*Ptr)->prelogin = Pre_log; 
	(*Ptr)->device_info = device; 
	(*Ptr)->conversation = conv; 

	Mu_EyewearADPInit(*Ptr);
	return MUOK; 
} 	
 
/*copy the relevant data from login.xml to EyewearADPPtr*/
int Mu_EyewearADPStrncpy(EyewearADPPtr Ptr, const xmlChar *string, const xmlChar *name) 
{ 
	if((NULL == Ptr)||(NULL == string)||(NULL ==name))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	Mu_StrncpyInt(Ptr->Protocolver, string, name, (const xmlChar*)"Protocol_Ver");
	Mu_StrncpyInt(Ptr->ProtocolOPC, string, name, (const xmlChar*)"Protocol_OPC");
	Mu_StrncpyInt(Ptr->prelogin->encryptionmethod, string, name, (const xmlChar*)"EncryptionMethod");
	Mu_StrncpyStr(Ptr->prelogin->challenge, string, name, (const xmlChar*)"Challenge");
	Mu_StrncpyStr(Ptr->device_info->DeviceIDInt, string, name, (const xmlChar*)"Device_ID_Int");
	Mu_StrncpyStr(Ptr->device_info->DeviceIDApp, string, name, (const xmlChar*)"Device_ID_App");
	Mu_StrncpyInt(Ptr->device_info->devicestatus, string, name, (const xmlChar*)"Device_Status");
	Mu_StrncpyInt(Ptr->device_info->trigresource, string, name, (const xmlChar*)"Trig_Resource");
	Mu_StrncpyStr(Ptr->device_info->SRVDateTime, string, name, (const xmlChar*)"SRV_Date_Time");
	Mu_StrncpyInt(Ptr->device_info->firmwareversion, string, name, (const xmlChar*)"FirmwareVersion");
	Mu_StrncpyInt(Ptr->device_info->hardwareversion, string, name, (const xmlChar*)"HardwareVersion");
	Mu_StrncpyInt(Ptr->device_info->serverinfostep, string, name, (const xmlChar*)"ServerInfo_Step");
	Mu_StrncpyStr(Ptr->conversation->ConversationID, string, name, (const xmlChar*)"ConversationID");
	Mu_StrncpyStr(Ptr->conversation->AuthUsername, string, name, (const xmlChar*)"AuthUsername");
	Mu_StrncpyStr(Ptr->conversation->AuthPassword, string, name, (const xmlChar*)"AuthPassword");
	
	return MUOK;
} 

/*reinitiallize*/
int Mu_UpdateInfoClear(UpdateInfoPtr Ptr)
{
	Ptr->srvsvrinfo->version = 0;
	Mu_Free(Ptr->srvsvrinfo->checksum);	
	Ptr->srvfirmsf->firmwareversion = 0;
	Mu_Free(Ptr->srvfirmsf->firmwarechecksum);
	Ptr->srvfirmsf->hardwareversion = 0;
	
	return MUOK;
}

/*initialize UpdateInfoPtr*/
int Mu_UpdateInfoInit(UpdateInfoPtr Ptr)
{
	Ptr->srvsvrinfo->version = 0;
	Ptr->srvsvrinfo->checksum = NULL;	
	Ptr->srvfirmsf->firmwareversion = 0;
	Ptr->srvfirmsf->firmwarechecksum = NULL;
	Ptr->srvfirmsf->hardwareversion = 0;
	
	return MUOK;
}

/*free UpdateInfoPtr*/
int Mu_UpdateInfoFree(UpdateInfoPtr Ptr)
{
	Mu_Free(Ptr->srvsvrinfo->checksum);	
	Mu_Free(Ptr->srvfirmsf->firmwarechecksum);
	Mu_Free(Ptr->srvsvrinfo);	
	Mu_Free(Ptr->srvfirmsf);
	free(Ptr);
	
	return MUOK;
}

/*create a new UpdateInfoPtr and initialize it*/
int Mu_UpdateInfoCreate(UpdateInfoPtr *Ptr)
{
	SRVSvrInfoPtr info;
	SRVFirmSFPtr firm;
	info = (SRVSvrInfoPtr)malloc(sizeof(SRVSvrInfo));

	if(NULL == info)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	firm = (SRVFirmSFPtr)malloc(sizeof(SRVFirmSF));
	if(NULL == firm)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	*Ptr = (UpdateInfoPtr)malloc(sizeof(UpdateInfo));
	if(NULL == *Ptr)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	
	}

	(*Ptr)->srvsvrinfo = info;
	(*Ptr)->srvfirmsf = firm;	
	Mu_UpdateInfoInit(*Ptr);

	return MUOK;
}

int Mu_UpdateInfoDelete(UpdateInfoPtr Ptr)
{
	if(Ptr==NULL)
    	{	
        	return MUNNOD;
    	}
    	else
    	{
		if(Ptr -> srvsvrinfo != NULL)
        	{
                		Mu_Free(Ptr -> srvsvrinfo -> checksum);
            			free(Ptr -> srvsvrinfo);
		}
		if(Ptr -> srvfirmsf != NULL)
       		{
                	Mu_Free(Ptr -> srvfirmsf -> firmwarechecksum);
            		free(Ptr -> srvfirmsf);
		}
         	free(Ptr);
	}

	return MUOK;
}

/*copy the relevant data from updateSRV.xml to UpdateInfoPtr*/
int Mu_UpdateInfoStrncpy(UpdateInfoPtr Ptr, const xmlChar *string, const xmlChar *name)
{
	if((NULL == Ptr)||(NULL == string)||(NULL == name))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	Mu_StrncpyInt(Ptr->srvsvrinfo->version, string, name, (const xmlChar*)"SvrInfo_version");
	Mu_StrncpyStr(Ptr->srvsvrinfo->checksum, string, name, (const xmlChar*)"SvrInfo_checksum");
	Mu_StrncpyInt(Ptr->srvfirmsf->firmwareversion, string, name, (const xmlChar*)"Fireware_version");
	Mu_StrncpyStr(Ptr->srvfirmsf->firmwarechecksum, string, name, (const xmlChar*)"Fireware_checksum");
	Mu_StrncpyInt(Ptr->srvfirmsf->hardwareversion, string, name, (const xmlChar*)"hardwareversion");

	return MUOK;
}	

/*reinitialize XmlDeviceInfoPtr*/
int Mu_XmlDeviceInfoClear(XmlDeviceInfoPtr Ptr)
{
	Mu_Free(Ptr->IdInt);
	Mu_Free(Ptr->App);
	Ptr->Hardwareversion = 0;
	
	return MUOK;
}

/*initialize XmlDeviceInfoPtr*/
int Mu_XmlDeviceInfoInit(XmlDeviceInfoPtr Ptr)
{
	Ptr->IdInt = NULL;
	Ptr->App = NULL;
	Ptr->Hardwareversion = 0;
	
	return MUOK;
}

/*free XmlDeviceInfoPtr*/
int Mu_XmlDeviceInfoFree(XmlDeviceInfoPtr Ptr)
{
	Mu_Free(Ptr->IdInt);
	Mu_Free(Ptr->App);
	Mu_Free(Ptr);
	
	return MUOK;
}

/*create a new XmlDeviceInfoPtr and initialize it*/
int Mu_XmlDeviceInfoCreate(XmlDeviceInfoPtr *Ptr)
{
	*Ptr = (XmlDeviceInfoPtr)malloc(sizeof(XmlDeviceInfo));
	if(NULL == *Ptr)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	Mu_XmlDeviceInfoInit(*Ptr);

	return MUOK;
}

/*copy the relevant data from deviecinformation.xml to XmlDeviceInfoPtr*/
int Mu_DeviceInfoStrncpy(XmlDeviceInfoPtr Ptr, const xmlChar *string, const xmlChar *name)
{
	if((NULL ==Ptr)||(NULL == string)||(NULL == name))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	Mu_StrncpyStr(Ptr->IdInt, string, name, (const xmlChar*)"Device_ID_Int");
	Mu_StrncpyStr(Ptr->App, string, name, (const xmlChar*)"Device_ID_App");
	Mu_StrncpyInt(Ptr->Hardwareversion, string, name, (const xmlChar*)"Device_Fireversion");
	
	return MUOK;
}

/*initialize ResumeInfoPtr*/
int Mu_ResumeInfoInit(ResumeInfoPtr Ptr, int num) 
{ 
//	Ptr += num; 
	int i;
	if(Ptr == NULL)
	{ 
		Mu_ErrorPrint();
		return MUNNOD; 
	} 
	for(i = 0; i< num; i++)
	{
		Ptr->type = 0;
		memset(Ptr->name, 0, MU_NAME_MAX + 1); 
		Ptr++;
	}
	return MUOK; 
}

/*create ResumeInfoPtr and init it*/
int Mu_ResumeInfoCreate(ResumeInfoPtr * Ptr,int num) 
{ 
	*Ptr = (ResumeInfoPtr)malloc(sizeof(ResumeInfo)*num); 

	if(NULL == *Ptr)
	{ 
			Mu_ErrorPrint();
			return MUNBUF; 
 	}	 
	Mu_ResumeInfoInit(*Ptr,num); 

  	return MUOK; 
}

/*insert and copy data to ResumeInfoPtr,the num at most is 10*/
int Mu_ResumeInsert(ResumeInfoPtr Ptr,int type,xmlChar *name,int num,const xmlChar* string) 
{ 
	int i;
	char str[100];

	for(i = 0;i < 10;i++)
	{ 
		if(!(Ptr+i)->type) 
		break; 
	
	}
 
	if(i == 10)
	{ 
		Mu_ErrorPrint();
		return MUNNOD; 
	} 

//	Mu_ResumeInfoInit(Ptr, i); 
	
	(Ptr + i)->type = type; 
	Convert(type,str);//convert int to string
	
//		memset((Ptr+i)->name, 0, MU_NAME_MAX + 1); 
	if(xmlStrcmp((xmlChar*)str,name))
	{	
		strncpy((Ptr+ i)->name, (char*)name, xmlStrlen(name)+1); 
	}

//	num = i;

	return MUOK; 
} 

/*reinitialize*/
int Mu_FileInfoClear(FileInfoPtr Ptr)
{
	Ptr->datebaseinfo->index = 0;
	Mu_Free(Ptr->datebaseinfo->program);
	Mu_Free(Ptr->datebaseinfo->group);
	Mu_Free(Ptr->datebaseinfo->owner);
	Mu_Free(Ptr->name);
	Ptr->size = 0;
	Mu_Free(Ptr->md5);
	Mu_Free(Ptr->crc);
	Ptr->type = 0;
	Ptr->rate = 0;
	Mu_Free(Ptr->displayname);
	Mu_Free(Ptr->description);
	Mu_Free(Ptr->validbeforedate);
	Mu_Free(Ptr->validafterwatch);
	Ptr->userrate = 0;	
	Ptr->userfreq = 0;	

	return MUOK;
}

/*initialize FileInfoPtr*/
int Mu_FileInfoInit(FileInfoPtr Ptr)
{
	Ptr->datebaseinfo->index = 0;
	Ptr->datebaseinfo->program = NULL;
	Ptr->datebaseinfo->group = NULL;
	Ptr->datebaseinfo->owner = NULL;
	Ptr->name = NULL;
	Ptr->size = 0;
	Ptr->md5 = NULL;
	Ptr->crc = NULL;
	Ptr->type = 0;
	Ptr->rate = 0;
	Ptr->displayname = NULL;
	Ptr->description = NULL;
	Ptr->validbeforedate = NULL;
	Ptr->validafterwatch = NULL;
	Ptr->userrate = 0;	
	Ptr->userfreq = 0;	

	return MUOK;
}

/*free FileInfoPtr*/
int Mu_FileInfoFree(FileInfoPtr Ptr)
{
	Mu_Free(Ptr->datebaseinfo->program);
	Mu_Free(Ptr->datebaseinfo->group);
	Mu_Free(Ptr->datebaseinfo->owner);
	Mu_Free(Ptr->name);
	Mu_Free(Ptr->md5);
	Mu_Free(Ptr->crc);
	Mu_Free(Ptr->displayname);
	Mu_Free(Ptr->description);
	Mu_Free(Ptr->validbeforedate);
	Mu_Free(Ptr->validafterwatch);
	Mu_Free(Ptr->datebaseinfo);
	free(Ptr);

	return MUOK;
}

/*create a new FileInfoPtr and initialize it*/
int Mu_FileInfoCreate(FileInfoPtr *Ptr)
{
	DatabaseInfoPtr info;
	info = (DatabaseInfoPtr)malloc(sizeof(DatabaseInfo));

	if(NULL == info)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	*Ptr = (FileInfoPtr)malloc(sizeof(FileInfo));

	if(NULL == *Ptr)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}

	(*Ptr)->datebaseinfo = info;
	Mu_FileInfoInit(*Ptr);
	
	return MUOK;
}

/*copy the relevant data from fileinfo.xml to FileInfoPtr*/
int Mu_FileInfoStrncpy(FileInfoPtr Ptr, xmlChar *string, const xmlChar *name)
{
	if((NULL == Ptr)||(NULL == string)||(NULL == name))
	{
		Mu_ErrorPrint();
		return MUNNOD;
	}	

	Mu_StrncpyStr(Ptr->name, string, name, (const xmlChar*)"File_Name");
	Mu_StrncpyLong(Ptr->size, string, name, (const xmlChar*)"File_Size");
	Mu_StrncpyStr(Ptr->md5, string, name, (const xmlChar*)"File_HMD5");
	Mu_StrncpyStr(Ptr->crc, string, name, (const xmlChar*)"File_CCRC");	
	Mu_StrncpyInt(Ptr->type, string, name, (const xmlChar*)"File_Type");
	Mu_StrncpyInt(Ptr->rate, string, name, (const xmlChar*)"File_Rate");
	Mu_StrncpyStr(Ptr->displayname, string, name, (const xmlChar*)"FileDisplayName");
	Mu_StrncpyStr(Ptr->description, string, name, (const xmlChar*)"FileDescription");
	Mu_StrncpyStr(Ptr->validbeforedate, string, name, (const xmlChar*)"ValidBeforeDate");
	Mu_StrncpyStr(Ptr->validafterwatch, string, name, (const xmlChar*)"ValidAfterWatch");
	Mu_StrncpyInt(Ptr->datebaseinfo->index, string, name, (const xmlChar*)"ID_IDX");
	Mu_StrncpyStr(Ptr->datebaseinfo->program, string, name, (const xmlChar*)"ID_PRG");
	Mu_StrncpyStr(Ptr->datebaseinfo->group, string, name, (const xmlChar*)"ID_GRP");
	Mu_StrncpyStr(Ptr->datebaseinfo->owner, string, name, (const xmlChar*)"ID_OWN");
	Mu_StrncpyInt(Ptr->userrate, string, name, (const xmlChar*)"User_Rate");
	Mu_StrncpyInt(Ptr->userfreq, string, name, (const xmlChar*)"User_Freq");
	
	return MUOK;
}
