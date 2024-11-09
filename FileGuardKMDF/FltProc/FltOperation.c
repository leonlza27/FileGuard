#include"FltOperation.h"
#include"..\DBG.h"

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
	if (strcmp("explorer.exe", ProcName) != 0) {
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


	if (wcsstr(DosFilePath.Buffer, L"C:\\Users\\leonl\\Desktop\\Recv")) {
		pData->IoStatus.Status = STATUS_UNABLE_TO_DELETE_SECTION;
		pData->IoStatus.Information = 0;
		DbgProc(DbgPrint("[SysMiniFlt1] Blocked all operations for directory(file) <%wZ>\n", DosFilePath));
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
