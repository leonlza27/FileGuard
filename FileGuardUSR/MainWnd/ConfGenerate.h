#pragma once

#include <Windows.h>
#include <commctrl.h>
#include <stdio.h>

#define CONFPATH "C:\\Windows\\INF\\CONF.fgconf"

#define FILEHEADER_SETUP(pfheader) {(pfheader)->idPara[0] = 0xC232D64D1E48872F;(pfheader)->idPara[1] = 0x9FAE12F34CFD747F; }
#define FILEHEADER_CONFIRM(fheader) ((fheader).idPara[0] == 0xC232D64D1E48872F && (fheader).idPara[1] == 0x9FAE12F34CFD747F)

#pragma pack(1)
typedef struct FileHeader {
	unsigned __int64 idPara[2];
	unsigned int ItemCount;
}FileHeader;

typedef struct TgItemInfo {
	wchar_t ObjName[1024];//附加信息
	wchar_t Path[1024];//文件(夹)路径
	int IsSettedProtect;
}TgItemInfo;

void GenerateConfigFile(HWND lst);

void LoadConfigFile(HWND lst);