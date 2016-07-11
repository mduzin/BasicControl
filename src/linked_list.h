/*
 * linked_list.h
 *
 *  Created on: 11 lip 2016
 *      Author: M
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

typedef struct _NODE* NODE_PTR;
typedef struct LINKED_LIST* LINKED_LIST_PTR;

void InitializeList(LINKED_LIST_PTR);
void AddItem(LINKED_LIST_PTR, void*);
void RemoveItem(LINKED_LIST_PTR, NODE_PTR);
NODE_PTR GetNextItem(LINKED_LIST_PTR);
NODE_PTR SeekItem(LINKED_LIST_PTR, int Index);

#endif /* LINKED_LIST_H_ */
