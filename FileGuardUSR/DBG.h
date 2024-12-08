#pragma once

#include<Windows.h>

#ifdef _DEBUG
#define NUL_CASE_BRANCH_RESPONCE MessageBeep(MB_ICONWARNING);
#define CUT_NPASS {if(GetLastError() != 0){ DWORD ret = GetLastError();char *ErrAccessed = 0;*ErrAccessed = 1;}}
#else
#define NUL_CASE_BRANCH_RESPONCE
#define CUT_NPASS
#endif