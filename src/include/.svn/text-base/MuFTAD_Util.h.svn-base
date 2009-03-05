#ifndef __MUFTAD_UTIL_H__
#define __MUFTAD_UTIL_H__
#include "Mu_Struct_main.h"
#include "Mu_Parsexml.h"
#include "Mu_NetIO.h"
#include "Mu_error.h"
#include "Mu_md5.h"
#include "Mu_crc.h"

#include "Mu_Http.h"

#include "Mu_Util.h"

int MuFTAD_PreLogin(void);
int MuFTAD_Login(void);
int MuFTAD_UpSRV120(void);
int MuFTAD_UpSRV121(void *Ptr, int type, int suc);
int MuFTAD_UpSRV122(void *Ptr, int type, int suc);
int MuFTAD_Logout101(void);
int MuFTAD_Logout102(void);
int MuFTAD_Query110(void);
int MuFTAD_Query111(void *Ptr, int type, int resume);
int MuFTAD_EyeADPSndInit(void);
int MuFTAD_DnUpdate(void);
int MuFTAD_Query110Proc(void);
int MuFTAD_QueryFddf(const char *url, int authmethod, int resume);
int MuFTAD_Download2XX(int fd, off_t offset, SegmentNodePtr Ptr, int resume);
int MuFTAD_Download4XX(const char *url, int type, int authmethod);
int MuFTAD_Resume(void);
int MuFTAD_Resume2XX(int i);
int MuFTAD_Resume4XX(int i);


#endif
