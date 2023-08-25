#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "list.h"

typedef struct
{
	char usr[20];
	unsigned char type;
	size_t size;
} DATA;

LISTNODE *createNode(void *data, size_t data_size);
int deleteNode(LISTNODE *node);


LISTNODE *createNode(void *data, size_t data_size)
{
	LISTNODE *phead = (LISTNODE *)malloc(sizeof(LISTNODE));
	if ( NULL == phead )
	{
		return NULL;
	}
	else
	{
		phead->data = (void *)malloc(data_size);
		if ( NULL == phead->data )
		{
			return NULL;
		}
		else
		{
			if ( NULL == data )
			{
				/* do nothing */
			}
			else
			{
				memmove(phead->data, data, data_size);
			}
			phead->next = NULL;
			phead->prev = NULL;
		}
	}
	return phead;
}

int deleteNode(LISTNODE *node)
{	
	if ( NULL == node )
	{
		return -1;
	}
	else
	{
		if ( NULL == node->data )
		{
			/* do nothing */
		}
		else
		{
			free(node->data);
		}
		free(node);
	}
	return 0;
}

LIST *list_create(void)
{
	LIST *list = (LIST *)malloc(sizeof(LIST));
	if ( NULL == list )
	{
		return NULL;
	}
	else
	{
		memset(list, 0, sizeof(LIST));
	}
	return list;
}

int list_destory(LIST *list)
{
	if ( NULL == list )
	{
		return -1;
	}
	else
	{
		int i = 0;
		size_t list_size = list->size;
		for ( i = 0; i < list_size; ++i )
		{
			list_delTail(list);
		}
		free(list);
		list = NULL;
	}
	return 0;
}

/* 向链表尾加入节点 */
int list_addTail(LIST *list, void *data, size_t data_size)
{
	if ( NULL == list )
	{
		return -1;
	}
	else
	{
		LISTNODE *node = createNode(data, data_size);
		if ( NULL == list->tail )
		{
			/* 若链表头为空*/
			list->head = node;
			list->tail = node;
		}
		else
		{
			/* 添加至链表尾 */
			list->tail->next = node;
			node->prev = list->tail;

			list->tail = node;
		}
		list->size++;
	}
	return 0;
}

/* 向链表头加入节点 */
int list_addHead(LIST *list, void *data, size_t data_size)
{
	if ( NULL == list )
	{
		return -1;
	}
	else
	{
		LISTNODE *node = createNode(data, data_size);
		if ( NULL == list->head )
		{
			/* 若链表头为空*/
			list->head = node;
			list->tail = node;
		}
		else
		{
			/* 添加至链表头，并更新head */
			list->head->prev = node;
			node->next = list->head;

			list->head = node;
		}
		list->size++;
	}
	return 0;
}

 /* 向链表中指定位置加入节点 */
int list_addIndex(LIST *list, int index, void *data, size_t data_size)
{
	if ( NULL == list )
	{
		return -1;
	}
	else
	{
		if ( index < 0 || index > list->size )
		{
			return -1;
		}
		else if( 0 == index )
		{
			list_addHead(list, data, data_size);
		}
		else if( index == list->size)
		{
			list_addTail(list, data, data_size);
		}
		else
		{
			LISTNODE *pnode = NULL;
			/* 根据index大小判断从哪边开始更快，最终到达index-1的位置进行插入操作 */
			if ( index <= list->size / 2 )
			{
				/* 从头部开始遍历 */
				pnode = list->head;
				int i = 0;
				for ( i = 0;i < index - 1; ++i )
				{
					pnode = pnode->next;
				}
			}
			else
			{
				/* 从尾部开始遍历 */
				pnode = list->tail;
				int i = 0;
				for ( i = 0;i < list->size - index; ++i )
				{
					pnode = pnode->prev;
				}
			}

			LISTNODE *node_new = createNode(data, data_size);
			
			pnode->next->prev = node_new;
			node_new->next = pnode->next;
			pnode->next = node_new;
			node_new->prev = pnode;

			list->size++;
		}
	}
	return 0;
}

int list_delTail(LIST *list)
{
	if ( NULL == list )
	{
		return -1;
	}
	else
	{
		if ( NULL == list->head || NULL == list->tail )
		{
			/* do nothing */
		}
		else if ( list->head == list->tail )
		{
			/* 当size == 1 即头尾相同时，直接删掉该节点 */
			deleteNode(list->tail);
			list->head = list->tail = NULL;
			list->size--;
		}
		else
		{
			LISTNODE *pnode = list->tail->prev;
			pnode->next = NULL;

			deleteNode(list->tail);
			list->tail = pnode;

			list->size--;
		}
	}
	return 0;
}

