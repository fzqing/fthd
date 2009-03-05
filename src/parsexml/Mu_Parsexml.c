#include "../include/Mu_error.h"
#include "../include/Mu_Struct_main.h"
#include "../include/Mu_xml_parse.h"

volatile int FDDF_2xx_total = 0;
volatile int DIR_4xx_total = 0;

/*get information from xmlXPathObjectPtr and to allot space*/
int Mu_ParseSingle(xmlXPathObjectPtr Ptr, void **back) 
{ 
	xmlNodePtr curNode = NULL; 
	xmlChar *muvalue = NULL; 
	int length; 
	//backvalue can use global value; 
	
	xmlNodeSetPtr munodeset = Ptr->nodesetval; 

	//do MUEERO. Because we parse the xml element is single!!!
	//if the munode is not NULL, we also just process ONE 
	if((munodeset == NULL) ||((munodeset->nodeNr) == 0)
	||(( curNode = munodeset->nodeTab[0]) == NULL) 
	||(( muvalue = xmlNodeGetContent(curNode)) == NULL))
	{ 
		Mu_ErrorPrint();
		return MUEXSIN;
	}
	length = xmlStrlen(muvalue); 

	if((length > MUXML_LENGTH_MAX) && ((length = Mu_TruncateInfo(muvalue,xmlStrlen(muvalue))) < 0))
	{ 
		Mu_ErrorPrint();
		return MUEXSIN; 
	} 
	
	if(((* back) = (xmlChar *)malloc(length + 1)) == NULL)
	{ 
		Mu_ErrorPrint();
		return MUNBUF; 
	} 

	memset(*back, 0, length + 1); 
	strncpy(*back,(char*)muvalue, length); 
	xmlFree(muvalue); 

	return MUOK;
}  

/*detect the length of string utf8*/
int Mu_TruncateInfo(xmlChar *value, int len) 
{ 
	//according to MUXML_LENGTH_MAX,truncate the string 
	//all the characters in string are UTF-8 
	//value: The string you want to cut.
    	//len: The display width of a string you want.
	int i = 0;
	int j = 0;
	char temp[61];
	char * XML_Buff = (char*)malloc(xmlStrlen(value) + 1);
	memset(XML_Buff, 0, (xmlStrlen(value) + 1));
	strncpy(XML_Buff, (char*)value, (xmlStrlen(value)));
	if(len > 60)
		len = 60;
	memset(temp, 0 ,61);
	do{
		//If the character is a Chinese
		if((unsigned char)XML_Buff[i] > 224)
		{
			strncat(temp, &XML_Buff[i], 1);
			strncat(temp, &XML_Buff[i+1], 1);
			strncat(temp, &XML_Buff[i+2], 1);
			i = i + 3;
			j = j + 2;
		}
		//If the character is a symble
		else if((unsigned char)XML_Buff[i] > 192)
		{
			strncat(temp, &XML_Buff[i], 1);
			strncat(temp, &XML_Buff[i+1], 1);
			i = i + 2;
			j = j + 2;
		}
		//If the character is a alpha
		else
		{
					
			strncat(temp, &XML_Buff[i], 1);
			i++;
			j++;
		}
	}while(j < len);

	return j;
} 

/*parse information from xmlXPathObjectPtr and to allot static address*/ 
int Mu_ParseFDDFList(xmlXPathObjectPtr Ptr, FddfListHeadPtr back)
{
	xmlNodePtr curNode = NULL;
	xmlNodePtr tempNode = NULL;
	xmlChar * muvalue = NULL;
	int num;
	int i = 0;
	int count;

	FddfListNodePtr node = NULL;	
	xmlNodeSetPtr munodeset = Ptr->nodesetval;
	if(NULL == munodeset)
	{
		Mu_ErrorPrint();
		return MUEERO;
	}
	//init the header of fddf list
	Mu_FddfListHeadClear(back);
	if(((curNode = munodeset->nodeTab[0]) == NULL)||
	((curNode = curNode->children->next) == NULL))
	{
		Mu_ErrorPrint();
		return MUEERO;
	}
	
	if(((muvalue = xmlNodeGetContent(curNode)) == NULL)
	||(xmlStrlen(muvalue) != 3))
	{
		Mu_ErrorPrint();
		return MUEERO;
	}

	num = atoi((char*)muvalue);	
	if(!(CHECK_FDDF_LIST(num)))
	{
		Mu_ErrorPrint();
		return MUEERO;
	}
	
	//stored the value of 'Type'
	if(Mu_FddfLisitHeadStrncpy(back,muvalue,curNode->name)<0)
	{
		Mu_ErrorPrint();
		return MUEERO;
	}
	Mu_xmlFree(muvalue);
	
	//get 6 elements:text,size,data1,data2,data3,data4
	while(i < 6)
	{
		//if(((curNode = curNode->next->next) == NULL)
		//||((muvalue = xmlNodeGetContent(curNode))==NULL)
		//||((Mu_FddfLisitHeadStrncpy(back,muvalue,curNode->name))<0))
		if(((tempNode = curNode->next->next) == NULL)
		||((muvalue = xmlNodeGetContent(tempNode))==NULL)
		||((Mu_FddfLisitHeadStrncpy(back,muvalue,tempNode->name))<0))
		{
			
				Mu_ErrorPrint();
				Mu_xmlFree(muvalue);
				return MUEERO;
		}
		Mu_xmlFree(muvalue);
           	xmlUnlinkNode(curNode);
		xmlFreeNode(curNode);
		curNode = tempNode;
		i++;
	}
	//get the path of the FDDF,stored in the server
		count=back->size - i + 2;
	while(count--)
	{
		//if(((curNode = curNode->next->next) == NULL)
		//||((muvalue = xmlNodeGetContent(curNode)) == NULL)
		//||(Mu_FddfListNodeCreate(&node) < 0)
		//||(Mu_FddfListNodeStrncpy(node,muvalue,curNode->name) < 0)
		//||(Mu_FddfListNodeInsert(back,node) < 0))
		if(((tempNode = curNode->next->next) == NULL)
		||((muvalue = xmlNodeGetContent(tempNode)) == NULL)
		||(Mu_FddfListNodeCreate(&node) < 0)
		||(Mu_FddfListNodeStrncpy(node,muvalue,tempNode->name) < 0)
		||(Mu_FddfListNodeInsert(back,node) < 0))
		{
			Mu_ErrorPrint();
			Mu_xmlFree(muvalue);
			return MUEERO;
		}
		node = NULL;
           	xmlUnlinkNode(curNode);
		xmlFreeNode(curNode);
		curNode = tempNode;
		Mu_xmlFree(muvalue);
		i++;
	}
	
	return MUOK;		
}

