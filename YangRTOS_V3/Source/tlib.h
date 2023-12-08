#ifndef __TILB_H
#define __TILB_H



#include <stdint.h>




typedef struct Bitmap{

	uint32_t bitmap;


}tBitmap;


typedef struct _tNode{
	struct _tNode *preNode;
	struct _tNode *nextNode;
	
}tNode;


typedef struct _tList{
	tNode headNode;
	uint32_t nodeCount;
	
}tList;


#define tNodeParent(node, parent, name)   (parent*)((uint32_t)node - (uint32_t)&((parent*)0)->name)         






void tBitmapInit(tBitmap* bitmap);
void tBitmapSet(tBitmap* bitmap,uint32_t pos);
void tBitmapClear(tBitmap* bitmap,uint32_t pos);
uint32_t tBitmapGetFirstSet(tBitmap* bitmap);
uint32_t tBitmapCount(void);

void tNodeInit(tNode* node);
void tListInit(tList* list);
uint32_t tListCount(tList* list);
tNode* tListFirst(tList* list);
tNode* tListLast(tList* list);
tNode* tListPre(tList* list, tNode* node);
tNode* tListNext(tList* list, tNode* node);
void tListRemoveAll(tList *list);
void tListAddFirst(tList* list, tNode* node);
void tListAddLast(tList* list, tNode* node);
tNode* tListRemoveFirst(tList* list);
void tListInsertAfter(tList* list, tNode* nodeAfter, tNode* nodeToInsert);
void tListRemove(tList* list, tNode* node);


#endif