模块项目名称：CRC32&MD5    代码创建人；王可心  代码创建日期：2008-05-04
──────────────────────────────────────
修改：2008-05-07
修改：2008-05-09  原来的文件指针改为文件描述符，测试完毕。


──────────────────────────────────────

功能描述：

	文件或分段文件的CRC32及MD5校验。

对外接口：

	参数约定：

		fp--文件指针，如没有可通过文件描述符提供：FILE * fp=fdopen(int fd,const char * mode);

		seek--分段起点

		char_length--分段长度

		crc32--文件或分段的源CRC32校验码
		
		md5--文件或分段的源MD5校验码

	CRC32部分：Mu_crc.c

		//文件CRC32校验

		int Mu_Crc32File(FILE *fp, unsigned int crc32);

		//文件分段CRC32校验

		int Mu_Crc32Segment(FILE *fp,size_t seek,size_t char_length,unsigned int crc32);


	MD5部分：Mu_md5.c

		//文件MD5校验

		int Mu_Md5File(FILE *fp, char *md5);


		//文件分段MD5校验

		int Mu_Md5Segment(FILE *fp, size_t seek, size_t char_length, char *md5)

