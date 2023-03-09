#include "tilemap.h"



FILE* tilemap_file = NULL;



Bool OpenTilemapFile(const char* const name)
{
	tilemap_file = fopen(name, "rb");

	if (!tilemap_file)
	{
		printf("ERROR! : Couldn't open tilemap file\n");
		return FALSE;
	}

	fread(&world_block_width, sizeof(int), 1, tilemap_file);
	fread(&world_block_height, sizeof(int), 1, tilemap_file);
	world_block_count = world_block_width * world_block_height;

	printf("Opened tilemap \"%s\". w = %i, h = %i\n", name, world_block_width, world_block_height);

	return TRUE;
}



static int get_block_ptr(int i)
{
	fseek(tilemap_file, 8 + (i << 2), SEEK_SET);
	int ptr;
	fread(&ptr, sizeof(int), 1, tilemap_file);
	return ptr;
}



TilemapBlock* LoadBlock(int bx, int by)
{
	if (bx < 0 || bx >= world_block_width || by < 0 || by >= world_block_height) return NULL;

	int ptr = get_block_ptr(bx + (by * world_block_width));
	if (!ptr) return NULL;

	TilemapBlock* block = MALLOC(sizeof(TilemapBlock));
	
	fseek(tilemap_file, ptr, SEEK_SET);

	for (int i = 0; i < TMBLOCK_COUNT >> 1; i++)
	{
		unsigned char byte = fgetc(tilemap_file);
		block->tiles[(i << 1)] = (Tile)(byte & 0b00001111);
		block->tiles[(i << 1) + 1] = (Tile)(byte >> 4);
	}

	return block;
}




// Flag tile to have variations
#define TTF_VARY 0b10000000

