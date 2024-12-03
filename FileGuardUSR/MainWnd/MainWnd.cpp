#include "MainWnd.h"
#include "../DBG.h"


static HWND filelst;/*对象列表(表头:对象名 / 路径 / 状态)*/
static HWND DlgMainWnd;/*对话框主窗口(句柄)*/
static HINSTANCE DlgHins;/*对话框实例*/

static double ScreenScale;/*屏幕缩放比*/
static int LstIndex;
static HMENU FileOperation;/*主菜单文件选项,需中途修改*/


INT_PTR MainWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		DlgMainWnd = hDlg;
		DlgHins = GetModuleHandleW(0);
		InitDlg();
		break;

	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)0;
		break;

	case WM_SIZE:
		ReSize();
		break;

	case WM_CONTEXTMENU:
		RbtnMenu(wParam, lParam);
		break;

	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case NM_RCLICK:
			if (((LPNMHDR)lParam)->hwndFrom == filelst) {
				LstIndex = ((LPNMITEMACTIVATE)(LPNMHDR)lParam)->iItem;
			}
			break;
		case NM_CLICK:
			if (((LPNMHDR)lParam)->hwndFrom == filelst && ((LPNMITEMACTIVATE)(LPNMHDR)lParam)->iItem == -1) {
				EnableMenuItem(FileOperation, ID_DEL, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			}
			else {
				EnableMenuItem(FileOperation, ID_DEL, MF_BYCOMMAND | MF_ENABLED);
			}
			break;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_ADD:
			DialogBox(DlgHins, MAKEINTRESOURCE(IDD_ADD), DlgMainWnd, AddOpProc);
			break;
		}
		break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = (long)(600 * ScreenScale);
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = (long)(400 * ScreenScale);
		break;
	}
	return (INT_PTR)0;
}

void InitDlg() {
	//设置图标
	HICON ico = LoadIconW(DlgHins, MAKEINTRESOURCEW(IDI_ICON1));
	SendMessageW(DlgMainWnd, WM_SETICON, ICON_SMALL, (LPARAM)ico);

	//主菜单初始化
	HMENU RootMainMenu = LoadMenuW(DlgHins, MAKEINTRESOURCE(IDR_MENU_MAIN));
	FileOperation = GetSubMenu(RootMainMenu, 0);
	EnableMenuItem(FileOperation, ID_DEL, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	SetMenu(DlgMainWnd, RootMainMenu);

	//绑定控件
	filelst = GetDlgItem(DlgMainWnd, IDC_FILELIST);

	//列表初始化
	SendMessage(filelst, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, ListView_GetExtendedListViewStyle(filelst) | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	LVCOLUMN lvCol = { 0 };
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.iSubItem = 0;
	lvCol.cx = 100;
	lvCol.pszText = (LPWSTR)L"对象名";
	SendMessage(filelst, LVM_INSERTCOLUMN, 0, (LPARAM)&lvCol);

	lvCol.iSubItem = 1;
	lvCol.cx = 400;
	lvCol.pszText = (LPWSTR)L"对象路径";
	SendMessage(filelst, LVM_INSERTCOLUMN, 1, (LPARAM)&lvCol);

	lvCol.iSubItem = 2;
	lvCol.cx = 100;
	lvCol.pszText = (LPWSTR)L"防护状态";
	SendMessage(filelst, LVM_INSERTCOLUMN, 2, (LPARAM)&lvCol);

	//获取缩放比

	HWND hWnd = GetDesktopWindow();
	HDC hDC = GetDC(hWnd);

	// 获取显示器的缩放比例
	ScreenScale = (double)GetDeviceCaps(hDC, LOGPIXELSX) / 96.0f;

	ReleaseDC(hWnd, hDC);
}

void RbtnMenu(WPARAM wParam, LPARAM lParam) {
	HMENU RootRbtnMenu = LoadMenuW(DlgHins, MAKEINTRESOURCE(IDR_MENU_RBTN));
	HMENU OpMenu = GetSubMenu(RootRbtnMenu, 0);

	POINT pnt;
	GetCursorPos(&pnt);

	//filelst
	if (LstIndex == -1) EnableMenuItem(OpMenu, ID_DEL, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	TrackPopupMenu(OpMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, 0, (HWND)wParam, NULL);

	DestroyMenu(RootRbtnMenu);
}

void ReSize() {
	RECT rect;
	GetWindowRect(DlgMainWnd, &rect);
	MoveWindow(filelst, 0, 0, rect.right-rect.left, rect.bottom-rect.top, 1);
}

