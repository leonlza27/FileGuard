#pragma once

//SetMenu(hDlg, LoadMenu(GetModuleHandleW(0), MAKEINTRESOURCEW(IDC_PROJECTTEST)));

#include <Windows.h>
#include <commctrl.h>
#include"../resource.h"
#include"../AddOp/AddOp.h"

//��������Ϣ�б�
INT_PTR CALLBACK MainWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//��Ӧ���ڳߴ�ı�
void ReSize();

//��ʼ��
void InitDlg();

//�Ҽ��˵�
void RbtnMenu(WPARAM wParam, LPARAM lParam);

//�б�����
int CALLBACK SortItemFunc(LPARAM lParam1, LPARAM lParam2, LPARAM sortID);
void OnColumnClick(LPNMLISTVIEW pLVInfo);