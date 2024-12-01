#include "MainWnd.h"

INT_PTR MainWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	HICON ico = LoadIcon(GetModuleHandleA(0), MAKEINTRESOURCEW(IDI_ICON1));
	switch (message) {
	case WM_INITDIALOG:

		//SetMenu(hDlg, LoadMenu(GetModuleHandleW(0), MAKEINTRESOURCEW(IDC_PROJECTTEST)));

		
		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)ico);
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		return 0;
		break;
	default:
		break;
	}
	return 0;
}
