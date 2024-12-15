#include "ConfGenerate.h"

void GenerateConfigFile(HWND lst) {
	FILE* ConfFileNew = fopen(CONFPATH, "wb");
	FileHeader headerInfo{};
	FILEHEADER_SETUP(&headerInfo);
	headerInfo.ItemCount = ListView_GetItemCount(lst);
	fwrite(&headerInfo, sizeof(FileHeader), 1, ConfFileNew);
	TgItemInfo infomid;
	for (UINT i = 0; i < headerInfo.ItemCount; i++) {
		ListView_GetItemText(lst, i, 2, infomid.ObjName, 1024);
		infomid.IsSettedProtect = (wcscmp(infomid.ObjName, L"已启用") == 0);
		ListView_GetItemText(lst, i, 1, infomid.ObjName, 1024);
		ListView_GetItemText(lst, i, 0, infomid.Path, 1024);
		fwrite(&infomid, sizeof(TgItemInfo), 1, ConfFileNew);
	}
	fclose(ConfFileNew);
}

void LoadConfigFile(HWND lst) {
	FILE* ConfFile = fopen(CONFPATH, "rb");
	if (ConfFile == 0) return;
	FileHeader OpenedHeader;
	fread(&OpenedHeader, sizeof(FileHeader), 1, ConfFile);
	if (!FILEHEADER_CONFIRM(OpenedHeader)) {
		fclose(ConfFile);
		return;
	}

	TgItemInfo ItemAdd;

	LVITEM itemNew;
	itemNew.mask = LVIF_TEXT;
	
	for (UINT i = 0; i < OpenedHeader.ItemCount; i++) {
		fread(&ItemAdd, sizeof(TgItemInfo), 1, ConfFile);
		itemNew.iItem = i;
		itemNew.iSubItem = 0;
		itemNew.pszText = ItemAdd.Path;
		ListView_InsertItem(lst, &itemNew);
		itemNew.iSubItem = 1;
		itemNew.pszText = ItemAdd.ObjName;
		ListView_SetItem(lst, &itemNew);
		itemNew.iSubItem = 2;
		itemNew.pszText = (LPWSTR)(ItemAdd.IsSettedProtect ? L"已启用" : L"未启用");
		ListView_SetItem(lst, &itemNew);
	}

	fclose(ConfFile);
}