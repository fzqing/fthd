/****************************************************************
* ====Microunit Techonogy Co,.LTD.====
* File Name: 
*
* CRC-32 & MD5.c
* 
* Description:
*
* Use the Routines contained in this file,
*
* to checksum the files,or the segment of the file.
* 
* Revision History:
*
* 20-3-2008 ver1.0
*
* Author:wkx(kevin@microunit.com.cn)
* 
*				***PROTECTED BY COPYRIGHT***
****************************************************************/

/*
* Global define
*/


#ifndef __MU_MD5_H__
#define __MU_MD5_H__

static void ToUpper(char *s);
int Mu_Md5File(int fd, char *md5);
int Mu_Md5Segment(int fd, off_t seek, size_t char_length, char *md5);
#endif
