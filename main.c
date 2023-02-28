#include <stdio.h>
#include <SDL.h>

#include "system.h"

#include "big.h"



int main(int argc, char** argv)
{
	printf("eyo?\n");

	if (!InitSystem()) return 1;

	if (!BigInit()) return 2;

	unsigned int ticks_start = 0;
	unsigned int ticks_end = 0;

	while (1)
	{
		UpdateInputs();
		if (system_inputs & SYSINP_EXIT) break;

		ticks_end = SDL_GetTicks();

		float dt = ((float)(ticks_end - ticks_start)) / 1000.0f;
		printf("dt = %f\n", dt);

		BigUpdate(dt);

		ticks_start = SDL_GetTicks();

		BigRender();

		UpdateWindow();
	}

	BigQuit();

	return 0;
}
