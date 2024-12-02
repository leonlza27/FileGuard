#pragma once

//SetMenu(hDlg, LoadMenu(GetModuleHandleW(0), MAKEINTRESOURCEW(IDC_PROJECTTEST)));

#include <Windows.h>
#include <commctrl.h>
#include"../resource.h"

//主窗口消息列表
INT_PTR CALLBACK MainWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//响应窗口尺寸改变
void ReSize();

//初始化
void InitDlg();