/*parse information from xmlXPathObjectPtr and to allot static address*/       
int Mu_ParseMulti(xmlXPathObjectPtr Ptr,ServerHeadPtr back)
{
	xmlNodePtr curNode = NULL;
	xmlChar * muvalue = NULL;
	int length;
	int i = 0;
	int backvalue = MUEERO;
	ServerNodePtr node = NULL;
	//backvalue can use global value;
	//clear the server header of list
	back->total = 0;
	back->next = back->last = NULL;
	xmlNodeSetPtr munodeset = Ptr->nodesetval;
	if(munodeset == NULL)
	{
		Mu_ErrorPrint();
		return  backvalue;
	}

	Mu_ServerHeadClear(back);
	//maybe omit
	for(i = 0;i< munodeset->nodeNr;i++)
	{
		curNode = munodeset->nodeTab[i];
		if(curNode != NULL)
			muvalue = xmlNodeGetContent(curNode->children->next);
		if(muvalue == NULL)
		{			
			Mu_ErrorPrint();
			continue;
		}
		length = xmlStrlen(muvalue);
		if(length > MUXML_LENGTH_MAX)
		{
			break;
		}
		//creat node
		Mu_ServerNodeCreate(&node);

		//copy length to buffer
		Mu_ServerNodeStrncpy(node,muvalue,curNode->name);
		xmlFree(muvalue);
		Mu_ServerNodeInsert(back,node);
	}
	return backvalue;	
}

/*parse information from xmlXPathObjectPtr and to allot static address*/       
/*the xpath is the father path of type 2xx*/
int Mu_ParseMulti2XX(xmlXPathObjectPtr Ptr, FddfStatusHeadPtr back) 
{ 
	xmlNodePtr curNode = NULL; 
//	xmlNodePtr delNode = NULL; 
	xmlChar *muvalue = NULL; 
	int num; 
	int total_parse = 0; 
	/*volatile*/ int insertmark; 
	FddfStatusNodePtr node = NULL; 
	//backvalue can use global value; 
	xmlNodeSetPtr munodeset = Ptr->nodesetval; 
	if(munodeset == NULL)
	{ 
		Mu_ErrorPrint();
		return MUEX2XX; 
	} 
	Mu_FddfStatusHeadClear(back);
	for(insertmark = 1; ((total_parse < MUMULTI_LIST_MAX) 
 	   && (FDDF_2xx_total < munodeset->nodeNr)); 
 	   FDDF_2xx_total ++, total_parse ++/*, insertmark = 1*/) 
	{ 
		if((curNode = munodeset->nodeTab[FDDF_2xx_total]) == NULL)
		{ 
			Mu_ErrorPrint();
			continue; 
		} 
		//jump to the first child 
		if(((curNode = curNode->children->next) == NULL) 
 		  ||((muvalue = xmlNodeGetContent(curNode)) == NULL) 
 		  ||(xmlStrlen(muvalue) != 3))//judge the type num,the length should be 3
		{ 
			Mu_ErrorPrint();
 			return MUEX2XX; 
		} 
		if(!(num = atoi((char*)muvalue)) || (!(CHECK_2XX(num))))
		{
			Mu_xmlFree(muvalue);
			continue;	 
		}
		if((Mu_FddfStatusNodeCreate(&node) < 0) 
 		  ||(Mu_FddfStatusNodeStrncpy(node, muvalue, curNode->name) < 0))
		{ 
			Mu_ErrorPrint();
 			Mu_xmlFree(muvalue); 
 			return MUEX2XX; 
		} 
		Mu_xmlFree(muvalue); 
		//get rid of one Item.Type = 2xx 
		while(curNode->next->next != NULL)
		{ 
			curNode = curNode->next->next; 
			if(NULL == (muvalue = xmlNodeGetContent(curNode)))
			{ 
 				Mu_xmlFree(muvalue); 
				Mu_ErrorPrint();
				return MUEX2XX; 
			} 
			if(xmlStrlen(muvalue) > MUXML_LENGTH_MAX)
			{		 
				Mu_FddfStatusNodeDelete(back,node); 
				insertmark = 0; 
				Mu_xmlFree(muvalue); 
				break; 
			}	 
			if(Mu_FddfStatusNodeStrncpy(node, muvalue, curNode->name)< 0)
			{ 
				Mu_FddfStatusNodeDelete(back,node); 
				Mu_xmlFree(muvalue); 
				Mu_ErrorPrint();
				return MUEX2XX; 
			}		 
			Mu_xmlFree(muvalue); 
			
		}//end of while 
		if((insertmark) && (Mu_FddfStatusNodeInsert(back, node) < 0)) 
		{
			printf("ending\n");
			return MUEX2XX; 
		}
	}
	//clear the counter for FDDF_2xx_total 
	if(FDDF_2xx_total == munodeset->nodeNr) 
		FDDF_2xx_total = 0; 
	return MUOK; 
} 

