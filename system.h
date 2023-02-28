#pragma once

#include <SDL.h>

#include "utility.h"

extern SDL_Window* main_window;
extern SDL_Renderer* main_renderer;
extern SDL_Texture* main_texture;

extern int main_window_width;
extern int main_window_height;
extern int main_texture_width;
extern int main_texture_height;

Bool InitSystem();

void UpdateWindow();



typedef enum
{
	SYSINP_EXIT = 1,
} SystemInputs;
extern SystemInputs system_inputs;

void UpdateInputs();
