#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "include/main.h"
#include "include/Mu_error.h"
#include "include/MuFTAD_Util.h"
#include "include/MuFTAD_App.h"
#include "include/MuFTAD_Init.h"
int main(void)
{
	threadnum= 0;	//for multi thread download
	need_retry = 0;
	
	//init the device and the date
	if(MuFTAD_DateInit() < 0
		||MuFTAD_DeviceInit(SERVICEINFO, DEVICEINFO) < 0){

		Mu_ErrorPrint();
		return MUEERO;
	}
	
	//prelogin and login
	if(MuFTAD_PreLogin() < 0
		||MuFTAD_EyeADPSndInit() < 0
		||MuFTAD_Login() < 0){

		Mu_ErrorPrint();
		return MUEERO;
	}
	
	//check resume
	if(MuFTAD_Resume() == MUEDNC)
		return MUEERO;

	//query server and download the programms
	MuFTAD_Query110();
	MuFTAD_Query110Proc();
	
	//logout
	MuFTAD_Logout102();

	MuFTAD_FreeSource();
	return MUOK;

}

