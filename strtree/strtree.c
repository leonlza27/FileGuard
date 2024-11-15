#include "strtree.h"

//struct ChrNode

ChrNode *initChrNode(){
  ChrNode *nodeNew = malloc(sizeof(ChrNode));
  memset(nodeNew->nodenext,0,16);
  nodeNew->end=1;
  return nodeNew;
}

void AddItem(ChrNode *node,unsigned char item){
  node->nodenext[item] = initChrNode();
  node->end=0;
}

int HaveItem(ChrNode *node,unsigned char item){
  return !(node->nodenext[item]==0);
}

void ClearAll(ChrNode *node){
  for(int i = 0;i<16;i++){
    if(node->nodenext[i] != 0) ClearAll(node->nodenext[i]);
  }
  free(node);
}

//struct StrTree

StrTree *initStrTree(){
  StrTree *TreeNew = malloc(sizeof(StrTree));
  TreeNew->head = initChrNode();
  TreeNew->cur = TreeNew->head;
  return TreeNew;
}

void AddString(StrTree *tree,const char* str){
  tree->cur = tree->head;
  for(size_t i =0;str[i]!=0;i++){
    char mid = (str[i]&0xF0)>>4;
    AddItem(tree->cur,mid);
    tree->cur = tree->cur->nodenext[mid];
    mid = str[i]&0xF;
    AddItem(tree->cur,mid);
    tree->cur = tree->cur->nodenext[mid];
  }
  tree->cur = tree->head;
}

int HaveStrOrSubStr(StrTree *tree,const char* str){
  tree->cur = tree->head;
  for(size_t i =0;str[i-1<0?0:i]!=0;i++){
    char mid = (str[i]&0xF0)>>4;
    if(!HaveItem(tree->cur,mid)) break;
    tree->cur = tree->cur->nodenext[mid];
    mid = str[i]&0xF;
    if(!HaveItem(tree->cur,mid)) break;
    tree->cur = tree->cur->nodenext[mid];
  }
  ChrNode *stor= tree->cur; 
  tree->cur = tree->head;
  return stor->end;
}

void freeStrTree(StrTree *tree){
  ClearAll(tree->head);
  free(tree);
}
