#include <stdio.h>
#include <SDL.h>

#include "system.h"

#include "big.h"



int main(int argc, char** argv)
{
	printf("eyo?\n");

	if (!InitSystem()) return 1;

	if (!BigInit()) return 2;

	unsigned long long timer_start = SDL_GetPerformanceCounter();
	unsigned long long timer_end = 0;

	float totaltime = 0.0f;

	while (1)
	{
		timer_end = SDL_GetPerformanceCounter();
		float dt = (((float)(timer_end - timer_start)) / (float)SDL_GetPerformanceFrequency());
		totaltime += dt;
		printf("FPS = %f, dt = %f, totaltime = %f, preffreq = %i\n", 1.0f / dt, dt, totaltime, SDL_GetPerformanceFrequency());

		UpdateInputs();
		if (system_inputs & SYSINP_EXIT) break;

		BigUpdate(dt);

		timer_start = SDL_GetPerformanceCounter();

		BigRender();

		UpdateWindow();
	}

	BigQuit();

	return 0;
}