const unsigned char tiletype_neighbours[256] =
{
	/*--------*/ TT_N_0 | TTF_VARY,
	/*-------U*/ TT_U,
	/*------D-*/ TT_D,
	/*------DU*/ TT_UD_0 | TTF_VARY,
	/*-----L--*/ TT_L,
	/*-----L-U*/ TT_UL,
	/*-----LD-*/ TT_DL,
	/*-----LDU*/ TT_TR,
	/*----R---*/ TT_R,
	/*----R--U*/ TT_UR,
	/*----R-D-*/ TT_DR,
	/*----R-DU*/ TT_TL,
	/*----RL--*/ TT_LR_0 | TTF_VARY,
	/*----RL-U*/ TT_TD,
	/*----RLD-*/ TT_TU,
	/*----RLDU*/ TT_X,
	/*---1----*/ TT_N_0 | TTF_VARY,
	/*---1---U*/ TT_U,
	/*---1--D-*/ TT_D,
	/*---1--DU*/ TT_UD_0 | TTF_VARY,
	/*---1-L--*/ TT_L,
	/*---1-L-U*/ TT_ULC,
	/*---1-LD-*/ TT_DL,
	/*---1-LDU*/ TT_TRCU,
	/*---1R---*/ TT_R,
	/*---1R--U*/ TT_UR,
	/*---1R-D-*/ TT_DR,
	/*---1R-DU*/ TT_TL,
	/*---1RL--*/ TT_LR_0 | TTF_VARY,
	/*---1RL-U*/ TT_TDCL,
	/*---1RLD-*/ TT_TU,
	/*---1RLDU*/ TT_XUL,
	/*--2-----*/ TT_N_0 | TTF_VARY,
	/*--2----U*/ TT_U,
	/*--2---D-*/ TT_D,
	/*--2---DU*/ TT_UD_0 | TTF_VARY,
	/*--2--L--*/ TT_L,
	/*--2--L-U*/ TT_UL,
	/*--2--LD-*/ TT_DL,
	/*--2--LDU*/ TT_TR,
	/*--2-R---*/ TT_R,
	/*--2-R--U*/ TT_URC,
	/*--2-R-D-*/ TT_DR,
	/*--2-R-DU*/ TT_TLCU,
	/*--2-RL--*/ TT_LR_0 | TTF_VARY,
	/*--2-RL-U*/ TT_TDCR,
	/*--2-RLD-*/ TT_TU,
	/*--2-RLDU*/ TT_XUR,
	/*--21----*/ TT_N_0 | TTF_VARY,
	/*--21---U*/ TT_U,
	/*--21--D-*/ TT_D,
	/*--21--DU*/ TT_UD_0 | TTF_VARY,
	/*--21-L--*/ TT_L,
	/*--21-L-U*/ TT_ULC,
	/*--21-LD-*/ TT_DL,
	/*--21-LDU*/ TT_TRCU,
	/*--21R---*/ TT_R,
	/*--21R--U*/ TT_URC,
	/*--21R-D-*/ TT_DR,
	/*--21R-DU*/ TT_TLCU,
	/*--21RL--*/ TT_LR_0 | TTF_VARY,
	/*--21RL-U*/ TT_TDW_0 | TTF_VARY,
	/*--21RLD-*/ TT_TU,
	/*--21RLDU*/ TT_XD,
	/*-3------*/ TT_N_0 | TTF_VARY,
	/*-3-----U*/ TT_U,
	/*-3----D-*/ TT_D,
	/*-3----DU*/ TT_UD_0 | TTF_VARY,
	/*-3---L--*/ TT_L,
	/*-3---L-U*/ TT_UL,
	/*-3---LD-*/ TT_DLC,
	/*-3---LDU*/ TT_TRCD,
	/*-3--R---*/ TT_R,
	/*-3--R--U*/ TT_UR,
	/*-3--R-D-*/ TT_DR,
	/*-3--R-DU*/ TT_TL,
	/*-3--RL--*/ TT_LR_0 | TTF_VARY,
	/*-3--RL-U*/ TT_TD,
	/*-3--RLD-*/ TT_TUCL,
	/*-3--RLDU*/ TT_XDL,
	/*-3-1----*/ TT_N_0 | TTF_VARY,
	/*-3-1---U*/ TT_U,
	/*-3-1--D-*/ TT_D,
	/*-3-1--DU*/ TT_UD_0 | TTF_VARY,
	/*-3-1-L--*/ TT_L,
	/*-3-1-L-U*/ TT_ULC,
	/*-3-1-LD-*/ TT_DLC,
	/*-3-1-LDU*/ TT_TRW_0 | TTF_VARY,
	/*-3-1R---*/ TT_R,
	/*-3-1R--U*/ TT_UR,
	/*-3-1R-D-*/ TT_DR,
	/*-3-1R-DU*/ TT_TL,
	/*-3-1RL--*/ TT_LR_0 | TTF_VARY,
	/*-3-1RL-U*/ TT_TDCL,
	/*-3-1RLD-*/ TT_TUCL,
	/*-3-1RLDU*/ TT_XR,
	/*-32-----*/ TT_N_0 | TTF_VARY,
	/*-32----U*/ TT_U,
	/*-32---D-*/ TT_D,
	/*-32---DU*/ TT_UD_0 | TTF_VARY,
	/*-32--L--*/ TT_L,
	/*-32--L-U*/ TT_UL,
	/*-32--LD-*/ TT_DLC,
	/*-32--LDU*/ TT_TRCD,
	/*-32-R---*/ TT_R,
	/*-32-R--U*/ TT_URC,
	/*-32-R-D-*/ TT_DR,
	/*-32-R-DU*/ TT_TLCU,
	/*-32-RL--*/ TT_LR_0 | TTF_VARY,
	/*-32-RL-U*/ TT_TDCR,
	/*-32-RLD-*/ TT_TUCL,
	/*-32-RLDU*/ TT_XURDL,
	/*-321----*/ TT_N_0 | TTF_VARY,
	/*-321---U*/ TT_U,
	/*-321--D-*/ TT_D,
	/*-321--DU*/ TT_UD_0 | TTF_VARY,
	/*-321-L--*/ TT_L,
	/*-321-L-U*/ TT_ULC,
	/*-321-LD-*/ TT_DLC,
	/*-321-LDU*/ TT_TRW_0 | TTF_VARY,
	/*-321R---*/ TT_R,
	/*-321R--U*/ TT_URC,
	/*-321R-D-*/ TT_DR,
	/*-321R-DU*/ TT_TLCU,
	/*-321RL--*/ TT_LR_0 | TTF_VARY,
	/*-321RL-U*/ TT_TDW_0 | TTF_VARY,
	/*-321RLD-*/ TT_TUCL,
	/*-321RLDU*/ TT_XDRC,

	/*4-------*/ TT_N_0 | TTF_VARY,
	/*4------U*/ TT_U,
	/*4-----D-*/ TT_D,
	/*4-----DU*/ TT_UD_0 | TTF_VARY,
	/*4----L--*/ TT_L,
	/*4----L-U*/ TT_UL,
	/*4----LD-*/ TT_DL,
	/*4----LDU*/ TT_TR,
	/*4---R---*/ TT_R,
	/*4---R--U*/ TT_UR,
	/*4---R-D-*/ TT_DR,
	/*4---R-DU*/ TT_TL,
	/*4---RL--*/ TT_LR_0 | TTF_VARY,
	/*4---RL-U*/ TT_TD,
	/*4---RLD-*/ TT_TU,
	/*4---RLDU*/ TT_X,
	/*4--1----*/ TT_N_0 | TTF_VARY,
	/*4--1---U*/ TT_U,
	/*4--1--D-*/ TT_D,
	/*4--1--DU*/ TT_UD_0 | TTF_VARY,
	/*4--1-L--*/ TT_L,
	/*4--1-L-U*/ TT_ULC,
	/*4--1-LD-*/ TT_DL,
	/*4--1-LDU*/ TT_TRCU,
	/*4--1R---*/ TT_R,
	/*4--1R--U*/ TT_UR,
	/*4--1R-D-*/ TT_DR,
	/*4--1R-DU*/ TT_TL,
	/*4--1RL--*/ TT_LR_0 | TTF_VARY,
	/*4--1RL-U*/ TT_TDCL,
	/*4--1RLD-*/ TT_TU,
	/*4--1RLDU*/ TT_XUL,
	/*4-2-----*/ TT_N_0 | TTF_VARY,
	/*4-2----U*/ TT_U,
	/*4-2---D-*/ TT_D,
	/*4-2---DU*/ TT_UD_0 | TTF_VARY,
	/*4-2--L--*/ TT_L,
	/*4-2--L-U*/ TT_UL,
	/*4-2--LD-*/ TT_DL,
	/*4-2--LDU*/ TT_TR,
	/*4-2-R---*/ TT_R,
	/*4-2-R--U*/ TT_URC,
	/*4-2-R-D-*/ TT_DR,
	/*4-2-R-DU*/ TT_TLCU,
	/*4-2-RL--*/ TT_LR_0 | TTF_VARY,
	/*4-2-RL-U*/ TT_TDCR,
	/*4-2-RLD-*/ TT_TU,
	/*4-2-RLDU*/ TT_XUR,
	/*4-21----*/ TT_N_0 | TTF_VARY,
	/*4-21---U*/ TT_U,
	/*4-21--D-*/ TT_D,
	/*4-21--DU*/ TT_UD_0 | TTF_VARY,
	/*4-21-L--*/ TT_L,
	/*4-21-L-U*/ TT_ULC,
	/*4-21-LD-*/ TT_DL,
	/*4-21-LDU*/ TT_TRCU,
	/*4-21R---*/ TT_R,
	/*4-21R--U*/ TT_URC,
	/*4-21R-D-*/ TT_DR,
	/*4-21R-DU*/ TT_TLCU,
	/*4-21RL--*/ TT_LR_0 | TTF_VARY,
	/*4-21RL-U*/ TT_TDW_0 | TTF_VARY,
	/*4-21RLD-*/ TT_TU,
	/*4-21RLDU*/ TT_XD,
	/*43------*/ TT_N_0 | TTF_VARY,
	/*43-----U*/ TT_U,
	/*43----D-*/ TT_D,
	/*43----DU*/ TT_UD_0 | TTF_VARY,
	/*43---L--*/ TT_L,
	/*43---L-U*/ TT_UL,
	/*43---LD-*/ TT_DLC,
	/*43---LDU*/ TT_TRCD,
	/*43--R---*/ TT_R,
	/*43--R--U*/ TT_UR,
	/*43--R-D-*/ TT_DR,
	/*43--R-DU*/ TT_TL,
	/*43--RL--*/ TT_LR_0 | TTF_VARY,
	/*43--RL-U*/ TT_TD,
	/*43--RLD-*/ TT_TUCL,
	/*43--RLDU*/ TT_XDL,
	/*43-1----*/ TT_N_0 | TTF_VARY,
	/*43-1---U*/ TT_U,
	/*43-1--D-*/ TT_D,
	/*43-1--DU*/ TT_UD_0 | TTF_VARY,
	/*43-1-L--*/ TT_L,
	/*43-1-L-U*/ TT_ULC,
	/*43-1-LD-*/ TT_DLC,
	/*43-1-LDU*/ TT_TRW_0 | TTF_VARY,
	/*43-1R---*/ TT_R,
	/*43-1R--U*/ TT_UR,
	/*43-1R-D-*/ TT_DR,
	/*43-1R-DU*/ TT_TL,
	/*43-1RL--*/ TT_LR_0 | TTF_VARY,
	/*43-1RL-U*/ TT_TDCL,
	/*43-1RLD-*/ TT_TUCL,
	/*43-1RLDU*/ TT_XR,
	/*432-----*/ TT_N_0 | TTF_VARY,
	/*432----U*/ TT_U,
	/*432---D-*/ TT_D,
	/*432---DU*/ TT_UD_0 | TTF_VARY,
	/*432--L--*/ TT_L,
	/*432--L-U*/ TT_UL,
	/*432--LD-*/ TT_DLC,
	/*432--LDU*/ TT_TRCD,
	/*432-R---*/ TT_R,
	/*432-R--U*/ TT_URC,
	/*432-R-D-*/ TT_DR,
	/*432-R-DU*/ TT_TLCU,
	/*432-RL--*/ TT_LR_0 | TTF_VARY,
	/*432-RL-U*/ TT_TDCR,
	/*432-RLD-*/ TT_TUCL,
	/*432-RLDU*/ TT_XURDL,
	/*4321----*/ TT_N_0 | TTF_VARY,
	/*4321---U*/ TT_U,
	/*4321--D-*/ TT_D,
	/*4321--DU*/ TT_UD_0 | TTF_VARY,
	/*4321-L--*/ TT_L,
	/*4321-L-U*/ TT_ULC,
	/*4321-LD-*/ TT_DLC,
	/*4321-LDU*/ TT_TRW_0 | TTF_VARY,
	/*4321R---*/ TT_R,
	/*4321R--U*/ TT_URC,
	/*4321R-D-*/ TT_DR,
	/*4321R-DU*/ TT_TLCU,
	/*4321RL--*/ TT_LR_0 | TTF_VARY,
	/*4321RL-U*/ TT_TDW_0 | TTF_VARY,
	/*4321RLD-*/ TT_TUCL,
	/*4321RLDU*/ TT_XDRC,
};

