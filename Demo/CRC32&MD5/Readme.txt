########################################################################
测试程序主入口：

int main(void);

功能：

	测试CRC32校验：
		
		int CRC32_a();//文件校验
		　　　　│
		　　　　└Mu_Crc32File(fp,crc);//库接口

		int CRC32_b();//文件分段校验
		　　　　│
		　　　　└Mu_Crc32Segment(fp,start,len,crc);//库接口

	测试MD5校验：
		int MD5_a();//文件MD5校验
		　　　　│
		　　　　└Mu_Md5File(fp,md5);//库接口

		int MD5_b();//文件分段MD5校验
		　　　　│
		　　　　└Mu_Md5Segment(fp,start,len,md5);//库接口


#########################################################################



Demo测试步骤：

1、运行测试程序：
	[~]$./demo

2、选择校验算法“1”或“2”

Select check type:
          1-CRC32
          2-MD5

3、选择校验选项，“1”文件或“2”文件分段。

CRC32 Check option:

          <1>File CRC32 check
          <2>File segment CRC32 check

MD5 Check option:

          <1>File MD5 check
          <2>File segment MD5 check

4、文件校验提示输入参数：

CRC32提示输入:

	>>Input a file name to check://输入文件名

	>>Input the CRC32 code of the file://输入文件的CRC32码

MD5提示输入:

	>>Input a file name to check://输入文件名

	>>Input the MD5 code of the file://输入文件的MD5码

4、文件分段校验提示输入参数：

CRC32提示输入:

	>>Input a file name to check://输入文件名

	>>Input point of seek://输入分段起点

	>>Input length to check://输入分段长度

	>>Input the CRC32 code of the segment://输入分段的CRC32码


MD5提示输入:

	>>Input a file name to check://输入文件名

	>>Input point of seek://输入分段起点

	>>Input length to check://输入分段长度

	>>Input the MD5 code of the segment://输入分段的CRC32码

5、验证结果返回：


CRC32：
	>>CRC32 check success!//文件校验成功

	>>CRC32 check Failed!//文件校验失败

	>>CRC32 segment check success!//文件分段校验成功

	>>CRC32 segment check Failed!//文件分段校验失败

MD5：
	>>MD5 check success!//文件校验成功

	>>MD5 check Failed!//文件校验失败

	>>MD5 segment check success!//文件分段校验成功

	>>MD5 segment check Failed!//文件分段校验失败




