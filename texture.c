#include "texture.h"

#include <stdio.h>

#include "system.h"



SDL_Texture* tilemaptiles_texture = NULL;



static SDL_Texture* load_texture(const char* const filename)
{
	printf("Loading texture: \"%s\"\n", filename);
	SDL_Surface* surface = SDL_LoadBMP(filename);

	if (surface == NULL)
	{
		printf("ERROR! : Couldn't load texture\n");
	} else
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 255, 255, 255));

		SDL_Texture* texture = SDL_CreateTextureFromSurface(main_renderer, surface);

		return texture;
	}

	return NULL;
}



Bool LoadTextures()
{
	tilemaptiles_texture = load_texture("tilemapTileTextures.bmp");

	return TRUE;
}
