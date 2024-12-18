#pragma once


#include <ntddk.h>
#include <stdio.h>

/*
  char
  0000  0000
  ^     ^
  128   8
 */
typedef struct _ChrNode {
	struct _ChrNode *nodenext[16];
	int end;
} ChrNode;

typedef struct _StrTree {
	ChrNode *head;
	ChrNode *cur;
} StrTree;

//struct StrTree

void AddString(StrTree *tree, const char *str);

StrTree *initStrTree();

int HaveStrOrSubStr(StrTree *tree, const char *str);

void freeStrTree(StrTree *tree);

//struct ChrNode

ChrNode *initChrNode();

void AddItem(ChrNode *node, unsigned char item);

int HaveItem(ChrNode *node, unsigned char item);

void ClearAll(ChrNode *node);
