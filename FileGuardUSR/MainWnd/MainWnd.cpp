#include "MainWnd.h"
#include "../DBG.h"


static HWND filelst;/*对象列表(表头:对象名 / 路径 / 状态)*/
static HWND DlgMainWnd;/*对话框主窗口(句柄)*/
static HINSTANCE DlgHins;/*对话框实例*/

static double ScreenScale;/*屏幕缩放比*/
static int LstIndex;
static HMENU FileOperation;/*主菜单文件选项,需中途修改*/

extern wchar_t filename[1024], objname[1024];



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
			if (((LPNMHDR)lParam)->hwndFrom == filelst && (LstIndex = ((LPNMITEMACTIVATE)(LPNMHDR)lParam)->iItem) == -1) {
				EnableMenuItem(FileOperation, ID_DEL, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			}
			else {
				EnableMenuItem(FileOperation, ID_DEL, MF_BYCOMMAND | MF_ENABLED);
			}
			break;

		case LVN_COLUMNCLICK:
			OnColumnClick((LPNMLISTVIEW)lParam);
			break;

		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_ADD:
			if (DialogBox(DlgHins, MAKEINTRESOURCE(IDD_ADD), DlgMainWnd, AddOpProc) == Obj_Added) {
				LVITEM itemNew;
				itemNew.mask = LVIF_TEXT;
				itemNew.iItem = 0;
				itemNew.iSubItem = 0;
				itemNew.pszText = objname;
				ListView_InsertItem(filelst, &itemNew);
				itemNew.iSubItem = 1;
				itemNew.pszText = filename;
				ListView_SetItem(filelst, &itemNew);
				itemNew.iSubItem = 2;
				itemNew.pszText = (LPWSTR)L"已启用";
				ListView_SetItem(filelst, &itemNew);
			}
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

	TrackPopupMenu(OpMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, 0, DlgMainWnd, NULL);

	DestroyMenu(RootRbtnMenu);
}

int SortItemFunc(LPARAM lParam1, LPARAM lParam2, LPARAM sortID) {
	BOOL ascending = *(BOOL*)(sortID);
	int res = wcscmp((const wchar_t*)lParam1, (const wchar_t*)lParam2);
	return ascending ? res >= 0 : res <= 0;
}

void ReSize() {
	RECT rect;
	GetWindowRect(DlgMainWnd, &rect);
	MoveWindow(filelst, 0, 0, rect.right-rect.left, rect.bottom-rect.top, 1);
}

void OnColumnClick(LPNMLISTVIEW pLVInfo) {
	static int column = 0;
	static BOOL ascending = TRUE;
	if (pLVInfo->iSubItem != column)
		ascending = TRUE;
	else
		ascending = !ascending;
	column = pLVInfo->iSubItem;

	HWND hlist = pLVInfo->hdr.hwndFrom;
	int count = ListView_GetItemCount(hlist);
	if (count < 1) return;

	//allocate strings
	wchar_t** arr = (wchar_t**)malloc(count * sizeof(wchar_t*));
	LVITEM lvi = { 0 };
	lvi.mask = LVIF_PARAM;
	for (int i = 0; i < count; i++)
	{
		//get column string
		wchar_t buf[100]; //random max buffer size, hopefully it's big enough
		ListView_GetItemText(hlist, i, column, buf, _countof(buf));
		arr[i] = _wcsdup(buf);

		//match lParam to the string
		lvi.lParam = (LPARAM)arr[i];
		lvi.iItem = i;
		ListView_SetItem(hlist, &lvi);
	}

	ListView_SortItems(hlist, SortItemFunc, (LPARAM)&ascending);

	//cleanup
	for (int i = 0; i < count; i++)
		free(arr[i]);
	free(arr);
}