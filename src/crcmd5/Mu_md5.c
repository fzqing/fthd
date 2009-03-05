/****************************************************************
* ====Microunit Techonogy Co,.LTD.====
* File Name: 
*
* CRC-32 & MD5.c
* 
* Description:
*
* Use the MD5 in this file,
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
* MD5 check
*/

//Default include

#include <stdio.h>

#include <string.h>

#include <sys/types.h>

#include <unistd.h>

//Md5 library

#include <openssl/md5.h>

//SSL library

#include <openssl/ssl.h>

//Global define head file

#include "../include/Mu_md5.h"

//Error code define head file

#include "../include/Mu_error.h"

//Files's MD5 code check

int Mu_Md5File(int fd, char *md5){

	char output[33] = {""};

	char output1[32];

	MD5_CTX context;

	size_t length = 0;

	size_t bytes_read;

	int retval = MUCNOM;

	unsigned char buf[1023 + 1];

	unsigned char digest[MD5_DIGEST_LENGTH];

	MD5_Init(&context);

	memset(buf, 0, 1023 + 1);

    while ((bytes_read = read (fd,buf,1024))>0){

		MD5_Update (&context, buf, bytes_read);

		if(bytes_read==0){
			
			break;

		}
		length+=bytes_read;
	}

    MD5_Final (digest, &context);

	int i;

    for (i = 0; i < 16; i++){

		sprintf(&(output1[2*i]),"%02x",(unsigned char)digest[i]);

        sprintf(&(output1[2*i+1]),"%02x",(unsigned char)(digest[i]<<4));

	}

    for(i=0;i<32;i++){

       output[i]=output1[i];

	}

	//Uppercase

	ToUpper(output);

	ToUpper(md5);

	retval = strncmp(output, md5, 32);

	//Return

	if(retval==0){

		return MUOK;

	}else{

		return retval;

	}

	return MUFRAD;
}


//Segment MD5 code check

int Mu_Md5Segment(int fd, off_t seek, size_t char_length, char *md5){
	
	char output[33] = {""};

	char output1[32];

	MD5_CTX context;

	size_t length = 0;

	size_t bytes_read;

	int retval = MUFRAD;

	unsigned char buf[1023 + 1];

	unsigned char digest[MD5_DIGEST_LENGTH];

	MD5_Init(&context);

	memset(buf, 0, 1023 + 1);

	lseek(fd,seek,SEEK_SET);

	printf("char_length:%d\n",char_length);

    while ((bytes_read = read (fd,buf,1024))>0){

		while(length + bytes_read > char_length){

			bytes_read--;

		}

		length += bytes_read;

		MD5_Update (&context, buf, bytes_read);

		if(bytes_read==char_length){

			break;

		}

		if(bytes_read==0){
			
			break;

		}

	}

    MD5_Final (digest, &context);

	int i;

    for (i = 0; i < 16; i++){

		sprintf(&(output1[2*i]),"%02x",(unsigned char)digest[i]);

        sprintf(&(output1[2*i+1]),"%02x",(unsigned char)(digest[i]<<4));

	}

    for(i=0;i<32;i++){

       output[i]=output1[i];

	}

	//Uppercase befor check

	ToUpper(output);

	ToUpper(md5);

	retval = strncmp(output, md5, 32);

	//Return

	if(retval==0){

		return MUOK;

	}else{

		return retval;

	}

	return MUFRAD;

}

static void ToUpper(char s[]){
	int i=0;
	while(s[i]!='\0'){
		if(s[i]>='a'&&s[i]<='z'){
			s[i] -= 32;
		}
		i++;
	}
}