/*parse information from xmlXPathObjectPtr and to allot static address*/       
int Mu_ParseMulti4XX(xmlXPathObjectPtr Ptr, DirStatusHeadPtr back) 
{ 
	int length; 
	int total_parse; 
	int mark = 0;  
	int num;
	//backvalue can use global value
	xmlNodePtr curNode = NULL; 
	xmlNodePtr delNode = NULL; 
	xmlChar *muvalue = NULL; 
	DirStatusNodePtr node = NULL; 
	DirDownloadAddrPtr addrnode = NULL; 
	xmlNodeSetPtr munodeset = Ptr->nodesetval; 
	if(munodeset == NULL)
	{ 
		Mu_ErrorPrint();
		return MUEX4XX; 
	} 
	Mu_DirStatusHeadClear(back);
	for(total_parse = 0; 
 		(total_parse < MUMULTI_LIST_MAX) 
 		&& (DIR_4xx_total < munodeset->nodeNr); 
 		DIR_4xx_total ++, total_parse ++)
	{ 
		if((curNode = munodeset->nodeTab[DIR_4xx_total]) == NULL)
		{ 
			Mu_ErrorPrint();
			continue; 
		} 
		delNode = curNode; 
		//jump to the first child 
		if(((curNode = curNode->children->next) == NULL) 
 		||((muvalue = xmlNodeGetContent(curNode)) == NULL) 
  		||(xmlStrlen(muvalue) != 3) 
   		||(!( num = atoi((char*)muvalue))) 
    		||(!(CHECK_4XX(num))))
		{ 
    			fprintf(stdout, "the Type is MUEERO, jump to next!\n");
    			Mu_xmlFree(muvalue); 
    			continue; 
    		} 
    		if(Mu_DirStatusNodeCreate(&node) < 0 
     		|| Mu_DirStatusNodeStrncpy(node, muvalue, curNode->name) < 0)
		{ 
     			fprintf(stdout, "copy OR create node MUEERO!\n");
     			Mu_xmlFree(muvalue); 
     			Mu_DirStatusNodeDelete(back,node); 
     			return MUEX4XX; 
     		} 
     		Mu_xmlFree(muvalue); 
		//get ride of one Item.Type = 4xx 
		curNode = curNode->next->next; 
		curNode = curNode->next->next; 
		//skip two novalid segment 
		//jump "size" and "text" 
			mark = 0;	
		while(curNode->next->next != NULL)
		{ 
			curNode = curNode->next->next; 
			if(mark < 4)
			{ 
				if(((muvalue = xmlNodeGetContent(curNode)) == NULL) 
 				|| ((xmlStrlen(muvalue)) > MUXML_LENGTH_MAX))
				{ 
 					Mu_xmlFree(muvalue); 
					Mu_DirStatusNodeDelete(back,node); 
					//Other processes!!!According to need!!! 
					break; 
 				} 
				if(Mu_DirStatusNodeStrncpy(node, muvalue, curNode-> name) < 0)
				{ 
					Mu_xmlFree(muvalue); 
					Mu_DirStatusNodeDelete(back,node); 
					//Other processes!!!According to need!!! 
					return MUEX4XX; 
 				} 
 				Mu_xmlFree(muvalue); 
				mark ++; 
				continue; 
			} 
			if(((muvalue = xmlNodeGetContent(curNode)) == NULL) 
 		/*	||( Mu_DirStatusNodeInsert (back, node) < 0)*/
 			||((length = xmlStrlen(muvalue)) > MUXML_LENGTH_MAX))
			{ 
 				Mu_xmlFree(muvalue); 
				Mu_DirDownloadAddrDeleteList(node); 
				//can't free the header, just free node; 
				//Other processes!!!According to need!!! 
				break; 
			} 
			if(( Mu_DirStatusAddrCreate(&addrnode) < 0)
 			||( Mu_DirDownloadAddrStrncpy(addrnode, muvalue) < 0) 
 			||( Mu_DirDownloadAddrInsert(node, addrnode) < 0))
			{ 
 				Mu_xmlFree(muvalue); 
				Mu_DirDownloadAddrDeleteList(node); 
				return MUEX4XX; 
			} 
			
			Mu_xmlFree(muvalue); 
//			node = NULL; 
//			addrnode = NULL; 
		} 
		Mu_DirStatusNodeInsert (back, node);
	} 
	//clear the counter for DIR_4xx_total 
	if(DIR_4xx_total == munodeset->nodeNr) 
 		DIR_4xx_total = 0; 

	return MUOK; 
} 

