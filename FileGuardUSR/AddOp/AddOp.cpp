#include "AddOp.h"

static HWND DlgMainWnd;/*对话框主窗口(句柄)*/
static HINSTANCE DlgHins;/*对话框实例*/

wchar_t filename[1024],objname[1024];
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
			if(check()==0) EndDialog(hDlg,Obj_Added);
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
	GetWindowTextW(objPathStor, filename, 1024);
	if (_waccess(filename, 0) == -1) {
		SetWindowTextA(s_callback, "文件(夹)路径无效");
		SetFocus(objPathStor);
		return 1;
	}
	GetWindowTextW(objNameStor, objname, 1024);
	return 0;
}

void getOpen() {
	OPENFILENAME ofn{ 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = DlgMainWnd;
	ofn.lpstrFilter = L"所有文件(*.*)\0";
	ofn.lpstrInitialDir = L"C:";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 1024;
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	if (GetOpenFileNameW(&ofn)) {
		SetWindowTextW(objPathStor, filename);
	}
}
