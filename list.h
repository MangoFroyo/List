#ifndef _LIST_H_
#define _LIST_H_

#include <stddef.h>

typedef struct _LISTNODE
{
	/* data */
	void *data;

	/* list */
	struct _LISTNODE *prev;
	struct _LISTNODE *next;
} LISTNODE;

typedef struct _LIST
{
	LISTNODE *head;
	LISTNODE *tail;
	size_t size;
} LIST;

LIST *list_create(void);
int list_destory(LIST *list);

int list_addTail(LIST *list, void *data, size_t data_size);
int list_addHead(LIST *list, void *data, size_t data_size);
int list_addIndex(LIST *list, int index, void *data, size_t data_size);

int list_delTail(LIST *list);
int list_delHead(LIST *list);
int list_delIndex(LIST *list, int index);

int list_getSize(LIST *list);
void *list_getData(LIST *list, int index);

int list_isEmpty(LIST *list);


#endif /* _LIST_H_ */ 