#ifndef __MU_XML_PARSE_H__
#define __MU_XML_PARSE_H__

#define MUXML_LENGTH_MAX 250

#define MUMULTI_LIST_MAX 20

#define PARSE_SINGLE 1
#define PARSE_FDDF_LIST 2
#define PARSE_MULTI 3
#define PARSE_MULTI_2XX 4
#define PARSE_MULTI_4XX 5
#define PARSE_MULTI_FDDF 6
#define PARSE_SEGMENT    7

#define PARSE_EYEWEAR_ADP 101
#define PARSE_EYEWEAR_FDDF 102
#define PARSE_SERVICE_INFO 103
#define PARSE_UPDATE_INFO 104
#define PARSE_DEVICE_INFO 105
#define PARSE_RESUME_INFO 106
#define PARSE_FILE_INFO   107

#define CHECK_2XX(num) (((num) > 200) && ((num) < 205))
#define CHECK_4XX(num) (((num) > 400) && ((num) < 405))
#define CHECK_XPATH(num) ((num) > 100)

#define Mu_xmlFree(name)\
do{\
	if(name){\
	xmlFree(name);\
	name = NULL;\
	}\
}while(0)



/*get information from xmlXPathObjectPtr and to allot space*/
int Mu_ParseSingle(xmlXPathObjectPtr Ptr, void **back);
/*detect the length of string utf8*/
int Mu_TruncateInfo(xmlChar *value);
/*parse information from xmlXPathObjectPtr and to allot static address*/
int Mu_ParseFDDFList(xmlXPathObjectPtr Ptr, FddfListHeadPtr back);
/*parse information from xmlXPathObjectPtr and to allot static address*/
int Mu_ParseMulti(xmlXPathObjectPtr Ptr,ServerHeadPtr back);
/*parse information from xmlXPathObjectPtr and to allot static address*/
int Mu_ParseMulti2xx(xmlXPathObjectPtr Ptr, FddfStatusHeadPtr back);
/*parse information from xmlXPathObjectPtr and to allot static address*/
int Mu_ParseMulti4XX(xmlXPathObjectPtr Ptr, DirStatusHeadPtr back);
/*parse segment content of FDDF_2.xml*/
int Mu_ParseFDDFsegment(xmlXPathObjectPtr Ptr,SegmentHeadPtr back);
/*parse information from xmlXPathObjectPtr and to allot static address*/
int Mu_ParseMultiFDDF(xmlXPathObjectPtr Ptr, FddfStatusHeadPtr back);
/*manner to choose way to parse xml*/
int Mu_ParseXpathXML(int type, xmlDocPtr Doc, xmlChar *xpath, void ** back);
/*manner to choose way to parse xml*/
int Mu_ParseXML(int type, xmlDocPtr doc, void ** back);
/*provide the interface to xml*/
int Mu_Parse (int type, const int fd , xmlChar *xpath, void ** back);
/*parse afterquery.xml*/
int Mu_ParseEyewearADP(xmlDocPtr doc, EyewearADPPtr back);
/*parse serviceinfo.xml*/
int Mu_ParseServiceInfo(xmlDocPtr doc, ServiceInfoPtr back);
/*parse FDDF.xml*/
int Mu_ParseEyewearFDDF(xmlDocPtr Doc,FddfFilePtr back);
/*parse update.xml*/
int Mu_ParseUpdateInfo(xmlDocPtr Doc,UpdateInfoPtr back);
/*parse deviceinformation.xml*/
int Mu_ParseDeviceInfo(xmlDocPtr Doc,XmlDeviceInfoPtr back);
/*parse resume.xml*/
int Mu_ParseResumeInfo(xmlDocPtr Doc,ResumeInfoPtr back);
/*parse fileinfo.xml*/
int Mu_ParseFileInfo(xmlDocPtr Doc,FileInfoPtr back);
/*parse the payload segment*/
int Mu_ParsePayload(xmlDocPtr Doc,SRV_SvrInfPtr srv);
#endif
