#include "pch.h"
#include "KernConn.h"

HANDLE FilterPort;
char returnval;

__declspec(dllexport) int ConnectKernelFlt() {
	if (FilterConnectCommunicationPort(L"\\FileGuard", 0, 0, 0, 0, &FilterPort)!=0) {
		return 1;
	}
	return 0;

	
}

__declspec(dllexport) void SendReload() {
	DWORD byteGettedReturn;
	
	FilterSendMessage(FilterPort, FG_RELOAD, sizeof(char), &returnval, sizeof(char), &byteGettedReturn);

}
