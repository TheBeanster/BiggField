#include <stdio.h>
#include <SDL.h>

#include "system.h"



int main(int argc, char** argv)
{
	printf("eyo?\n");

	InitSystem();

	while (1)
	{
		UpdateInputs();
		if (system_inputs & SYSINP_EXIT) break;

		SDL_SetRenderTarget(main_renderer, main_texture);
		SDL_SetRenderDrawColor(main_renderer, 255, 255, 0, 255);
		SDL_RenderClear(main_renderer);

		UpdateWindow();
	}

	return 0;
}
