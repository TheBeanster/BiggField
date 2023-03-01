#include "system.h"

#include <stdio.h>

SDL_Window* main_window = NULL;
SDL_Renderer* main_renderer = NULL;
SDL_Texture* main_texture = NULL;

int main_window_width = 0;
int main_window_height = 0;
int screen_width = 0;
int screen_height = 0;



static Bool init_main_window(int w, int h)
{
	main_window = SDL_CreateWindow(
		"big",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		w,
		h,
		0
	);
	if (!main_window)
	{
		printf("ERROR! Couldn't initialize main window. SDL_Error() = \"%s\"\n", SDL_GetError());
		return FALSE;
	}
	main_window_width = w;
	main_window_height = h;
	return TRUE;
}

static Bool init_main_renderer()
{
	main_renderer = SDL_CreateRenderer(
		main_window,
		-1,
		SDL_RENDERER_PRESENTVSYNC |
		SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_TARGETTEXTURE
	);
	if (!main_renderer)
	{
		printf("ERROR! Couldn't initialize main renderer. SDL_Error() = \"%s\"\n", SDL_GetError());
		return FALSE;
	}
	return TRUE;
}

static Bool init_main_texture(int w, int h)
{
	main_texture = SDL_CreateTexture(
		main_renderer,
		SDL_PIXELFORMAT_BGR888,
		SDL_TEXTUREACCESS_TARGET,
		w,
		h
	);
	if (!main_texture)
	{
		printf("ERROR! Couldn't initialize main texture. SDL_Error() = \"%s\"\n", SDL_GetError());
		return FALSE;
	}
	screen_width = w;
	screen_height = h;
	return TRUE;
}



Bool InitSystem()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("ERROR! Couldn't initialize SDL\n");
		return FALSE;
	}

	if (!init_main_window(640, 360))	goto onerror;
	if (!init_main_renderer())			goto onerror;
	if (!init_main_texture(320, 180))	goto onerror;

	keyboard_inputs = (char*)SDL_GetKeyboardState(NULL);

	return TRUE;

onerror:

	return FALSE;
}



void UpdateWindow()
{
	SDL_SetRenderTarget(main_renderer, NULL);
	SDL_RenderCopy(main_renderer, main_texture, NULL, NULL);
	SDL_RenderPresent(main_renderer);
}



SystemInputs system_inputs = 0;

char* keyboard_inputs = NULL;

int mousex = 0;
int mousey = 0;
MouseBtns mousebtns = 0;

void UpdateInputs()
{
	SDL_Event e;

	system_inputs = 0;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			system_inputs |= SYSINP_EXIT;
			break;

		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mousex, &mousey);
			mousex /= (main_window_width / (float)screen_width);
			mousey /= (main_window_height / (float)screen_height);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT)
				mousebtns |= MOUSEBTN_LDOWN;
			else if (e.button.button == SDL_BUTTON_RIGHT)
				mousebtns |= MOUSEBTN_RDOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT)
				mousebtns &= ~MOUSEBTN_LDOWN;
			else if (e.button.button == SDL_BUTTON_RIGHT)
				mousebtns &= ~MOUSEBTN_RDOWN;
			break;
		}
	}
}