// Unsafe get tile
#define GET_TILE_U(x, y) block->tiles[x | (y << TMBLOCK_WIDTH_SHIFT)]

void DecorateBlockTiles(int bx, int by)
{
	TilemapBlock* block = GET_BLOCK(bx, by);

	printf("Decorating block, %i, %i\n", bx, by);

	if (!block) return;

	/*TilemapBlock* blockleft = bx > 0 ? GET_BLOCK(bx - 1, by) : NULL;
	TilemapBlock* blockright = bx < WORLD_TMBLOCKLOAD_WIDTH - 1 ? GET_BLOCK(bx + 1, by) : NULL;
	TilemapBlock* blockup = by > 0 ? GET_BLOCK(bx, by - 1) : NULL;
	TilemapBlock* blockdown = by < WORLD_TMBLOCKLOAD_HEIGHT - 1 ? GET_BLOCK(bx, by + 1) : NULL;

	printf("(%i, %i) : l%i, r%i, u%i, d%i\n", bx, by, blockleft, blockright, blockup, blockdown);*/
	
	for (int y = 1; y < TMBLOCK_HEIGHT - 1; y++)
	{
		for (int x = 1; x < TMBLOCK_WIDTH - 1; x++)
		{
			Tile t = GET_TILE_U(x, y);
			if (t == 0) continue;

			int type = GET_TILE_U(x + 1, y + 1); // dr
			type <<= 1; type |= (GET_TILE_U(x - 1, y + 1) != 0); // dl
			type <<= 1; type |= (GET_TILE_U(x + 1, y - 1) != 0); // ur
			type <<= 1; type |= (GET_TILE_U(x - 1, y - 1) != 0); // ul
			type <<= 1; type |= (GET_TILE_U(x + 1, y) != 0); // right
			type <<= 1; type |= (GET_TILE_U(x - 1, y) != 0); // left
			type <<= 1; type |= (GET_TILE_U(x, y + 1) != 0); // down
			type <<= 1; type |= (GET_TILE_U(x, y - 1) != 0); // up

			type = tiletype_neighbours[type];
			if (type & TTF_VARY)
				type += (rand() & 3);

			GET_TILE_U(x, y) = (t << 8) | (short)type;
		}
	}


}



void TestLoadTilemap()
{
	for (int y = 0; y < WORLD_TMBLOCKLOAD_HEIGHT; y++)
		for (int x = 0; x < WORLD_TMBLOCKLOAD_WIDTH; x++)
			world_tilemapblocks[x + (y * WORLD_TMBLOCKLOAD_WIDTH)] = LoadBlock(x, y);

	for (int y = 0; y < WORLD_TMBLOCKLOAD_HEIGHT; y++)
		for (int x = 0; x < WORLD_TMBLOCKLOAD_WIDTH; x++)
			DecorateBlockTiles(x, y);
}