/*parse segment content of FDDF_2.xml*/
int Mu_ParseFDDFsegment(xmlXPathObjectPtr Ptr,SegmentHeadPtr back)
{
	int length;
	int i = 0;
	int mark = 0;
	int backvalue = MUOK;

	xmlNodePtr curNode = NULL;
	xmlChar * muvalue = NULL;
	SegmentNodePtr Node = NULL;

	xmlNodeSetPtr munodeset = Ptr->nodesetval; 
	if(NULL == munodeset)
	{
		Mu_ErrorPrint();
		return MUNNOD;
	} 
	Mu_SegmentHeadClear(back);
	for(i=0;i < munodeset->nodeNr;i++)
	{
		curNode = munodeset->nodeTab[i];
		if(NULL == curNode)
		{
			Mu_ErrorPrint();
			break;
		}
		curNode = curNode->children->next;
		if(NULL == curNode)
		{
			Mu_ErrorPrint();
			break;
		}
		Mu_SegmentNodeCreate(&Node);
		while(NULL != curNode)
		{
			muvalue = xmlNodeGetContent(curNode);
			if(muvalue == NULL)	
			{
				Mu_ErrorPrint();
				mark = 1;
				break;
			}
			if((length = xmlStrlen(muvalue)) > MUXML_LENGTH_MAX)
			{
				Mu_ErrorPrint();
				mark = 1;
				break;
			}
			Mu_SegmentNodeStrncpy(Node,muvalue,curNode->name);
			Mu_xmlFree(muvalue);
			curNode = curNode->next->next;
		}
		if(mark)
		{
			Mu_SegmentNodeDelete(back);
		}
		else 
		{
			Mu_SegmentNodeInsert(back,Node);
		}
	}
		
	return backvalue;
}

/*parse information from xmlXPathObjectPtr and to allot static address*/       
int Mu_ParseMultiFDDF(xmlXPathObjectPtr Ptr, FddfStatusHeadPtr back) 
{ 
	int length; 
	int i = 0; 
	int mark = 0; 
	int backvalue = MUOK; 
	//backvalue can use global value; 
	xmlNodePtr curNode = NULL; 
	xmlChar *muvalue = NULL; 
	FddfStatusNode *node = NULL; 
	xmlNodeSetPtr munodeset = Ptr->nodesetval; 
	if(munodeset == NULL)
	{ 
		Mu_ErrorPrint();
		return MUNNOD; 
	} 
	Mu_FddfStatusHeadClear(back);
	for(i=0; i < munodeset->nodeNr; i++)
	{ 
		curNode = munodeset->nodeTab[i]; 
		if(curNode == NULL)
		{ 
			Mu_ErrorPrint();
			break; 
		} 
		//jump to child segment 
		curNode = curNode->children->next; 
		if(curNode == NULL)
		{ 
			Mu_ErrorPrint();
			break; 
		}
	 
		Mu_FddfStatusNodeCreate (&node); 
		while(curNode != NULL)
		{ 
			muvalue = xmlNodeGetContent(curNode); 
			if(muvalue == NULL)
			{ 
				Mu_ErrorPrint();
				mark = 1; 
				break; 
			} 
			if((length = xmlStrlen(muvalue)) > MUXML_LENGTH_MAX)
			{ 
				Mu_ErrorPrint();
				mark = 1; 
				break; 
			} 
			Mu_FddfStatusNodeStrncpy (node, muvalue, curNode->name); 
			Mu_xmlFree(muvalue); 
			curNode = curNode->next->next;
	 	}
		if(mark) 
		{
			Mu_FddfStatusNodeDelete(back,node); 
		}
		else 
		{
			Mu_FddfStatusNodeInsert (back, node);
		}
		 
	}
	return backvalue; 
} 

