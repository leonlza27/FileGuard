#include<ntifs.h>
#include<fltKernel.h>

//与应用层通信

#define FG_RELOAD (void*)100

NTSTATUS FltConnectNotify(PFLT_PORT pClientPort, PVOID pServerPortCoockie, PVOID pConnContext, ULONG SizeofContext, PVOID* ConnectionPortCookie);
void FltDisconnectNotify(PVOID ConnecionCookie);
NTSTATUS FltMessageNotify(PVOID PortCookie, PVOID InputBuffer, ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLength, PULONG ReturnOutputBufferLength);