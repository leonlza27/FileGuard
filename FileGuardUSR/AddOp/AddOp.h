#pragma once

#include <Windows.h>
#include <commctrl.h>
#include"../resource.h"
#include<io.h>

//状态
enum RetStatus{Obj_Added = 0,Usr_Canceled};

//添加操作窗口消息列表
INT_PTR CALLBACK AddOpProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int check();

void getOpen();