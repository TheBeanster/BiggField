#pragma once

#include "utility.h"

#include <stdio.h>



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



#define WORLD_TMBLOCKLOAD_WIDTH 5
#define WORLD_TMBLOCKLOAD_HEIGHT 9
#define WORLD_TMBLOCKLOAD_COUNT (WORLD_TMBLOCKLOAD_WIDTH * WORLD_TMBLOCKLOAD_HEIGHT)

extern TilemapBlock* world_tilemapblocks[WORLD_TMBLOCKLOAD_COUNT];

TilemapBlock** GetWorldBlockPtr(int x, int y);

void InitDebugTilemap();



Tile GetTile(int x, int y);
Bool GetTileSolid(int x, int y);



extern int world_block_width;
extern int world_block_height;
extern int world_block_count;

extern FILE* tilemap_file;

extern int world_posx;
extern int world_posy;

Bool OpenTilemapFile(const char* const name);

TilemapBlock* LoadBlock(int x, int y);

void TestLoadTilemap();



void ShiftTilemapAndEntities(int x, int y);

void RenderTilemap();
