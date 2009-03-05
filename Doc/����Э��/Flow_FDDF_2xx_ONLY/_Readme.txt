本文件说明基于FDDF的文件下载流程（不Logout）

文件说明
00_ServerInfo.xml
	服务器信息描述符，假设Device连接上的是Localhost

01_PreLogin.xml
	GET /SRV_PreLogin.php 返回的文件

02_Before_Login.xml
03_After_Login.xml
	构造并返回的Login XML文件

04_Before_Query.xml   
05_After_Query.xml    
	进行查询，返回带有FDDF的列表。这里2-0 = 2,所以会返回两个FDDF文件。

06_Before_QueryFDDF.xml
07_After_QueryFDDF.xml
	查询FDDF文件列表。

08_Before_Update.xml
09_after_Update.xml
	Update文件服务器下载完成信息。
	没有按照范围更新，而是更新两个独立的文件。

Logout略去。

FDDF分别示范没有分段的和有两个分段的情况。
