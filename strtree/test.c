#include<stdio.h>
#include"strtree.h"

int main(){
  StrTree *node = initStrTree();
  printf("str\'a\'added\n");
  AddString(node,"a");
  printf("try to find \'b\':%d\n",HaveStrOrSubStr(node,"b"));
  printf("try to find \'a\':%d\n",HaveStrOrSubStr(node,"a"));
  printf("try to find \'ab\':%d\n",HaveStrOrSubStr(node,"ab"));

  printf("str\'泥嚎\'added\n");
  AddString(node,"泥嚎");
  printf("try to find \'你\':%d\n",HaveStrOrSubStr(node,"你"));
  printf("try to find \'泥嚎\':%d\n",HaveStrOrSubStr(node,"泥嚎"));
  printf("try to find \'泥\':%d\n",HaveStrOrSubStr(node,"泥"));
  printf("try to find \'泥好\':%d\n",HaveStrOrSubStr(node,"泥好"));
  freeStrTree(node);
  return 0;
}
