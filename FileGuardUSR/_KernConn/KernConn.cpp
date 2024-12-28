#include "pch.h"
#include "KernConn.h"

#pragma comment(linker,"/section:Shared,rws")

HANDLE FilterPort;
char returnval;
SC_HANDLE hServiceMgr = NULL;
SC_HANDLE hFgServiceDDK = NULL;

SERVICE_STATUS serviceStatus = { 0 };

#pragma data_seg("Shared")
HWND regedWnd = 0;
#pragma data_seg()

__declspec(dllexport) int ConnectKernelFlt() {
	if (FilterConnectCommunicationPort(L"\\FileGuard_Port", 0, 0, 0, 0, &FilterPort)!=0) {
		return 1;
	}
	return 0;

	
}

__declspec(dllexport) void SendReload() {
	DWORD byteGettedReturn;
	
	FilterSendMessage(FilterPort, FG_RELOAD, sizeof(char), &returnval, sizeof(char), &byteGettedReturn);
	
}

void DisconnectKernelFlt() {
	CloseHandle(FilterPort);
}

void unloadDrvService()	{
	CloseServiceHandle(hFgServiceDDK);
	CloseServiceHandle(hServiceMgr);
}

int loadDrvService() {
	hServiceMgr = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	hFgServiceDDK = OpenServiceA(hServiceMgr, "FileGuard", SERVICE_ALL_ACCESS);
	if (hFgServiceDDK == NULL) {
		CloseServiceHandle(hFgServiceDDK);
		CloseServiceHandle(hServiceMgr);
		return 0;
	}
	return 1;
}

int getDrvServiceStatus() {
	QueryServiceStatus(hFgServiceDDK, &serviceStatus);
	switch (serviceStatus.dwCurrentState) {
	case SERVICE_RUNNING:
	case SERVICE_PAUSE_PENDING:
	case SERVICE_STOP_PENDING:
		return Running;
		break;

	case SERVICE_PAUSED:
	case SERVICE_CONTINUE_PENDING:
		return Paused;
		break;

	case SERVICE_STOPPED:
	case SERVICE_START_PENDING:
		return Closed;
		break;
	}
	
}

static SERVICE_STATUS SvrSta;

void startDRV(){
	switch (serviceStatus.dwCurrentState) {
		case SERVICE_PAUSED:
			ControlService(hFgServiceDDK, SERVICE_CONTROL_CONTINUE, &SvrSta);
			break;
		case SERVICE_STOPPED:
			StartService(hFgServiceDDK, 0, 0);
			break;
	}
}

void stopDRV() {
	ControlService(hFgServiceDDK, SERVICE_CONTROL_STOP, &SvrSta);
}

int AppProcRegCtl(HWND regnewwnd) {
	if (regedWnd != 0) {
		ShowWindow(regedWnd, SW_SHOW);
		SetForegroundWindow(regedWnd);
		SetFocus(regedWnd);
		return 1;
	}
	regedWnd = regnewwnd;
	return 0;
}