/*manner to choose way to parse xml*/
int Mu_ParseXpathXML(int type, xmlDocPtr Doc, xmlChar *xpath, void ** back) 
{ 
	xmlXPathObjectPtr xpath_result = NULL; 
	int backvalue = MUOK; 
	xmlXPathContextPtr context;
	context = xmlXPathNewContext(Doc);
	xpath_result = xmlXPathEvalExpression(xpath, context); 
	if(xpath_result == NULL)
	{ 
		Mu_ErrorPrint();
		return MUEERO;
	} 
	switch(type)
	{ 
		case PARSE_SEGMENT:
			backvalue =Mu_ParseFDDFsegment(xpath_result,*back);
			break;
		case PARSE_SINGLE: 
			backvalue = Mu_ParseSingle(xpath_result, back); 
			break; 
		case PARSE_FDDF_LIST:
			backvalue = Mu_ParseFDDFList(xpath_result, *back); 
			break; 
		case PARSE_MULTI: 
			backvalue = Mu_ParseMulti(xpath_result, *back); 
			break; 
		case PARSE_MULTI_1XX:
			backvalue = Mu_ParseMulti1XX(xpath_result,*back);
			break;
		case PARSE_MULTI_2XX: 
			backvalue = Mu_ParseMulti2XX(xpath_result, *back); 
			break; 
		case PARSE_MULTI_4XX: 
			backvalue = Mu_ParseMulti4XX(xpath_result, *back); 
			break; 
		case PARSE_MULTI_FDDF: 
			backvalue = Mu_ParseMultiFDDF(xpath_result, *back);
			break; 
		default: 
			fprintf(stdout, "MUEERO format!\n");
			break; 
	} 
	xmlXPathFreeContext(context);
	xmlXPathFreeObject(xpath_result);
	return backvalue; 
} 

/*manner to choose way to parse xml*/
int Mu_ParseXML(int type, xmlDocPtr doc, void ** back) 
{ 
 	int backvalue = MUOK; 
 	if(NULL == doc)
	{ 
 		Mu_ErrorPrint();
		return backvalue; 
	} 

	switch(type)
	{ 
		case PARSE_EYEWEAR_ADP: 
 			backvalue = Mu_ParseEyewearADP(doc, *back); 
 			break; 
 		case PARSE_EYEWEAR_FDDF: 
 			backvalue = Mu_ParseEyewearFDDF(doc, *back); 
 			break; 
 		case PARSE_SERVICE_INFO: 
 			backvalue = Mu_ParseServiceInfo(doc, *back); 
 			break; 
 		case PARSE_UPDATE_INFO: 
 			backvalue = Mu_ParseUpdateInfo(doc, *back); 
 			break; 
 		case PARSE_RESUME_INFO: 
 			backvalue = Mu_ParseResumeInfo(doc, *back); 
 			break; 
 		case PARSE_FILE_INFO: 
 			backvalue = Mu_ParseFileInfo(doc, *back); 
 			break; 
 		case PARSE_DEVICE_INFO: 
 			backvalue = Mu_ParseDeviceInfo (doc, *back); 
 			break; 
		case PARSE_SRV_SVRINF:
			backvalue = Mu_ParsePayload(doc, *back);
			break;
		default: 
			fprintf(stdout, "MUEERO format!\n");
			break; 
	}
 
	return backvalue; 
} 

/*provide the interface to xml*/
int Mu_Parse (int type, const int fd , xmlChar *xpath, void ** back)
{ 
 	int backvalue = MUOK; 
	xmlDocPtr doc = NULL; 
	lseek(fd, 0, SEEK_SET); //location to the start
	
	doc = xmlReadFd(fd, NULL, "UTF-8", XML_PARSE_RECOVER); 
//	doc = xmlReadFd(fd, NULL, "UTF-8", 0); 

	if(doc == NULL)
	{ 
		Mu_ErrorPrint();
		return MUEERO;
	} 
	if(CHECK_XPATH(type)) 
 		backvalue = Mu_ParseXML(type, doc, back); 
 	else 
 		backvalue = Mu_ParseXpathXML(type, doc, xpath, back); 

 	xmlFreeDoc(doc); 

	return backvalue; 
} 

int Mu_EyewearADPTravel2(xmlDocPtr doc,xmlNodePtr Node,EyewearADPPtr back)
{
 	xmlChar *muvalue = NULL; 
	xmlNodePtr tempNode = NULL;

	while(Node)
	{
 		if(Node->children) 
 			Mu_EyewearADPTravel2(doc,Node->children, back); 
 		if(NULL == ( muvalue = xmlNodeListGetString(doc, Node->children, 1)))
		{ 
 			Node = Node->next; 
 			continue; 
 		}
		else
		{
 			Mu_EyewearADPStrncpy(back ,muvalue, Node->name); 
		}
 		Mu_xmlFree(muvalue); 
           	tempNode = Node->next;
           	xmlUnlinkNode(Node);
           	xmlFreeNode(Node);
           	Node = tempNode;
 	//	Node = Node->next; 
 	} 
	return MUOK;	
}

