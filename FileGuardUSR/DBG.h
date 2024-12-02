#pragma once

#ifdef _DEBUG
#define NUL_CASE_BRANCH_RESPONCE MessageBeep(MB_ICONWARNING);
#else
#define NUL_CASE_BRANCH_RESPONCE
#endif