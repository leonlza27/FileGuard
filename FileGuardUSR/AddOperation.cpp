// AddOperation.cpp: 实现文件
//

#include "pch.h"
#include "FileGuardUSR.h"
#include "afxdialogex.h"
#include "AddOperation.h"


// AddOperation 对话框

IMPLEMENT_DYNAMIC(AddOperation, CDialogEx)

AddOperation::AddOperation(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDOP, pParent)
{
	isCanceled = 0;
}

AddOperation::~AddOperation()
{
}

void AddOperation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_OBJ_NAME, AddtionNameEnter);
	DDX_Control(pDX, IDC_ADD_OBJ_PATH, AddtionPathEnter);
}


BEGIN_MESSAGE_MAP(AddOperation, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &AddOperation::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &AddOperation::OnBnClickedOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// AddOperation 消息处理程序


int AddOperation::CheckInfo() {
	AddtionNameEnter.GetWindowTextW(ObjNameNew);

	if (ObjNameNew == L"") {
		::SetWindowTextW(GetDlgItem(IDC_SWINFO)->m_hWnd, L"对象名不能为空");
		MessageBeep(MB_ICONERROR);
		AddtionNameEnter.SetFocus();
		return 1;
	}

	AddtionPathEnter.GetWindowTextW(ObjPathNew);

	if (ObjPathNew == L"") {
		::SetWindowTextW(GetDlgItem(IDC_SWINFO)->m_hWnd, L"文件路径不能为空");
		MessageBeep(MB_ICONERROR);
		AddtionPathEnter.SetFocus();
		return 1;
	}

	if (_waccess(ObjPathNew.AllocSysString(), 0) == -1) {
		::SetWindowTextW(GetDlgItem(IDC_SWINFO)->m_hWnd, L"文件路径无效");
		MessageBeep(MB_ICONERROR);
		AddtionPathEnter.SetFocus();
		return 1;
	}
	

	return 0;
}

void AddOperation::OnBnClickedCancel() {
	// TODO: 在此添加控件通知处理程序代码
	isCanceled = 1;
	CDialogEx::OnCancel();
}


void AddOperation::OnClose() {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	isCanceled = 1;
	CDialogEx::OnClose();
}


BOOL AddOperation::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HICON icon;
	//加载图标
	icon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(icon, true);
	SetIcon(icon, false);

	::SetWindowTextW(GetDlgItem(IDC_SWINFO)->m_hWnd, L"");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void AddOperation::OnBnClickedOk() {
	// TODO: 在此添加控件通知处理程序代码

	if (CheckInfo()) return;
	CDialogEx::OnOK();
}


HBRUSH AddOperation::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性


	if (pWnd->GetDlgCtrlID() == IDC_SWINFO) {
		pDC->SetTextColor(RGB(255, 0, 0));
	}


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
