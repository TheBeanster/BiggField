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



TilemapBlock* LoadBlock(int x, int y)
{
	if (x < 0 || x >= world_block_width || y < 0 || y >= world_block_height) return NULL;

	int ptr = get_block_ptr(x + (y * world_block_width));
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



void TestLoadTilemap()
{
	for (int y = 0; y < world_block_height; y++)
	{
		for (int x = 0; x < world_block_width; x++)
		{
			world_tilemapblocks[x + (y * WORLD_TMBLOCKLOAD_WIDTH)] = LoadBlock(x, y);
		}
	}
}