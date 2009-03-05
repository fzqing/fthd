#ifndef __MUFTAD_PROC_H__
#define __MUFTAD_PROC_H__
#include "Mu_Creatxml.h"
#include "Mu_Struct_main.h"
#include "Mu_Buildxml.h"
#include "Mu_Parsexml.h"	//for CHECK_2XX

#include "Mu_Util.h"
#include "Mu_HttpUtil.h"
#include "Mu_Http.h"
#include "Mu_HttpDown.h"
#include "Mu_md5.h"

#define LOGIN 100
#define LOGOUT101 101
#define LOGOUT102 102
#define QUERY110 110
#define QUERY111 111
#define UPSTAT120 120
#define UPDN121 121
#define UPFILE122 122


int MuFTAD_EyeADPPost(int opccode);
int MuFTAD_QueryListPost(void *Ptr, int type);

int MuFTAD_UpDnPost(void *Ptr, int opccode, int type, int suc);
int MuFTAD_XmlProc(int statcode);
int MuFTAD_ProcLogin200(void);
int MuFTAD_Proc200(int opccode);
int MuFTAD_Proc300(void);
int MuFTAD_Proc301(void);
int MuFTAD_UpSRV120Post(void);
int MuFTAD_ProcQueryStat200(void);

int MuFTAD_QueryPost(int querytype);

int MuFTAD_GetStoreFd(int opccode);
char *MuFTAD_GetAPI(int opccode);

int MuFTAD_MakeURL(char **query, int method, char *path);

int MuFTAD_Resume2XXCheck(DatabaseInfoPtr *Ptr, const char *name);


#endif
