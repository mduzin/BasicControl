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
  void *data;
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

