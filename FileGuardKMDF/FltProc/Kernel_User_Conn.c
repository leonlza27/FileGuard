#include "Kernel_User_Conn.h"
#include "FltOperation.h"

PFLT_PORT cliport;
extern PFLT_FILTER pg_Filter;

NTSTATUS FltConnectNotify(PFLT_PORT pClientPort, PVOID pServerPortCoockie, PVOID pConnContext, ULONG SizeofContext, PVOID* ConnectionPortCookie) {
	cliport = pClientPort;
	return 0;
}

void FltDisconnectNotify(PVOID ConnecionCookie) {
	FltCloseClientPort(pg_Filter, cliport);
}

NTSTATUS FltMessageNotify(PVOID PortCookie, PVOID InputBuffer, ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLength, PULONG ReturnOutputBufferLength) {
	NTSTATUS status = STATUS_SUCCESS;

	if (InputBuffer == 0 || InputBufferLength == 0) {
		status = STATUS_INVALID_PARAMETER;
		return status;
	}

	if (InputBuffer == FG_RELOAD) {
		RefreshTgTree();
	}

	return status;
}
