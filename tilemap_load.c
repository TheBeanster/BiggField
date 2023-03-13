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
	
	block->decorated = FALSE;

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
	/*4---R-D-*/ TT_DRC,
	/*4---R-DU*/ TT_TLCD,
	/*4---RL--*/ TT_LR_0 | TTF_VARY,
	/*4---RL-U*/ TT_TD,
	/*4---RLD-*/ TT_TUCR,
	/*4---RLDU*/ TT_XDR,
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
	/*4--1R-DU*/ TT_TLCD,
	/*4--1RL--*/ TT_LR_0 | TTF_VARY,
	/*4--1RL-U*/ TT_TDCL,
	/*4--1RLD-*/ TT_TUCR,
	/*4--1RLDU*/ TT_XULDR,
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
	/*4-2-R-D-*/ TT_DRC,
	/*4-2-R-DU*/ TT_TLW_0 | TTF_VARY,
	/*4-2-RL--*/ TT_LR_0 | TTF_VARY,
	/*4-2-RL-U*/ TT_TDCR,
	/*4-2-RLD-*/ TT_TUCR,
	/*4-2-RLDU*/ TT_XL,
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
	/*4-21R-D-*/ TT_DRC,
	/*4-21R-DU*/ TT_TLW_0 | TTF_VARY,
	/*4-21RL--*/ TT_LR_0 | TTF_VARY,
	/*4-21RL-U*/ TT_TDW_0 | TTF_VARY,
	/*4-21RLD-*/ TT_TUCR,
	/*4-21RLDU*/ TT_XDLC,



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
	/*43--R-D-*/ TT_DRC,
	/*43--R-DU*/ TT_TLCD,
	/*43--RL--*/ TT_LR_0 | TTF_VARY,
	/*43--RL-U*/ TT_TD,
	/*43--RLD-*/ TT_TUW_0 | TTF_VARY,
	/*43--RLDU*/ TT_XU,

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
	/*43-1R-D-*/ TT_DRC,
	/*43-1R-DU*/ TT_TLCD,
	/*43-1RL--*/ TT_LR_0 | TTF_VARY,
	/*43-1RL-U*/ TT_TDCL,
	/*43-1RLD-*/ TT_TUW_0 | TTF_VARY,
	/*43-1RLDU*/ TT_XURC,

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
	/*432-R-D-*/ TT_DRC,
	/*432-R-DU*/ TT_TLW_0 | TTF_VARY,
	/*432-RL--*/ TT_LR_0 | TTF_VARY,
	/*432-RL-U*/ TT_TDCR,
	/*432-RLD-*/ TT_TUW_0 | TTF_VARY,
	/*432-RLDU*/ TT_XULC,

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
	/*4321R-D-*/ TT_DRC,
	/*4321R-DU*/ TT_TLW_0 | TTF_VARY,
	/*4321RL--*/ TT_LR_0 | TTF_VARY,
	/*4321RL-U*/ TT_TDW_0 | TTF_VARY,
	/*4321RLD-*/ TT_TUW_0 | TTF_VARY,
	/*4321RLDU*/ TT_S_0 | TTF_VARY,
};





// Unsafe get tile
#define GET_TILE_U(x, y) (block->tiles[(x) | ((y) << TMBLOCK_WIDTH_SHIFT)])

#define UPDATE_TILE(x, y)										\
	type = tiletype_neighbours[type];							\
	if (type & TTF_VARY)										\
	type += (Random() & 3);										\
																\
	type &= 0x7F;												\
																\
	GET_TILE_U(x, y) = (t << 8) | (short)type

#define GET_BLOCK_TILE(block, x, y) (block->tiles[(x) | ((y) << TMBLOCK_WIDTH_SHIFT)] != 0)
#define GET_NBLOCK_TILE(block, x, y) (block ? GET_BLOCK_TILE(block, x, y) : 1)
#define BLOCK_RIGHT (TMBLOCK_WIDTH - 1)
#define BLOCK_BOTTOM (TMBLOCK_HEIGHT - 1)



