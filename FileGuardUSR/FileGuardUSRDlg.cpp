
// FileGuardUSRDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FileGuardUSR.h"
#include "FileGuardUSRDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

	filelist.SetExtendedStyle(filelist.GetExStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	filelist.InsertColumn(0, L"对象名称" ,0,200);
	filelist.InsertColumn(1, L"对象路径",0,500);
	filelist.InsertColumn(2, L"状态",0,100);

	CMenu* MainMenu = new CMenu();
	CMenu* menuOp = new CMenu();
	MainMenu->LoadMenuW(IDR_MENUMAIN);
	menuOp = MainMenu->GetSubMenu(0);
	menuOp->EnableMenuItem(ID_DEL, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	SetMenu(MainMenu);

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

	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CFileGuardUSRDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point/*point*/) {
	// TODO: 在此处添加消息处理程序代码

	CMenu menu;
	menu.LoadMenu(IDR_MENURBTN);
	CMenu* pMenu;
	pMenu = menu.GetSubMenu(0);

	pMenu->EnableMenuItem(ID_ADD_RBTN, MF_BYCOMMAND | MF_ENABLED);
	pMenu->EnableMenuItem(ID_DEL_RBTN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	pMenu->EnableMenuItem(ID_REFRESH_DRV_TGTREE_RBTN, MF_BYCOMMAND | MF_ENABLED);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	pMenu->Detach();
	menu.DestroyMenu();

}
