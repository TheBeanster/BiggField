#pragma once

#include "utility.h"



typedef unsigned short Tile;

#define TILE_SIZE 8
#define TILE_SHIFT 3
#define TILE_MASK 7

#define TMBLOCK_WIDTH 16
#define TMBLOCK_WIDTH_SHIFT 4
#define TMBLOCK_WIDTH_MASK 15
#define TMBLOCK_HEIGHT 8
#define TMBLOCK_HEIGHT_SHIFT 3
#define TMBLOCK_HEIGHT_MASK 7

#define TMBLOCK_COUNT (TMBLOCK_WIDTH * TMBLOCK_HEIGHT)

typedef struct
{
	Tile tiles[TMBLOCK_COUNT];
} TilemapBlock;



#define WORLD_TMBLOCK_SIZE 5
#define WORLD_TMBLOCK_COUNT (WORLD_TMBLOCK_SIZE * WORLD_TMBLOCK_SIZE)

extern TilemapBlock* world_tilemapblocks[WORLD_TMBLOCK_COUNT];



void InitDebugTilemap();



Tile GetTile(int x, int y);



void RenderTilemap();
