#pragma once
#include "afxdialogex.h"


// AddOperation 对话框

class AddOperation : public CDialogEx
{
	DECLARE_DYNAMIC(AddOperation)

public:
	AddOperation(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AddOperation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDOP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit AddtionNameEnter;
	CMFCEditBrowseCtrl AddtionPathEnter;

	int isCanceled;
	CString ObjNameNew;
	CString ObjPathNew;

	int CheckInfo();

	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
