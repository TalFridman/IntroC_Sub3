/********************************************/
/*   list.h                                 */
/*   a dynamic  linked list with a header   */
/********************************************/


#ifndef _LIST_
#define _LIST_

#include "def.h"


/*** Definitions ***/

// List
typedef struct
{
	NODE head;
}LIST;


/*** Function prototypes ***/

BOOL L_init(LIST* pList);					// create new list

NODE* L_insert(NODE* pNode, DATA Value);	// add new node after *pNode

BOOL L_delete(NODE* pNode, void (*freeFunction)(void*));					// erase node after *pNode

NODE* L_find(NODE* pNode, DATA Value, int (*compare)(void*, void*));		// return a pointer to the node 

BOOL L_free(LIST* pList, void (*freeFunction)(void*));					// free list memory

int L_print(const LIST* pList, void (*printFunction)(void*));					// print the list content

#endif
