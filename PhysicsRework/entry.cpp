#include "World.h"
#include "SDL.h"
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;

	SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);

	World* world = new World();
	world->Setup();

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 currentTime = 0, deltaTimeMilliseconds = 0, oldTime = 0;
	Uint32 frameTime = 0;
	float DeltaTimeInSeconds = 0.0;
	oldTime = SDL_GetTicks();

	while (true)
	{
		currentTime = SDL_GetTicks();

		//frame delta in miliseconds
		deltaTimeMilliseconds = currentTime - oldTime;

		DeltaTimeInSeconds = deltaTimeMilliseconds / 1000.0f;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			
		}

		if (deltaTimeMilliseconds != 0)
		{
			world->Update(DeltaTimeInSeconds);
			world->Render(renderer);
		}

		frameTime = SDL_GetTicks() - currentTime;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);

		oldTime = currentTime;
	}

	return 0;
}