#include <iostream>
#include"strtree.h"

const char *Testbuf[] = {"C:\\Users\\leonl\\Desktop\\glog",
						 "C:\\Users\\Public\\Desktop\\Cpolar Web UI.lnk",
						 "D:\\Everything",
						 "D:\\Games\\Kerbal Space Program\\Plugins",
						 "C:\\Program Files (x86)\\Windows Kits\\10\\Include",
						 "D:\\Git\\cmd"};

const char *SccanLst[] = {"C:\\Users\\leonl\\Desktop\\glog",						//1
						  "C:\\Users\\Public\\Desktop\\Cpolar Web UI.lnk",			//1
						  "C:\\Users\\leonl\\Desktop\\glog\\1",						//1 extend .../1
						  "D:\\Everything",											//1
						  "D:\\Games\\Kerbal Space Program\\Plugins",				//1
						  "D:\\Games\\Kerbal Space Program",						//0
						  "C:\\Program Files (x86)\\Windows Kits\\10\\Include",		//1
						  "C:\\Program Files (x86)\\Microsoft\\Edge\\Application",	//0
						  "C:\\Users\\Public\\Desktop\\嘉立创EDA(专业版).lnk",		//0
						  "D:\\Git\\cmd",											//1
						  "D:\\"};													//0


int main() {
	StrTree *tree = initStrTree();
	for (int i = 0; i < 6; i++)
		AddString(tree, Testbuf[i]);

	for(int i =0;i<11;i++){
		printf("%s -> %d\n",SccanLst[i], HaveStrOrSubStr(tree,SccanLst[i]));
	}
	return 0;
}
