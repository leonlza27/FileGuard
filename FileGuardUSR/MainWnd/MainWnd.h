#pragma once

//SetMenu(hDlg, LoadMenu(GetModuleHandleW(0), MAKEINTRESOURCEW(IDC_PROJECTTEST)));

#include <Windows.h>
#include <commctrl.h>
#include"../resource.h"

//��������Ϣ�б�
INT_PTR CALLBACK MainWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//��Ӧ���ڳߴ�ı�
void ReSize();

//��ʼ��
void InitDlg();