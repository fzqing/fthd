#ifndef __MU_HTTPBUILD_H__
#define __MU_HTTPBUILD_H__

#include "Mu_Http.h"
#include "Mu_error.h"
#include "Mu_NetIO.h"
#include "Mu_HttpUtil.h"

#include "Mu_HttpParse.h"
#include "Mu_Util.h"

int Mu_BuildQuery(Mu_Build build, char **Ptr);
int Mu_BuildQueryGet(Mu_Build build, char **Ptr);
int Mu_BuildQueryHead(Mu_Build build, char **Ptr);
int Mu_BuildQueryPost(Mu_Build build, char **Ptr);


#endif
