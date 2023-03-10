#include "graphics.h"

#include "system.h"
#include "camera.h"



void ClearScreen()
{
	SDL_SetRenderTarget(main_renderer, main_texture);
	SDL_SetRenderDrawColor(main_renderer, 7, 0, 23, 255);
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

void DrawRectInWorld(int x, int y, int w, int h)
{
	DrawRect(x - camera_renderpos_x, y - camera_renderpos_y, w, h);
}

void FillRectInWorld(int x, int y, int w, int h)
{
	FillRect(x - camera_renderpos_x, y - camera_renderpos_y, w, h);
}
