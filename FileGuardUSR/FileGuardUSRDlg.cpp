
// FileGuardUSRDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FileGuardUSR.h"
#include "FileGuardUSRDlg.h"
#include "afxdialogex.h"

#include"AddOperation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//屏幕缩放率,用于调整主窗口大小
double screenscale;
//列表选中项(全局)
int IndexGlobal;

// CFileGuardUSRDlg 对话框



CFileGuardUSRDlg::CFileGuardUSRDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEGUARDUSR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileGuardUSRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILELIST, filelist);
}

BEGIN_MESSAGE_MAP(CFileGuardUSRDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_GETMINMAXINFO()
	ON_WM_CONTEXTMENU()
	//添加指令
	ON_COMMAND(ID_ADD, AddObject)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_FILELIST, &CFileGuardUSRDlg::OnColumnclickFilelist)
	ON_NOTIFY(NM_RCLICK, IDC_FILELIST, &CFileGuardUSRDlg::OnRclickFilelist)
END_MESSAGE_MAP()


// CFileGuardUSRDlg 消息处理程序

BOOL CFileGuardUSRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//对象列表初始化
	filelist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	filelist.InsertColumn(0, L"对象名称" ,0,200);
	filelist.InsertColumn(1, L"对象路径",0,500);
	filelist.InsertColumn(2, L"防护状态",0,100);

	//主菜单设置

	CMenu* MainMenu = new CMenu();
	CMenu* menuOp = new CMenu();
	MainMenu->LoadMenuW(IDR_MENUMAIN);
	menuOp = MainMenu->GetSubMenu(0);
	menuOp->EnableMenuItem(ID_DEL, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	SetMenu(MainMenu);

	//获取缩放率

	// 获取窗口当前显示的监视器
	HWND hWnd = ::GetDesktopWindow();//根据需要可以替换成自己程序的句柄 
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	// 获取监视器逻辑宽度与高度
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);

	// 获取监视器物理宽度与高度
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;

	//缩放比例计算
	screenscale = ((double)cxPhysical / (double)cxLogical);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileGuardUSRDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileGuardUSRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileGuardUSRDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	lpMMI->ptMinTrackSize.x = (long)(800 * screenscale);
	lpMMI->ptMinTrackSize.y = (long)(600 * screenscale);

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CFileGuardUSRDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point/*point*/) {
	// TODO: 在此处添加消息处理程序代码

	CMenu menu;
	menu.LoadMenuW(IDR_MENURBTN);
	CMenu* pMenu;
	pMenu = menu.GetSubMenu(0);

	pMenu->EnableMenuItem(ID_ADD, MF_BYCOMMAND | MF_ENABLED);
	if (IndexGlobal == -1) {
		pMenu->EnableMenuItem(ID_DEL, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		pMenu->EnableMenuItem(ID_ITEM_ENABLE_OP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	else {
		pMenu->EnableMenuItem(ID_DEL, MF_BYCOMMAND | MF_ENABLED);
		if (filelist.GetItemText(IndexGlobal, 2) == L"已启用") {
			pMenu->ModifyMenuW(ID_ITEM_ENABLE_OP, MF_BYCOMMAND, ID_ITEM_ENABLE_OP, L"关闭防护");
		}
		pMenu->EnableMenuItem(ID_ITEM_ENABLE_OP, MF_BYCOMMAND | MF_ENABLED);	
	}
	pMenu->EnableMenuItem(ID_REFRESH_DRV_TGTREE, MF_BYCOMMAND | MF_ENABLED);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	pMenu->Detach();
	menu.DestroyMenu();

}

int CFileGuardUSRDlg::LstSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	CString& lp1 = *((CString*)lParam1);
	CString& lp2 = *((CString*)lParam2);
	int& sort = *(int*)lParamSort;
	if (sort == 0) {
		return lp1.CompareNoCase(lp2);
	}
	else {
		return lp2.CompareNoCase(lp1);
	}
}

void CFileGuardUSRDlg::AddObject() {
	AddOperation opWindow;
	opWindow.DoModal();
	if (!opWindow.isCanceled) {
		filelist.InsertItem(0, opWindow.ObjNameNew);
		filelist.SetItemText(0, 1, opWindow.ObjPathNew);
		filelist.SetItemText(0, 2, L"已启用");
		UpdateWindow();
	}
}


void CFileGuardUSRDlg::OnOK() {
	// TODO: 在此添加专用代码和/或调用基类
	return;
}


void CFileGuardUSRDlg::OnColumnclickFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	static int sorttype = 0;
	int Length = filelist.GetItemCount();
	CArray<CString, CString> ItemData;

	ItemData.SetSize(Length);

	for (int i = 0; i < Length; i++) {
		ItemData[i] = filelist.GetItemText(i, pNMLV->iSubItem);
		filelist.SetItemData(i, (DWORD_PTR)&ItemData[i]);//设置排序关键字
	}
	static int SubItem = 0;
	if (SubItem != pNMLV->iSubItem) {
		sorttype = 0;
		SubItem = pNMLV->iSubItem;
	}
	else {
		if (sorttype == 0) {
			sorttype = 1;
		}
		else {
			sorttype = 0;
		}
	}

	filelist.SortItems(LstSortProc, (DWORD_PTR) & sorttype);

	*pResult = 0;
}


void CFileGuardUSRDlg::OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	IndexGlobal = pNMItemActivate->iItem;
	
	*pResult = 0;
}
