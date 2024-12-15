#include"Main.h"
#include"DBG.h"

#include"TgStorage/strtree.h"
#include"FltProc\FltOperation.h"

extern StrTree* FgTgStorage;

FLT_OPERATION_REGISTRATION FltCallbacks[] = {
	{IRP_MJ_CREATE,0,PreFltCreate,PostFltCreate},
	{IRP_MJ_DIRECTORY_CONTROL,0,PreFltDirCtl,PostFltDirCtl},
	{IRP_MJ_OPERATION_END}
};


FLT_REGISTRATION FltRegistry = {
	sizeof(FLT_REGISTRATION),
	FLT_REGISTRATION_VERSION,
	0,
	0,
	FltCallbacks,
	FltUnload,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

PFLT_FILTER pg_Filter = 0;
PFLT_PORT pgServer = 0;


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegPath) {
	NTSTATUS status = STATUS_SUCCESS;
	//KdBreakPoint();
	DbgProc(DbgPrint("[SysMiniFlt1] Filter load\n"));


	status = FltRegisterFilter(pDriverObj, &FltRegistry, &pg_Filter);
	if (!NT_SUCCESS(status)) {
		DbgProc(DbgPrint("[SysMiniFlt1] Failed to register\n"));
		return status;
	}

	FgTgStorage = initStrTree();
	if(RefreshTgTree()) return STATUS_UNSUCCESSFUL;


	//创建通讯端口

	OBJECT_ATTRIBUTES ObjAttr = { 0 };
	PSECURITY_DESCRIPTOR pSercurity = 0;
	UNICODE_STRING CommPortName = RTL_CONSTANT_STRING(L"\\FileGuard_Port");
	status = FltBuildDefaultSecurityDescriptor(&pSercurity, FLT_PORT_ALL_ACCESS);
	if (!NT_SUCCESS(status)) {
		DbgProc(DbgPrint("[SysMiniFlt1] Failed to open build security desriptor\n"));
		return status;
	}
	InitializeObjectAttributes(&ObjAttr, &CommPortName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 0, pSercurity);

	status = FltCreateCommunicationPort(pg_Filter, &pgServer, &ObjAttr, 0, FltConnectNotify, FltDisconnectNotify, FltMessageNotify, 1);
	FltFreeSecurityDescriptor(pSercurity);
	if (!NT_SUCCESS(status)) {
		DbgPrint("[SysMiniFlt1] Failed to create comm port\n");
		FltCloseCommunicationPort(pgServer);
		FltUnregisterFilter(pg_Filter);
		return status;
	}

	status = FltStartFiltering(pg_Filter);
	if (!NT_SUCCESS(status)) {
		DbgProc(DbgPrint("[SysMiniFlt1] Failed to start\n"));
		return status;
	}

	DbgProc(DbgPrint("[SysMiniFlt1] Load succeed\n"));

	return status;
}

NTSTATUS FltUnload(FLT_FILTER_UNLOAD_FLAGS Flags) {
	NTSTATUS status = STATUS_SUCCESS;
	FltCloseCommunicationPort(pgServer);
	FltUnregisterFilter(pg_Filter);
	DbgProc(DbgPrint("[SysMiniFlt1] Filter unload\n"));
	return status;
}
