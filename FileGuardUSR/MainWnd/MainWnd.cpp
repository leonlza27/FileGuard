#include "MainWnd.h"
#include "../DBG.h"

//�����б�(��ͷ:������/·��/״̬)
HWND filelst;
//�Ի���������(���)
HWND DlgMainWnd;
//��Ļ���ű�
double ScreenScale;


INT_PTR MainWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		DlgMainWnd = hDlg;
		InitDlg();
		break;

	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		return 0;
		break;

	case WM_SIZE:
		ReSize();
		break;

	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case NM_RCLICK:
			NUL_CASE_BRANCH_RESPONCE
			break;
		}
		break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = (long)(600 * ScreenScale);
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = (long)(400 * ScreenScale);
		break;
	}
	return 0;
}

void InitDlg() {
	//����ͼ��
	HICON ico = LoadIcon(GetModuleHandleA(0), MAKEINTRESOURCEW(IDI_ICON1));
	SendMessageW(DlgMainWnd, WM_SETICON, ICON_SMALL, (LPARAM)ico);

	//�󶨿ؼ�
	filelst = GetDlgItem(DlgMainWnd, IDC_FILELIST);

	//�б��ʼ��

	SendMessage(filelst, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, ListView_GetExtendedListViewStyle(filelst) | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	LVCOLUMN lvCol = { 0 };
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.iSubItem = 0;
	lvCol.cx = 100;
	lvCol.pszText = (LPWSTR)L"������";
	SendMessage(filelst, LVM_INSERTCOLUMN, 0, (LPARAM)&lvCol);

	lvCol.iSubItem = 1;
	lvCol.cx = 400;
	lvCol.pszText = (LPWSTR)L"����·��";
	SendMessage(filelst, LVM_INSERTCOLUMN, 1, (LPARAM)&lvCol);

	lvCol.iSubItem = 2;
	lvCol.cx = 100;
	lvCol.pszText = (LPWSTR)L"����״̬";
	SendMessage(filelst, LVM_INSERTCOLUMN, 2, (LPARAM)&lvCol);

	//��ȡ���ű�

	HWND hWnd = GetDesktopWindow();
	HDC hDC = GetDC(hWnd);

	// ��ȡ��ʾ�������ű���
	ScreenScale = (double)GetDeviceCaps(hDC, LOGPIXELSX) / 96.0f;

	ReleaseDC(hWnd, hDC);
}


void ReSize() {
	RECT rect;
	GetWindowRect(DlgMainWnd, &rect);
	MoveWindow(filelst, 0, 0, rect.right-rect.left, rect.bottom-rect.top, 1);
}

