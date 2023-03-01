#pragma once

#include <SDL.h>

#include "utility.h"

extern SDL_Window* main_window;
extern SDL_Renderer* main_renderer;
extern SDL_Texture* main_texture;

extern int main_window_width;
extern int main_window_height;
extern int screen_width;
extern int screen_height;

Bool SystemInit();

void UpdateWindow();

void SystemQuit();



typedef enum
{
	SYSINP_EXIT = 1,
} SystemInputs;
extern SystemInputs system_inputs;

extern char* keyboard_inputs;

extern int mousex;
extern int mousey;

typedef enum
{
	MOUSEBTN_LDOWN = 1,
	MOUSEBTN_RDOWN = 2,
} MouseBtns;
extern MouseBtns mousebtns;

void UpdateInputs();
