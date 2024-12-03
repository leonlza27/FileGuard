#include "AddOp.h"

static HWND DlgMainWnd;/*对话框主窗口(句柄)*/
static HINSTANCE DlgHins;/*对话框实例*/

INT_PTR AddOpProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		DlgMainWnd = hDlg;
		DlgHins = GetModuleHandleW(0);
		break;

	case WM_CLOSE:
		EndDialog(hDlg, Usr_Canceled);
		return (INT_PTR)0;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, Usr_Canceled);
			break;
		}
		break;
	}
	return (INT_PTR)0;
}