/*parse the data and copy except payload segment*/
void Mu_EyewearADPTravel(xmlDocPtr doc, xmlNodePtr Node, EyewearADPPtr back) 
{ 
 	while(Node)
	{
		if((xmlStrcmp(Node->name,(xmlChar *)"payload")))
		{
			Mu_EyewearADPTravel2(doc,Node,back);
		}
 		Node = Node->next; 
 	} 

	return; 
}
 
/*parse afterquery.xml*/
int Mu_ParseEyewearADP(xmlDocPtr doc, EyewearADPPtr back) 
{ 
	xmlNodePtr curNode = NULL; 
	if(NULL == (curNode = xmlDocGetRootElement(doc)) 
 	||(xmlStrcmp(curNode->name, (xmlChar *)"Eyewear_ADP")))
	{ 
		Mu_ErrorPrint();
 		return MUEXADP;
 	} 
	//init
 	Mu_EyewearADPClear(back);
 	curNode = curNode->children; 
 	Mu_EyewearADPTravel(doc, curNode, back); 
 	return MUOK; 
} 

/*parse serviceinfo.xml*/
void Mu_ServiceInfoTravel(xmlDocPtr doc,xmlNodePtr Node, ServiceInfoPtr back) 
{ 
 	xmlChar *muvalue = NULL; 
 	ServerNodePtr servernode = NULL; 
 	while(Node)
	{ 
 		if(Node->children) 
	 		Mu_ServiceInfoTravel(doc,Node->children, back); 
 		if(NULL == ( muvalue = xmlNodeListGetString(doc, Node->children, 1)))
		{ 
 			Node = Node->next; 
 			continue; 
 		}
 
		else if(xmlStrcmp(Node->name,(xmlChar*) "Addr")) 	
		{
 			Mu_ServiceInfoStrncpy(back ,muvalue, Node->name); 
		}
 		else
		{ 
 			Mu_ServerNodeCreate(&servernode); 
			Mu_ServerNodeStrncpy(servernode, muvalue, Node->name); 
			Mu_ServerNodeInsert(back->server, servernode); 
		} 
		Mu_xmlFree(muvalue); 
 		Node = Node->next; 
 	} 

	back->server->cur = back->server->next;//pointe to the first node
	return; 
}
 
/*parse serviceinfo.xml*/
int Mu_ParseServiceInfo(xmlDocPtr doc, ServiceInfoPtr back) 
{ 
	xmlNodePtr curNode = NULL; 
	int backvalue = MUOK; 
 	if((NULL == (curNode = xmlDocGetRootElement(doc))) 
 	||(xmlStrcmp(curNode->name, (xmlChar *)"ServiceInfo")))
	{ 
		Mu_ErrorPrint();
 		return MUEXSIN; 
	} 
	curNode = curNode->children; 
	Mu_ServiceInfoClear(back);
 	Mu_ServiceInfoTravel(doc,curNode, back); 

 	return backvalue; 
} 

/*parse FDDF.xml*/
int Mu_EyewearFDDFTravle1(xmlDocPtr Doc,xmlNodePtr Node,FddfFilePtr back)
{
	xmlChar *muvalue = NULL;
 	while(Node)
	{
 		if(Node->children) 
	 		Mu_EyewearFDDFTravle1(Doc,Node->children, back); 
 			if(NULL == ( muvalue = xmlNodeListGetString(Doc, Node->children, 1)))
			{	 
 				Node = Node->next; 
 				continue; 
 			}
			else	
			{
 				Mu_FddfFileStrncpy(back ,muvalue, Node->name); 
			}
		
		Mu_xmlFree(muvalue); 
 		Node = Node->next; 
 	} 
		
	return MUOK;
}

/*parse FDDF.xml*/
int Mu_EyewearFDDFTravle(xmlDocPtr Doc,xmlNodePtr Node,FddfFilePtr back)
{
 	while(Node)
	{
		if(!(xmlStrcmp(Node->name, (xmlChar *)"File_Infomation")))
		{
			Node = Node->children;
			Mu_EyewearFDDFTravle1(Doc,Node,back);
		}
 		Node = Node->next; 
 	} 
	return MUOK;
}

/*parse FDDF.xml*/
int Mu_ParseEyewearFDDF(xmlDocPtr Doc,FddfFilePtr back)
{
	xmlNodePtr curNode = NULL;
	int backvalue = MUOK;
	if((NULL == (curNode = xmlDocGetRootElement(Doc)))
	||(xmlStrcmp(curNode->name,(xmlChar *)"Eyewear_FDDF")))
	{
		Mu_ErrorPrint();
		return MUEXSIN;
	}
	curNode = curNode->children;
	Mu_FddfFileClear(back);
	Mu_EyewearFDDFTravle(Doc,curNode,back);

	return backvalue;
}

