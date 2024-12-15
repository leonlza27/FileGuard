#include"FltOperation.h"
#include"..\DBG.h"

#include"../TgStorage/strtree.h"


extern StrTree* FgTgStorage;
int AllowFliting = 1;

NTSTATUS CovertDevPathToDosPath(PFLT_FILE_NAME_INFORMATION pFileInfo, PDEVICE_OBJECT pDiskObj, UNICODE_STRING* pDestFilePath) {
	NTSTATUS status = STATUS_SUCCESS;
	wchar_t pathbuf[5] = { 0 };
	UNICODE_STRING DosDiskName = { 0 };
	RtlInitEmptyUnicodeString(&DosDiskName, pathbuf, sizeof(pathbuf));

	__try {
		status = IoVolumeDeviceToDosName(pDiskObj, &DosDiskName);
		if (!NT_SUCCESS(status)) {
			return status;
		}

	}
	__finally {
		ObDereferenceObject(pDiskObj);
	}

	status = RtlAppendUnicodeStringToString(pDestFilePath, &DosDiskName);
	if (!NT_SUCCESS(status)) {
		return status;
	}
	status = RtlAppendUnicodeStringToString(pDestFilePath, &pFileInfo->ParentDir);
	if (!NT_SUCCESS(status)) {
		return status;
	}
	status = RtlAppendUnicodeStringToString(pDestFilePath, &pFileInfo->FinalComponent);
	if (!NT_SUCCESS(status)) {
		return status;
	}

	return STATUS_SUCCESS;
}


FLT_PREOP_CALLBACK_STATUS PreFltCreate(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObj, PVOID* CompletionContext) {

	PEPROCESS pProc = 0;
	PCHAR ProcName = 0;
	PFLT_FILE_NAME_INFORMATION pFileInfo = 0;
	NTSTATUS status = STATUS_SUCCESS;
	wchar_t pathbuf[260] = { 0 };
	UNICODE_STRING DosFilePath = { 0 };
	RtlInitEmptyUnicodeString(&DosFilePath, pathbuf, sizeof(pathbuf));
	PDEVICE_OBJECT pDiskObj;

	pProc = FltGetRequestorProcess(pData);
	ProcName = PsGetProcessImageFileName(pProc);
	if (strcmp("FileGuardUSR.exe", ProcName) == 0) {
		return FLT_POSTOP_FINISHED_PROCESSING;
	}

	status = FltGetFileNameInformation(pData, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_FILESYSTEM_ONLY, &pFileInfo);
	if (!NT_SUCCESS(status)) {
		return FLT_POSTOP_FINISHED_PROCESSING;
	}

	__try {

		status = FltParseFileNameInformation(pFileInfo);
		if (!NT_SUCCESS(status)) {
			return FLT_POSTOP_FINISHED_PROCESSING;
		}

	}
	__finally {
		FltReleaseFileNameInformation(pFileInfo);

	}

	status = FltGetDiskDeviceObject(pFltObj->Volume, &pDiskObj);
	if (!NT_SUCCESS(status)) {
		return status;
	}

	status = CovertDevPathToDosPath(pFileInfo, pDiskObj, &DosFilePath);

	//wcsstr(DosFilePath.Buffer, L"C:\\Users\\leonl\\Desktop\\Recv"

	char buf[400];
	sprintf(buf, "%ls", DosFilePath.Buffer);

	if (HaveStrOrSubStr(FgTgStorage, buf)&&AllowFliting) {
		pData->IoStatus.Status = STATUS_UNABLE_TO_DELETE_SECTION;
		pData->IoStatus.Information = 0;
		return FLT_PREOP_COMPLETE;
	}


	return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS PreFltDirCtl(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObj, PVOID* CompletionContext) {
	return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_POSTOP_CALLBACK_STATUS PostFltCreate(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObj, PVOID CompletionContext, FLT_POST_OPERATION_FLAGS Flags) {
	return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_POSTOP_CALLBACK_STATUS PostFltDirCtl(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObj, PVOID CompletionContext, FLT_POST_OPERATION_FLAGS Flags) {
	return FLT_POSTOP_FINISHED_PROCESSING;
}

int RefreshTgTree() {
	HANDLE ConfFile;
	OBJECT_ATTRIBUTES oaConfName;
	IO_STATUS_BLOCK iosBlock;
	size_t fileOffset = 0;
	UNICODE_STRING OP_ConfFilePath = RTL_CONSTANT_STRING(CONFPATH);
	InitializeObjectAttributes(&oaConfName, &OP_ConfFilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, 0);
	if (!NT_SUCCESS(ZwOpenFile(&ConfFile, FILE_READ_DATA, &oaConfName, &iosBlock, FILE_SHARE_READ, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT))) {
		return 1;
	}
	
	__int64 byteOffset = sizeof(FileHeader);

	FileHeader OpenedHeader;
	ZwReadFile(ConfFile, 0, 0, 0, &iosBlock, &OpenedHeader, sizeof(FileHeader), 0, 0);
	if (!FILEHEADER_CONFIRM(OpenedHeader)) {
		ZwClose(ConfFile);
		return 1;
	}

	AllowFliting = 0;
	ClearAll(FgTgStorage->head);
	TgItemInfo infomid;

	for (unsigned int i = 0; i < OpenedHeader.ItemCount; i++) {
		ZwReadFile(ConfFile, 0, 0, 0, &iosBlock, &infomid, sizeof(TgItemInfo), byteOffset, 0);
		if(infomid.IsSettedProtect) AddString(FgTgStorage, infomid.Path);
		byteOffset += sizeof(TgItemInfo);
	}


	AllowFliting = 1;
	ZwClose(ConfFile);
	return 0;
}
