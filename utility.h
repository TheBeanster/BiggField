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

