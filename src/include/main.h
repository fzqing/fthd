#ifndef __MAIN_H__
#define __MAIN_H__
#include "Mu_Http.h"

Mu_DnStat MuDnStat;
#define SERVICEINFO "seviceinfo.xml"
#define DEVICEINFO "deviceinfo.xml"
volatile int need_retry; //must be set to 1

#define PROCVERSION 1.0
#define FIREWEAREVERSION 1.0

#define RESUME_FILE "resume.xml"
#define UPDATEPATH "updatefile"
#define UPDATEFILE "update_SRV.xml"
#define FIREWEARE "lib"


#endif