void DecorateBlockTiles(int bx, int by)
{
	SetRandomSeed(0);

	TilemapBlock* block = GET_BLOCK(bx, by);

	if (!block) return;
	if (block->decorated) return;

	block->decorated = TRUE;

	for (int y = 1; y < TMBLOCK_HEIGHT - 1; y++)
	{
		for (int x = 1; x < TMBLOCK_WIDTH - 1; x++)
		{
			Tile t = GET_TILE_U(x, y);
			if (t != 0)
			{
				if (t == 15) GET_TILE_U(x, y) = 0xFFFF;
				else
				{
					int type = (GET_TILE_U(x + 1, y + 1) != 0); // dr
					type <<= 1; type |= (GET_TILE_U(x - 1, y + 1) != 0); // dl
					type <<= 1; type |= (GET_TILE_U(x + 1, y - 1) != 0); // ur
					type <<= 1; type |= (GET_TILE_U(x - 1, y - 1) != 0); // ul
					type <<= 1; type |= (GET_TILE_U(x + 1, y) != 0); // right
					type <<= 1; type |= (GET_TILE_U(x - 1, y) != 0); // left
					type <<= 1; type |= (GET_TILE_U(x, y + 1) != 0); // down
					type <<= 1; type |= (GET_TILE_U(x, y - 1) != 0); // up
					UPDATE_TILE(x, y);
				}
			}
		}
	}



	TilemapBlock* blocku = NULL;
	if (by > 0) blocku = GET_BLOCK(bx, by - 1);

	TilemapBlock* blockd = NULL;
	if (by < WORLD_TMBLOCKLOAD_HEIGHT - 1) blockd = GET_BLOCK(bx, by + 1);

	TilemapBlock* blockl = NULL;
	if (bx > 0) blockl = GET_BLOCK(bx - 1, by);

	TilemapBlock* blockr = NULL;
	if (bx < WORLD_TMBLOCKLOAD_WIDTH - 1) blockr = GET_BLOCK(bx + 1, by);

	for (int x = 1; x < TMBLOCK_WIDTH - 1; x++)
	{
		int type;
		Tile t;
		
		t = GET_TILE_U(x, 0);
		if (t != 0)
		{
			if (t == 15) GET_TILE_U(x, 0) = 0xFFFF;
			else
			{
				type = GET_BLOCK_TILE(block, x + 1, 1); // dr
				type <<= 1; type |= GET_BLOCK_TILE(block, x - 1, 1); // dl
				type <<= 1; type |= GET_NBLOCK_TILE(blocku, x + 1, BLOCK_BOTTOM); // ur
				type <<= 1; type |= GET_NBLOCK_TILE(blocku, x - 1, BLOCK_BOTTOM); // ul
				type <<= 1; type |= GET_BLOCK_TILE(block, x + 1, 0); // right
				type <<= 1; type |= GET_BLOCK_TILE(block, x - 1, 0); // left
				type <<= 1; type |= GET_BLOCK_TILE(block, x, 1); // down
				type <<= 1; type |= GET_NBLOCK_TILE(blocku, x, BLOCK_BOTTOM); // up
				UPDATE_TILE(x, 0);
			}
		}



		t = GET_TILE_U(x, BLOCK_BOTTOM);
		if (t != 0)
		{
			if (t == 15) GET_TILE_U(x, BLOCK_BOTTOM) = 0xFFFF;
			else
			{
				type = GET_NBLOCK_TILE(blockd, x + 1, 0); // dr
				type <<= 1; type |= GET_NBLOCK_TILE(blockd, x - 1, 0); // dl
				type <<= 1; type |= GET_BLOCK_TILE(block, x + 1, BLOCK_BOTTOM - 1); // ur
				type <<= 1; type |= GET_BLOCK_TILE(block, x - 1, BLOCK_BOTTOM - 1); // ul
				type <<= 1; type |= GET_BLOCK_TILE(block, x + 1, BLOCK_BOTTOM); // right
				type <<= 1; type |= GET_BLOCK_TILE(block, x - 1, BLOCK_BOTTOM); // left
				type <<= 1; type |= GET_NBLOCK_TILE(blockd, x, 0); // down
				type <<= 1; type |= GET_BLOCK_TILE(block, x, BLOCK_BOTTOM - 1); // up
				UPDATE_TILE(x, BLOCK_BOTTOM);
			}
		}
	}



	for (int y = 1; y < TMBLOCK_HEIGHT - 1; y++)
	{
		int type;
		Tile t;

		t = GET_TILE_U(0, y);
		if (t != 0)
		{
			if (t == 15) GET_TILE_U(0, y) = 0xFFFF;
			else
			{
				type = GET_BLOCK_TILE(block, 1, y + 1); // dr
				type <<= 1; type |= GET_NBLOCK_TILE(blockl, BLOCK_RIGHT, y + 1); // dl
				type <<= 1; type |= GET_BLOCK_TILE(block, 1, y - 1); // ur
				type <<= 1; type |= GET_NBLOCK_TILE(blockl, BLOCK_RIGHT, y - 1); // ul
				type <<= 1; type |= GET_BLOCK_TILE(block, 1, y); // right
				type <<= 1; type |= GET_NBLOCK_TILE(blockl, BLOCK_RIGHT, y); // left
				type <<= 1; type |= GET_BLOCK_TILE(block, 0, y + 1); // down
				type <<= 1; type |= GET_BLOCK_TILE(block, 0, y - 1); // up
				UPDATE_TILE(0, y);
			}
		}



		t = GET_TILE_U(BLOCK_RIGHT, y);
		if (t != 0)
		{
			if (t == 15) GET_TILE_U(BLOCK_RIGHT, y) = 0xFFFF;
			else
			{
				type = GET_NBLOCK_TILE(blockr, 0, y + 1); // dr
				type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT - 1, y + 1); // dl
				type <<= 1; type |= GET_NBLOCK_TILE(blockr, 0, y - 1); // ur
				type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT - 1, y - 1); // ul
				type <<= 1; type |= GET_NBLOCK_TILE(blockr, 0, y); // right
				type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT - 1, y); // left
				type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT, y + 1); // down
				type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT, y - 1); // up
				UPDATE_TILE(BLOCK_RIGHT, y);
			}
		}
	}



	int type = 0;
	Tile t = 0;

	// ul corner
	t = GET_TILE_U(0, 0);
	if (t != 0)
	{
		if (t == 15) GET_TILE_U(0, 0) = 0xFFFF;
		else
		{
			TilemapBlock* blockul = NULL;
			if (bx > 0 && by > 0) blockul = GET_BLOCK(bx - 1, by - 1);

			type = GET_BLOCK_TILE(block, 1, 1); // dr
			type <<= 1; type |= GET_NBLOCK_TILE(blockl, BLOCK_RIGHT, 1); // dl
			type <<= 1; type |= GET_NBLOCK_TILE(blocku, 1, BLOCK_BOTTOM); // ur
			type <<= 1; type |= GET_NBLOCK_TILE(blockul, BLOCK_RIGHT, BLOCK_BOTTOM); // ul
			type <<= 1; type |= GET_BLOCK_TILE(block, 1, 0); // right
			type <<= 1; type |= GET_NBLOCK_TILE(blockl, BLOCK_RIGHT, 0); // left
			type <<= 1; type |= GET_BLOCK_TILE(block, 0, 1); // down
			type <<= 1; type |= GET_NBLOCK_TILE(blocku, 0, BLOCK_BOTTOM); // up
			UPDATE_TILE(0, 0);
		}
	}



	// ur
	t = GET_TILE_U(BLOCK_RIGHT, 0);
	if (t != 0)
	{
		if (t == 15) GET_TILE_U(BLOCK_RIGHT, 0) = 0xFFFF;
		else
		{
			TilemapBlock* blockur = NULL;
			if (bx < WORLD_TMBLOCKLOAD_WIDTH - 1 && by > 0) blockur = GET_BLOCK(bx + 1, by - 1);

			type = GET_NBLOCK_TILE(blockr, 0, 1); // dr
			type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT - 1, 1); // dl
			type <<= 1; type |= GET_NBLOCK_TILE(blockur, 0, BLOCK_BOTTOM); // ur
			type <<= 1; type |= GET_NBLOCK_TILE(blocku, BLOCK_RIGHT - 1, BLOCK_BOTTOM); // ul
			type <<= 1; type |= GET_NBLOCK_TILE(blockr, 0, 0); // right
			type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT - 1, 0); // left
			type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT, 1); // down
			type <<= 1; type |= GET_NBLOCK_TILE(blocku, BLOCK_RIGHT, BLOCK_BOTTOM); // up
			UPDATE_TILE(BLOCK_RIGHT, 0);
		}
	}



	// dl
	t = GET_TILE_U(0, BLOCK_BOTTOM);
	if (t != 0)
	{
		if (t == 15) GET_TILE_U(0, BLOCK_BOTTOM) = 0xFFFF;
		else
		{
			TilemapBlock* blockdl = NULL;
			if (bx > 0 && by < WORLD_TMBLOCKLOAD_HEIGHT - 1) blockdl = GET_BLOCK(bx - 1, by + 1);

			type = GET_NBLOCK_TILE(blockd, 1, 0); // dr
			type <<= 1; type |= GET_NBLOCK_TILE(blockdl, BLOCK_RIGHT, 0); // dl
			type <<= 1; type |= GET_BLOCK_TILE(block, 1, BLOCK_BOTTOM - 1); // ur
			type <<= 1; type |= GET_NBLOCK_TILE(blockl, BLOCK_RIGHT, BLOCK_BOTTOM - 1); // ul
			type <<= 1; type |= GET_BLOCK_TILE(block, 1, BLOCK_BOTTOM); // right
			type <<= 1; type |= GET_NBLOCK_TILE(blockl, BLOCK_RIGHT, BLOCK_BOTTOM); // left
			type <<= 1; type |= GET_NBLOCK_TILE(blockd, 0, 0); // down
			type <<= 1; type |= GET_BLOCK_TILE(block, 0, BLOCK_BOTTOM - 1); // up
			UPDATE_TILE(0, BLOCK_BOTTOM);
		}
	}



	// dr
	t = GET_TILE_U(BLOCK_RIGHT, BLOCK_BOTTOM);
	if (t != 0)
	{
		if (t == 15) GET_TILE_U(BLOCK_RIGHT, BLOCK_BOTTOM) = 0xFFFF;
		else
		{
			TilemapBlock* blockdr = NULL;
			if (bx < WORLD_TMBLOCKLOAD_WIDTH - 1 && by < WORLD_TMBLOCKLOAD_HEIGHT - 1) blockdr = GET_BLOCK(bx + 1, by + 1);

			type = GET_NBLOCK_TILE(blockdr, 0, 0); // dr
			type <<= 1; type |= GET_NBLOCK_TILE(blockd, BLOCK_RIGHT - 1, 0); // dl
			type <<= 1; type |= GET_NBLOCK_TILE(blockr, 0, BLOCK_BOTTOM - 1); // ur
			type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT - 1, BLOCK_BOTTOM - 1); // ul
			type <<= 1; type |= GET_NBLOCK_TILE(blockr, 0, BLOCK_BOTTOM); // right
			type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT - 1, BLOCK_BOTTOM); // left
			type <<= 1; type |= GET_NBLOCK_TILE(blockd, BLOCK_RIGHT, 0); // down
			type <<= 1; type |= GET_BLOCK_TILE(block, BLOCK_RIGHT, BLOCK_BOTTOM - 1); // up
			UPDATE_TILE(BLOCK_RIGHT, BLOCK_BOTTOM);
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