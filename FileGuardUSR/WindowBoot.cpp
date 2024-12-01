#include"WindowBoot.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {

	//自动兼容字体
	SetProcessDPIAware();
	//拉起主窗口
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINWND), 0, MainWndProc);
}

