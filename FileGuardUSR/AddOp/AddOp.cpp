#include "AddOp.h"

static HWND DlgMainWnd;/*对话框主窗口(句柄)*/
static HINSTANCE DlgHins;/*对话框实例*/

static wchar_t filename[1024];
static HWND objNameStor, objPathStor;//对象名;对象路径
static HWND s_callback;

INT_PTR AddOpProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	static HDC hdcStatic;
	static HBRUSH hBrush;
	switch (message) {
	case WM_INITDIALOG:
		hBrush = CreateSolidBrush(RGB(240, 240, 240));
		DlgMainWnd = hDlg;
		DlgHins = GetModuleHandleW(0);
		SendMessageW(DlgMainWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIconW(DlgHins, MAKEINTRESOURCEW(IDI_ICON1)));
		objNameStor = GetDlgItem(DlgMainWnd, IDC_OBJNAME);
		objPathStor = GetDlgItem(DlgMainWnd, IDC_OBJPATH);
		s_callback = GetDlgItem(DlgMainWnd, IDC_STATUS_CALLBACK);
		SetWindowTextA(s_callback, "");
		
		break;

	case WM_CLOSE:
		EndDialog(hDlg, Usr_Canceled);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, Usr_Canceled);
			break;
		case IDOK:
			check();
			break;
		case IDC_BROWSE:
			getOpen();
			break;
		}
		break;

	case WM_CTLCOLORSTATIC:
		hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		if ((HWND)lParam == s_callback) {
			SetTextColor(hdcStatic, RGB(255, 0, 0));
		}
		return (INT_PTR)hBrush;
		break;

	case WM_PAINT:
		break;
	}
	return (INT_PTR)0;
}

int check() {
	char strchec[3];
	GetWindowTextA(objNameStor, strchec, 3);
	if (strchec[0] == 0) {
		SetWindowTextA(s_callback, "对象名不能为空");
		return 1;
	}
	return 0;
}

void getOpen() {
	OPENFILENAME ofn{ 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = DlgMainWnd;
	ofn.lpstrFilter = L"所有文件(*.*)\0";
	ofn.lpstrInitialDir = L"C:\\";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 1024;
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	if (GetOpenFileName(&ofn)) {
		SetWindowTextW(objPathStor, filename);
	}
}
