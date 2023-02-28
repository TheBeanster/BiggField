#pragma once

#include "utility.h"

enum
{
	ET_NULL,
	ET_TESTENTITIY,
};
typedef unsigned char EntityType;

enum
{
	EF_NOUPDATE = (1 << 0),
	EF_NORENDER = (1 << 1),
	
	EF_DELETE = (1 << 15),
};
typedef unsigned short EntityFlags;



typedef struct Entity
{
	struct Entity* prev;
	struct Entity* next;

	EntityFlags flags;
	EntityType type;

	float x;
	float y;
} Entity;



Entity* SpawnEntity(EntityType type, float x, float y);

void UpdateEntity(Entity* e, float dt);

void RenderEntity(Entity* e);
