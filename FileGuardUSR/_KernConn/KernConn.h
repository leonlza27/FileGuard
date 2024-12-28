#pragma once

#include<fltUser.h>

#define FG_RELOAD (void*)100

typedef struct CommandSend {
	int command;
	char ConfFilePath[2048];
}CommandSend;

enum DrvServiceStatus{Running = 0,Paused,Closed};	//0->正在运行 1->挂起 2->已关闭		

__declspec(dllexport) int ConnectKernelFlt();

__declspec(dllexport) void SendReload();

__declspec(dllexport) void DisconnectKernelFlt();

__declspec(dllexport) void unloadDrvService();

__declspec(dllexport) int loadDrvService();

__declspec(dllexport) int getDrvServiceStatus();

__declspec(dllexport) void startDRV();

__declspec(dllexport) void stopDRV();

__declspec(dllexport) int AppProcRegCtl(HWND regnewwnd);