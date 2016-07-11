/*
 * linked_list.c
 *
 *  Created on: 11 lip 2016
 *      Author: M
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "linked_list.h"


typedef struct _NODE
{
  void *Data;
  struct _NODE *Next;

} NODE;

typedef struct _LINKED_LIST
{
  NODE *Head;
  NODE *Tail;
  NODE *Current;
} LINKED_LIST;

typedef struct _NODE* NODE_PTR;
typedef struct _LINKED_LIST* LINKED_LIST_PTR;

void InitializeList(LINKED_LIST_PTR List)
{
 List->Head = NULL;
 List->Tail = NULL;
 List->Current = NULL;
}

//Kolejne elementy dodajemy zawsze na koncu kolejki
void AddItem(LINKED_LIST_PTR List, void* Data)
{
  NODE_PTR Item = (NODE_PTR)malloc(sizeof(NODE));
  if(NULL != Item)
  {
     Item->Data = Data;
	 Item->Next = NULL;

	 if(NULL == List->Head)
	 {
	    List->Head = Item;
	 }
	 else
	 {
		 List->Tail->Next = Item;
	 }

	 List->Tail = Item;
  }
  else
  {
	//<TODO:> exception handling
  }

}

NODE_PTR GetNextItem(LINKED_LIST_PTR List)
{


}

void RemoveItem(LINKED_LIST_PTR, NODE_PTR);
NODE_PTR SeekItem(LINKED_LIST_PTR, int Index);

