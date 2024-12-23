#include "strtree.h"

//struct ChrNode

ChrNode* initChrNode() {
    //ChrNode *nodeNew = malloc(sizeof(ChrNode));
    ChrNode* nodeNew = ExAllocatePool(PagedPool, sizeof(ChrNode));
    for (int i = 0; i < 16; i++) {
        nodeNew->nodenext[i] = 0;
    }
    nodeNew->end = 1;
    return nodeNew;
}

void AddItem(ChrNode* node, unsigned char item) {
    node->nodenext[item] = initChrNode();
    node->end = 0;
}

int HaveItem(ChrNode* node, unsigned char item) {
    return node->nodenext[item] == 0 ? 0 : 1;
}

void ClearAll(ChrNode* node) {
    for (int i = 0; i < 16; i++) {
        if (node->nodenext[i] != 0) ClearAll(node->nodenext[i]);
    }
    ExFreePool(node);
}

//struct StrTree

StrTree* initStrTree() {
    StrTree* TreeNew = ExAllocatePool(PagedPool, sizeof(ChrNode));
    TreeNew->head = initChrNode();
    return TreeNew;
}

void AddString(StrTree* tree, const char* str) {
    ChrNode *tmp = tree->head;
    for (size_t i = 0; str[i] != 0; i++) {
        char mid = (str[i] & 0xF0) >> 4;
        if (tmp->nodenext[mid] == 0)
            AddItem(tmp, mid);
        tmp = tmp->nodenext[mid];
        mid = str[i] & 0xF;
        if (tmp->nodenext[mid] == 0)
            AddItem(tmp, mid);
        tmp = tmp->nodenext[mid];
    }
}

int HaveStrOrSubStr(StrTree* tree, const char* str) {
    ChrNode* tmp = tree->head;
    for (size_t i = 0; str[i] != 0 && str[0] != 0; i++) {
        char mid = (str[i] & 0xF0) >> 4;
        if (!HaveItem(tmp, mid)) break;
        tmp = tmp->nodenext[mid];
        mid = str[i] & 0xF;
        if (!HaveItem(tmp, mid)) break;
        tmp = tmp->nodenext[mid];
    }
    return tmp->end;
}

void freeStrTree(StrTree* tree) {
    ClearAll(tree->head);
    ExFreePool(tree);
}
