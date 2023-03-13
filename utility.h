#pragma once

#include <stdlib.h>



#define COMPILE_USE_STD_ALLOCATION 1

#if COMPILE_USE_STD_ALLOCATION

#define MALLOC(size)			malloc(size)
#define CALLOC(count, size)		calloc(count, size)
#define REALLOC(block, size)	realloc(block, size)
#define	ALLOC_TYPE(type)		(type*)calloc(1, sizeof(type))
#define FREE(block)				free(block)

#else

void* Big_Malloc(const size_t size);
void* Big_Calloc(const int count, const size_t size);
void* Big_Realloc(void* block, const size_t size);
void* Big_Free(const void* const block);

#define MALLOC(size)			Big_Malloc(size)
#define CALLOC(count, size)		Big_Calloc(count, size)
#define	ALLOC_TYPE(type)		Big_Calloc(1, sizeof(type))
#define FREE(block)				Big_Free(block)

#endif



typedef enum
{
	FALSE = 0,
	TRUE = 1
} Bool;



typedef struct ListNode
{
	struct ListNode* prev;
	struct ListNode* next;
} ListNode;

typedef struct
{
	ListNode*	begin;
	ListNode*	end;
	int			count;
} List;

#define ListLinksHeader(type) struct type* prev; struct type* next



void PushFrontList(List* const list, ListNode* const node);
void PushBackList(List* const list, ListNode* const	node);
void* PopFrontList(List* const	list);
void* PopBackList(List* const list);
void UnlinkFromList(List* const	list, ListNode* const node);

/// @brief Removes all elements from a list and calling the destroy_func on every element.
/// This does not free the list itself, only empties it.
/// @param list Pointer to the list to empty.
/// @param destroy_func Pointer to a destructor function, or NULL to use FREE().
void ClearList(
	List* const list,
	void(*destroy_func)(void*)
);



// Returns a random number from 0 to 65535
unsigned short Random();

// Returns a random float in the range from min to max
float RandomRange(float min, float max);

void SetRandomSeed(unsigned short seed);
