#pragma once

#include <Windows.h>
#include <commctrl.h>
#include"../resource.h"

//״̬
enum RetStatus{Obj_Added = 0,Usr_Canceled};

//���Ӳ���������Ϣ�б�
INT_PTR CALLBACK AddOpProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);