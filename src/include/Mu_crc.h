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

//Length for CRC check

#ifndef __Mu_CRC_H__
#define __Mu_CRC_H__

#define MAX_CRC_LEN 1023

int Mu_Crc32File(int fd, unsigned int crc32);
int Mu_Crc32Segment(int fd,off_t seek,size_t char_length,unsigned int crc32);
#endif
