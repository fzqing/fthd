#ifndef __MUFTAD_APP_H__
#define __MUTFAD_APP_H__
#include "Mu_Struct_main.h"
#include "Mu_xml_parse.h"
#include "Mu_Buildxml.h"
#include "Mu_Creatxml.h"
#include <unistd.h>
#include "Mu_Util.h"


void MuFTAD_ErrorProc(int ret);

int MuFTAD_CopyInfoToDBII(FddfStatusNodePtr Ptr, DBIInfoPtr DbiInfo);
int MuFTAD_CopyInfoToDBUD(DirStatusNodePtr Ptr, DBUDirtPtr Dbudirt, int suc);
int MuFTAD_CopyInfoToDBUR(DatabaseInfoPtr Ptr, DBURatePtr Dburate, int rate);
int MuFTAD_CopyInfoToDBUI(DatabaseInfoPtr Ptr, DBUInfoPtr DbuInfo, int suc);

int MuFTAD_RecordResum(ResumeInfoPtr resume, int type, const char *name, int *i);
int MuFTAD_DeleteResum(ResumeInfoPtr resume, int i);
int MuFTAD_UpdateTime(char *Time);
int MuFTAD_UpdateID(const char *IdInit,  const char *IdApp);
int MuFTAD_UpdateConv(ConversationPtr *Ptr);

#endif