/*parse update.xml*/
int Mu_UpdateInfoTravel(xmlDocPtr Doc,xmlNodePtr Node,UpdateInfoPtr Ptr)
{
	xmlChar *muvalue = NULL;
 	while(Node)
	{
 		if(Node->children) 
	 		Mu_UpdateInfoTravel(Doc,Node->children, Ptr); 
 		if(NULL == (muvalue = xmlNodeListGetString(Doc, Node->children, 1)))
		{ 
 			Node = Node->next; 
 			continue; 
 		}
		else	
 			Mu_UpdateInfoStrncpy(Ptr ,muvalue, Node->name); 
		Mu_xmlFree(muvalue); 
 		Node = Node->next; 
 	} 
	return MUOK;
}

/*parse update.xml*/
int Mu_ParseUpdateInfo(xmlDocPtr Doc,UpdateInfoPtr back)
{
	xmlNodePtr curNode = NULL;
	int backvalue = MUOK;
	if((NULL == (curNode = xmlDocGetRootElement(Doc)))
	||(xmlStrcmp(curNode->name,(xmlChar *)"Update_Information")))
	{
		Mu_ErrorPrint();
		return MUEXSIN;
	}
	curNode = curNode->children;
	Mu_UpdateInfoClear(back);
	Mu_UpdateInfoTravel(Doc,curNode,back);

	return backvalue;
}

/*parse deviceinformation.xml*/
int Mu_DeviceInfoTravel(xmlDocPtr Doc,xmlNodePtr Node,XmlDeviceInfoPtr Ptr)
{
	xmlChar *muvalue = NULL;
	while(Node)
	{
 		if(Node->children) 
	 		Mu_DeviceInfoTravel(Doc,Node->children, Ptr); 
 		if(NULL == (muvalue = xmlNodeListGetString(Doc, Node->children, 1)))
		{ 
 			Node = Node->next; 
 			continue; 
 		}
		else
		{	
 			Mu_DeviceInfoStrncpy(Ptr ,muvalue, Node->name); 
		}
		Mu_xmlFree(muvalue); 
 		Node = Node->next; 
 	} 
	return MUOK;
}

/*parse deviceinformation.xml*/
int Mu_ParseDeviceInfo(xmlDocPtr Doc,XmlDeviceInfoPtr back)
{
	xmlNodePtr curNode = NULL;
	int backvalue = MUOK;
	if((NULL == (curNode = xmlDocGetRootElement(Doc)))
	||(xmlStrcmp(curNode->name,(xmlChar *)"DeviceInfo")))
	{	
		Mu_ErrorPrint();
		return MUEXSIN;
	}
	curNode = curNode->children;
	Mu_XmlDeviceInfoClear(back);
	Mu_DeviceInfoTravel(Doc,curNode,back);
	return backvalue;
}

/*parse resume.xml*/
int Mu_ResumeInfoTravel(xmlDocPtr Doc,xmlNodePtr Node,ResumeInfoPtr Ptr)
{
	int len;
	int type =0;
	int num =1;
	xmlChar *muvalue = NULL;
	while(Node)
	{
 		if(Node->children) 
	 		Mu_ResumeInfoTravel(Doc,Node->children, Ptr); 
  		if(NULL == (muvalue = xmlNodeListGetString(Doc, Node->children, 1)))
		{ 	
 			Node = Node->next; 
 			continue; 
 		}
		else
 		{	
			if(!xmlStrcmp(Node->name,(const xmlChar*)"type"))
			{
				len=xmlStrlen(muvalue);
				type=atoi((char*)muvalue);
			}
			if(!xmlStrcmp(Node->name,(const xmlChar*)"name"))
			{	
				Mu_ResumeInsert(Ptr,type,muvalue,num,Node->name);
			}
		}
			Mu_xmlFree(muvalue); 
 			Node = Node->next; 
 		 
	}
	return MUOK;
}	

/*parse resume.xml*/
int Mu_ParseResumeInfo(xmlDocPtr Doc,ResumeInfoPtr back)
{
	xmlNodePtr curNode = NULL;
	int backvalue = MUOK;
	if((NULL == (curNode = xmlDocGetRootElement(Doc)))
	||(xmlStrcmp(curNode->name,(xmlChar *)"resuming")))
	{
		Mu_ErrorPrint();
		return MUEXSIN;
	}
	curNode = curNode->children;
//	Mu_ResumeInfoInit(back,1); 
	Mu_ResumeInfoTravel(Doc,curNode,back);
	return backvalue;
}

/*parse fileinfo.xml*/
int Mu_FileInfoTravel(xmlDocPtr Doc,xmlNodePtr Node,FileInfoPtr Ptr)
{
	xmlChar *muvalue = NULL;
	while(Node)
	{
 		if(Node->children) 
		{	
	 			Mu_FileInfoTravel(Doc, Node->children, Ptr); 
		}
 		if(NULL == (muvalue = xmlNodeListGetString(Doc, Node->children, 1)))
		{ 
 				Node = Node->next; 
 				continue; 
 		}
		else
		{
 			Mu_FileInfoStrncpy(Ptr ,muvalue, Node->name); 
		}		
		Mu_xmlFree(muvalue); 
 		Node = Node->next; 
 	} 
	return MUOK;	
}

