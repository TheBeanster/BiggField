#include "utility.h"

void PushFrontList(List* const list, ListNode* const node)
{
	if (list->begin == NULL)
	{
		// List is empty
		node->prev = NULL;
		node->next = NULL;
		list->begin = node;
		list->end = node;
	} else
	{
		node->prev = NULL;
		node->next = list->begin;
		list->begin->prev = node;
		list->begin = node;
	}
	list->count++;
}

void PushBackList(List* const list, ListNode* const node)
{
	if (list->begin == NULL)
	{
		// List is empty
		node->prev = NULL;
		node->next = NULL;
		list->begin = node;
		list->end = node;
	} else
	{
		node->prev = list->end;
		node->next = NULL;
		list->end->next = node;
		list->end = node;
	}
	list->count++;
}

void* PopFrontList(List* const list)
{
	ListNode* node = list->begin;
	UnlinkFromList(list, node);
	return node;
}

void* PopBackList(List* const list)
{
	ListNode* node = list->end;
	UnlinkFromList(list, node);
	return node;
}

void UnlinkFromList(List* const list, ListNode* const node)
{
	if (node->prev)
		node->prev->next = node->next;
	else
		list->begin = node->next;

	if (node->next)
		node->next->prev = node->prev;
	else
		list->end = node->prev;

	node->prev = NULL;
	node->next = NULL;

	list->count--;
}

void ClearList(List* const list, void(*destroy_func)(void*))
{
	if (!list) return;
	if (list->count <= 0) return;
	if (list->begin == NULL) return;
	if (list->end == NULL) return;

	ListNode* iterator = list->begin;
	while (iterator)
	{
		ListNode* delblock = iterator;
		iterator = iterator->next;
		if (destroy_func)
			destroy_func(delblock);
		else
			FREE(delblock);
	}
	list->begin = NULL;
	list->end = NULL;
	list->count = 0;
}

