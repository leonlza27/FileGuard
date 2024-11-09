#include"Kernel_User_Conn.h"

NTSTATUS FltConnectNotify(PFLT_PORT pClientPort, PVOID pServerPortCoockie, PVOID pConnContext, ULONG SizeofContext, PVOID* ConnectionPortCookie) {

	return 0;
}

void FltDisconnectNotify(PVOID ConnecionCookie) {

}

NTSTATUS FltMessageNotify(PVOID PortCookie, PVOID InputBuffer, ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLength, PULONG ReturnOutputBufferLength) {
	NTSTATUS status = STATUS_SUCCESS;

	if (InputBuffer == 0 || InputBufferLength == 0) {
		status = STATUS_INVALID_PARAMETER;
		return status;
	}

	return status;
}