/*parse fileinfo.xml*/
int Mu_ParseFileInfo(xmlDocPtr Doc,FileInfoPtr back)
{
	xmlNodePtr curNode = NULL;
	int backvalue = MUOK;
	if((NULL == (curNode = xmlDocGetRootElement(Doc)))
	||(xmlStrcmp(curNode->name,(const xmlChar *)"File_Infomation")))
	{
		Mu_ErrorPrint();
		return MUEXSIN;
	}
	curNode = curNode->xmlChildrenNode;
	Mu_FileInfoClear(back);
	Mu_FileInfoTravel(Doc,curNode,back);
	
	return backvalue;
}

int Mu_PayloadTrave2(xmlDocPtr Doc,xmlNodePtr Node,SRV_SvrInfPtr Ptr)
{
	xmlChar * muvalue = NULL;
//	xmlNodePtr tempNode = NULL;
	while(Node)
	{
 		if(NULL == (muvalue = xmlNodeListGetString(Doc, Node->children, 1)))
		{ 
 				Node = Node->next; 
 				continue; 
		}
		else
			Mu_SRV_SvrInfStrncpy(Ptr, muvalue, Node->name);
		Mu_xmlFree(muvalue); 
		Node = Node->next;
  //         	tempNode = Node->next;
    //       	xmlUnlinkNode(Node);
      //     	xmlFreeNode(Node);
        //   	Node = tempNode;
	}
	return MUOK;
}

int Mu_PayloadTravel(xmlDocPtr Doc,xmlNodePtr Node,SRV_SvrInfPtr Ptr)
{
//	xmlNodePtr tempNode = NULL;
	while(Node)
	{
 		if(Node->children) 
		{	
	 			Mu_PayloadTravel(Doc, Node->children, Ptr); 
		}
		if(!xmlStrcmp(Node->name,(const xmlChar*)"Item"))
		{
			Node = Node->children;
			Mu_PayloadTrave2(Doc,Node,Ptr);
		}
		Node = Node->next;
  //         	tempNode = Node->next;
    //       	xmlUnlinkNode(Node);
      //     	xmlFreeNode(Node);
        //   	Node = tempNode;
	}

	return MUOK;
}

/*parse the payload segment*/
int Mu_ParsePayload(xmlDocPtr Doc,SRV_SvrInfPtr srv)
{
	xmlNodePtr curNode = NULL;
	int backvalue = MUOK;
	if((NULL == (curNode = xmlDocGetRootElement(Doc)))
	||(xmlStrcmp(curNode->name,(const xmlChar *)"Eyewear_ADP")))
	{
		Mu_ErrorPrint();
		return MUEXSIN;
	}
	curNode = curNode->xmlChildrenNode;
	Mu_SRV_SvrInfClear(srv);
	Mu_PayloadTravel(Doc,curNode,srv);

	return backvalue;
}	
/*parse information from xmlXPathObjectPtr and to allot static address*/       
/*the xpath is the father path of type 2xx*/
int Mu_ParseMulti1XX(xmlXPathObjectPtr Ptr, SRV_SvrInfPtr back) 
{ 
	xmlNodePtr curNode = NULL; 
	xmlChar *muvalue = NULL; 
	static int num = 0;
	if(num > 1)
	{
		fprintf(stderr,"the num of list is outof range\n");
		return MUEERO;
	}

	xmlNodeSetPtr munodeset = Ptr->nodesetval; 
	if(munodeset == NULL)
	{ 
		Mu_ErrorPrint();
		return MUEERO; 
	} 
	Mu_SRV_SvrInfClear(back);

	if((curNode = munodeset->nodeTab[num]) == NULL)
	{ 
		Mu_ErrorPrint();
		return MUEERO;
	} 

	if(((curNode = curNode->children->next) == NULL) 
 	||((muvalue = xmlNodeGetContent(curNode)) == NULL) 
 	||(xmlStrlen(muvalue) != 3)//judge the type num,the length should be 3
	||((Mu_SRV_SvrInfStrncpy(back, muvalue, curNode->name)) < 0))
	{ 
		Mu_ErrorPrint();
 		return MUEERO; 
	} 
	Mu_Free(muvalue);
	while(curNode->next->next != NULL)
	{ 
		curNode = curNode->next->next; 
		if(NULL == (muvalue = xmlNodeGetContent(curNode)))
		{ 
 			Mu_xmlFree(muvalue); 
			Mu_ErrorPrint();
			return MUEERO; 
		} 
		if(Mu_SRV_SvrInfStrncpy(back, muvalue, curNode->name)< 0)
		{ 
			Mu_xmlFree(muvalue); 
			Mu_ErrorPrint();
			return MUEERO; 
		}		 
			Mu_xmlFree(muvalue); 
			
	}//end of while 
	num++;

	return MUOK; 
} 
