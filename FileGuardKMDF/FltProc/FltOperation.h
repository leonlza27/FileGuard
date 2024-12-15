#pragma once
#include<ntifs.h>
#include<fltKernel.h>

//文件内容组成

#define FILEHEADER_CONFIRM(fheader) ((fheader).idPara[0] == 0xC232D64D1E48872F && (fheader).idPara[1] == 0x9FAE12F34CFD747F)

#define CONFPATH L"\\\\??\\\\C:\\Windows\\INF\\CONF.fgconf"

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

//过滤过程

NTSTATUS CovertDevPathToDosPath(PFLT_FILE_NAME_INFORMATION pFileInfo, PDEVICE_OBJECT pDiskObj, UNICODE_STRING* pDestFilePath);
PCHAR PsGetProcessImageFileName(PEPROCESS Process);


//预处理回调

FLT_PREOP_CALLBACK_STATUS PreFltCreate(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObj, PVOID* CompletionContext);
FLT_PREOP_CALLBACK_STATUS PreFltDirCtl(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObj, PVOID* CompletionContext);


//后处理回调

FLT_POSTOP_CALLBACK_STATUS PostFltCreate(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObj, PVOID CompletionContext, FLT_POST_OPERATION_FLAGS Flags);
FLT_POSTOP_CALLBACK_STATUS PostFltDirCtl(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObj, PVOID CompletionContext, FLT_POST_OPERATION_FLAGS Flags);


//更新目录索引树
int RefreshTgTree();