int list_delHead(LIST *list)
{
	if ( NULL == list )
	{
		return -1;
	}
	else
	{
		if ( NULL == list->head || NULL == list->tail )
		{
			/* do nothing */
		}
		else if ( list->head == list->tail )
		{
			/* 当size == 1 即头尾相同时，直接删掉该节点 */
			deleteNode(list->tail);
			list->head = list->tail = NULL;
			list->size--;
		}
		else
		{
			LISTNODE *pnode = list->head->next;
			pnode->prev = NULL;

			deleteNode(list->head);
			list->head = pnode;

			list->size--;
		}
	}
	return 0;
}

int list_delIndex(LIST *list, int index)
{
	if ( NULL == list )
	{
		return -1;
	}
	else
	{
		if( index < 0 || index >= list->size )
		{
			return -1;
		}
		else if( index == 0 )
		{
			list_delHead(list);
		}
		else if( index == list->size - 1 )
		{
			list_delTail(list);
		}
		else
		{
			LISTNODE *pnode = NULL;
			/* 根据index大小判断从哪边开始更快，最终到达index的位置进行删除操作 */
			if ( index <= list->size / 2 )
			{
				/* 从头部开始遍历 */
				pnode = list->head;
				int i = 0;
				for ( i = 0;i < index; ++i )
				{
					pnode = pnode->next;
				}
			}
			else
			{
				/* 从尾部开始遍历 */
				pnode = list->tail;
				int i = 0;
				for ( i = 0;i < list->size - index - 1; ++i )
				{
					pnode = pnode->prev;
				}
			}
			pnode->prev->next = pnode->next;
			pnode->next->prev = pnode->prev;

			deleteNode(pnode);
		}
	}
}

int list_getSize(LIST *list)
{
	if ( NULL == list )
	{
		return -1;
	}
	else
	{
		/* do nothing */
	}
	return list->size;
}

void *list_getData(LIST *list, int index)
{
	if ( NULL == list )
	{
		return NULL;
	}
	else
	{
		if( index < 0 || index >= list->size )
		{
			return NULL;
		}
		else
		{
			LISTNODE *pnode = NULL;
			/* 根据index大小判断从哪边开始更快，最终到达index的位置进行操作 */
			if ( index <= list->size / 2 )
			{
				/* 从头部开始遍历 */
				pnode = list->head;
				int i = 0;
				for ( i = 0;i < index; ++i )
				{
					pnode = pnode->next;
				}
			}
			else
			{
				/* 从尾部开始遍历 */
				pnode = list->tail;
				int i = 0;
				for ( i = 0;i < list->size - index - 1; ++i )
				{
					pnode = pnode->prev;
				}
			}
			return pnode->data;
		}
	}
	return NULL;
}

int list_isEmpty(LIST *list)
{
	if ( NULL == list )
	{
		return -1;
	}
	else
	{
		/* do nothing */
	}
	return ((list->size > 0) ? 0 : 1);
}

void printfListData(LIST *list)
{
	LISTNODE *node = list->head;
	DATA *pdata = NULL;
	printf("Data: ");
	while(node)
	{
		pdata = (DATA *)node->data;
		printf("%d, ", pdata->size);
		
		node = node->next;
	}
	printf("\b\n");
}

int main()
{
	DATA *data = (DATA *)malloc(sizeof(DATA));
	LIST *common_list = list_create();
	printfListData(common_list);
	data->size = 1;
	list_addTail(common_list, (void *)data, sizeof(DATA));
	printfListData(common_list);
	data->size = 2;
	list_addTail(common_list, (void *)data, sizeof(DATA));
	printfListData(common_list);
	data->size = 3;
	list_addHead(common_list, (void *)data, sizeof(DATA));
	printfListData(common_list);
	data->size = 4;
	list_addIndex(common_list, 1, (void *)data, sizeof(DATA));
	printfListData(common_list);

	DATA *p = list_getData(common_list, 0);
	printf("data: %d\n", p->size);

	p = list_getData(common_list, 1);
	printf("data: %d\n", p->size);

	p = list_getData(common_list, 2);
	printf("data: %d\n", p->size);

	p = list_getData(common_list, 3);
	printf("data: %d\n", p->size);

	list_delIndex(common_list, 1);
	printfListData(common_list);

	list_delHead(common_list);
	printfListData(common_list);

	list_delTail(common_list);
	printfListData(common_list);

	list_delHead(common_list);
	printfListData(common_list);

	list_delTail(common_list);
	printfListData(common_list);

	list_destory(common_list);
	return 0;
}