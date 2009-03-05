#ifndef __MU_HTTPUTIL_H__
#define __MU_HTTPUTIL_H__

#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>

#include "Mu_Http.h"
#include "Mu_error.h"
#include "Mu_NetIO.h"
#include "Mu_Util.h"

#include "Mu_HttpParse.h"

#define MAX_THD_LEN 0x0800
void Mu_Base64Encode(const char *s, char *store, int length);
char *Mu_GetCookiesForHead();
char *Mu_GetBasicAuth(const char *user, const char *passwd);

void Mu_InitURL(Mu_URLPtr mu_url);
void Mu_FreeURL(Mu_URLPtr Ptr);

void Mu_InitBuild(Mu_BuildPtr build);
void Mu_InitDnStat(Mu_DnStatPtr MuDnStat);
void Mu_InitHttpStat(Mu_HttpStatusPtr Ptr);
int Mu_InitThreads(off_t length);
#endif



