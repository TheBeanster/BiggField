#pragma once

#include "utility.h"

#include <stdio.h>



typedef unsigned short Tile;

#define GET_TILE_STYLE(t) ((t & 0x0F00) >> 8)
#define GET_TILE_TYPE(t) (t & 0x00FF)

typedef enum
{
	TT_N_0,		// None
	TT_N_1,
	TT_N_2,
	TT_N_3,
	TT_U,		// U solid
	TT_D,
	TT_L,
	TT_R,
	TT_UD_0,	// U and D solid
	TT_UD_1,
	TT_UD_2,
	TT_UD_3,
	TT_LR_0,
	TT_LR_1,
	TT_LR_2,
	TT_LR_3,

	// Corners
	TT_UL,		// U and L solid
	TT_UR,
	TT_DL,
	TT_DR,
	TT_ULC,		// U and L and UL solid 
	TT_URC,
	TT_DLC,
	TT_DRC,

	// T sections
	TT_TU,		// All but U solid
	TT_TUCL,	// Same but DL solid
	TT_TUCR,
	TT_TUW_0,	// Wall up
	TT_TUW_1,
	TT_TUW_2,
	TT_TUW_3,
	TT_TD,
	TT_TDCL,
	TT_TDCR,
	TT_TDW_0,
	TT_TDW_1,
	TT_TDW_2,
	TT_TDW_3,
	TT_TL,
	TT_TLCU,
	TT_TLCD,
	TT_TLW_0,
	TT_TLW_1,
	TT_TLW_2,
	TT_TLW_3,
	TT_TR,
	TT_TRCU,
	TT_TRCD,
	TT_TRW_0,
	TT_TRW_1,
	TT_TRW_2,
	TT_TRW_3,

	// All edges
	TT_X,
	// One corner
	TT_XUL,		// UL solid
	TT_XUR,
	TT_XDL,
	TT_XDR,
	// Two corners
	TT_XU,		// No solid corners up
	TT_XD,
	TT_XL,
	TT_XR,
	TT_XULDR,	// Up left and down right solid
	TT_XURDL,	// Up right and down left solid
	// Three corners
	TT_XULC,	// UL not solid
	TT_XURC,
	TT_XDLC,
	TT_XDRC,
	// All corners and edges
	TT_S_0,
	TT_S_1,
	TT_S_2,
	TT_S_3,
} TileType;

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
	Bool decorated;
	Tile tiles[TMBLOCK_COUNT];

	List entities;
} TilemapBlock;



#define WORLD_TMBLOCKLOAD_WIDTH 9
#define WORLD_TMBLOCKLOAD_HEIGHT 9
#define WORLD_TMBLOCKLOAD_COUNT (WORLD_TMBLOCKLOAD_WIDTH * WORLD_TMBLOCKLOAD_HEIGHT)

extern TilemapBlock* world_tilemapblocks[WORLD_TMBLOCKLOAD_COUNT];

// Unsafe get block without out of bounds checks
#define GET_BLOCK(x, y) world_tilemapblocks[(x) + ((y) * WORLD_TMBLOCKLOAD_WIDTH)]

TilemapBlock* GetWorldBlock(int x, int y);
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

TilemapBlock* LoadBlock(int bx, int by);
void DecorateBlockTiles(int bx, int by);

void DeleteBlockEntities(TilemapBlock* block);

void TestLoadTilemap();



void ShiftTilemapAndEntities(int x, int y);

void RenderTilemap();
