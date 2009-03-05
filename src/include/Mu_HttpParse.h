#ifndef __MU_PARSE_H__
#define __MU_PARSE_H__
#include "Mu_error.h"

#include "Mu_NetIO.h"

#include "Mu_Http.h"
#include "Mu_HttpUtil.h"

#include "Mu_Util.h"

char *cookies;
char *location;

int Mu_GetCookies(const char *hdr);
int Mu_GetStatusCode(const char *hdr);
off_t Mu_GetContentLen(const char *hdr);
int Mu_ParseHeader(int socket, Mu_HttpStatusPtr Ptr);
int Mu_FetchHeader(int socket,char **hdr);
int Mu_GetRelocation(const char *hdr);

#endif

