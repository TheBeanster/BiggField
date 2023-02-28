#include "graphics.h"

#include "system.h"



void ClearScreen()
{
	SDL_SetRenderTarget(main_renderer, main_texture);
	SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
	SDL_RenderClear(main_renderer);
}

void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(main_renderer, r, g, b, a);
}

void DrawRect(int x, int y, int w, int h)
{
	SDL_Rect r = { x, y, w, h };
	SDL_RenderDrawRect(main_renderer, &r);
}

void FillRect(int x, int y, int w, int h)
{
	SDL_Rect r = { x, y, w, h };
	SDL_RenderFillRect(main_renderer, &r);
}
