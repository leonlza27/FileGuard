#pragma once

#include<fltUser.h>

#define FG_RELOAD (void*)100

typedef struct CommandSend {
	int command;
	char ConfFilePath[2048];
}CommandSend;

__declspec(dllexport) int ConnectKernelFlt();

__declspec(dllexport) void SendReload();