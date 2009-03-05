
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>


#define NONE         "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"

//CRC32 file check
int CRC32_a(){

	int fd;
	unsigned int crc;
	char file[50];
	printf(">>Input a file name to check:");
	scanf("%s",&file);
	printf(">>Input the CRC32 code of the file:");
    scanf("%08lx",&crc);
	if((fd=open(file,"O_RDONLY"))==-1){
          printf(">>Error to open file:\"%s\"!\n",file);
	}else{
		if(Mu_Crc32File(fd,crc)==0){
			printf(LIGHT_CYAN ">>CRC32 check success!\n");
		}else{
			printf(LIGHT_CYAN ">>CRC32 check Failed!\n");
		}
	}
	return 0;
}

//CRC32 segment check
int CRC32_b(){

	int fd;
	char file[50];
	unsigned int crc;
	unsigned int start;
	unsigned int len;

	printf(">>Input a file name to check::");
	scanf("%s",&file);
	printf(">>Input point of seek:");
	scanf("%d",&start);
	printf(">>Input length to check:");
	scanf("%d",&len);
	printf(">>Input the CRC32 code of the segment:");
    scanf("%08lx",&crc);

	if((fd=open(file,"O_RDONLY"))==-1){
          printf(">>Error to open file:\"%s\"!\n",file);
	}else{
		if(Mu_Crc32Segment(fd,start,len,crc)==0){
			printf(LIGHT_CYAN ">>CRC32 segment check success!\n");
		}else{
			printf(LIGHT_CYAN ">>CRC32 segment check Failed!\n");
		}
	}

	return 0;
}


//Md5 file check
int MD5_a(){
	int fd=-1;
	char *md5[32];
	char file[50];
	printf(">>Input a file name to check:");
	scanf("%s",file);
	printf(">>Input the MD5 code of the file:");
    scanf("%s",&md5);
	if((fd=open(file,"O_RDONLY"))==-1){
		printf(">>Error to open file:\"%s\"!\n",file);
	}else{
		if(Mu_Md5File(fd,md5)==0){
			printf(LIGHT_CYAN ">>MD5 check success!\n");
		}else{
			printf(LIGHT_CYAN ">>MD5 check Failed!\n");
		}
		close(fd);
	}
	return 0;
}

int MD5_b(){

	int fd;
	char file[50];
	char *md5[32];
	unsigned int start;
	unsigned int len;

	printf(">>Input a file name to check:");
	scanf("%s",&file);
	printf(">>Input point of seek:");
	scanf("%d",&start);
	printf(">>Input length to check:");
	scanf("%d",&len);
	printf(">>Input the MD5 code of the segment:");
    scanf("%s",&md5);

	if((fd=open(file,"O_RDONLY"))==-1){
          printf(">>Error to open file:\"%s\"!\n",file);
	}else{
		if(Mu_Md5Segment(fd,start,len,md5)==0){
			printf(LIGHT_CYAN ">>MD5 segment check success!\n");
		}else{
			printf(LIGHT_CYAN ">>MD5 segment check Failed!\n");
		}
	}

	return 0;
}

//Main

int main(void){
	int i_sel=0;
	printf(LIGHT_PURPLE "Select check type:\n          1-CRC32\n          2-MD5\n");
	scanf("%d",&i_sel);
	if(i_sel==1){
		int j;
		printf("Check option:\n          <1>File CRC32 check\n          <2>File segment CRC32 check\n");
		scanf("%d",&j);
		if(j==1){
			CRC32_a();
		}

		if(j==2){
			CRC32_b();
		}
	}

	if(i_sel==2){
		int j;
		printf("Check option:\n          <1>File MD5 check\n          <2>File segment MD5 check\n");
		scanf("%d",&j);
		if(j==1){
			MD5_a();
		}
		if(j==2){
			MD5_b();
		}
	}

}
