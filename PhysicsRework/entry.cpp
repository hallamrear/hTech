#include "World.h"
#include "SDL.h"
#include "Collision.h"
#include <Windows.h>
#include "MathsUtils.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;

	if (SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer) != 0)
	{
		std::string str = SDL_GetError();
	}

	World* world = new World();
	world->Setup();
	world->Renderer = renderer;

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
			switch (event.type)
			{

			case SDL_MOUSEBUTTONDOWN:
			{
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{

				}
				break;

				default:
					break;
				}
			}
			break;

			case SDL_KEYDOWN:
			{
				//event.key.keysym.scancode
				switch (event.key.keysym.sym)
				{
				case SDLK_EQUALS: break;
				case SDLK_MINUS:  break;

				case SDLK_w: world->Bodies.back()->Vel += Vector2(0.0f, 50.0f); break;
				case SDLK_s: world->Bodies.back()->Vel -= Vector2(0.0f, 50.0f); break;
				case SDLK_d: world->Bodies.back()->Vel += Vector2(50.0f, 0.0f); break;
				case SDLK_a: world->Bodies.back()->Vel -= Vector2(50.0f, 0.0f); break;
				case SDLK_q: world->Bodies.back()->AngularVel += MathsUtils::ConvertToRadians(200 * 0.016f); break;
				case SDLK_e: world->Bodies.back()->AngularVel -= MathsUtils::ConvertToRadians(200 * 0.016f); break;
				
				case SDLK_i: world->Bodies.front()->Vel += Vector2(0.0f, 50.0f); break;
				case SDLK_k: world->Bodies.front()->Vel -= Vector2(0.0f, 50.0f); break;
				case SDLK_l: world->Bodies.front()->Vel += Vector2(50.0f, 0.0f); break;
				case SDLK_j: world->Bodies.front()->Vel -= Vector2(50.0f, 0.0f); break;
				case SDLK_u: world->Bodies.front()->AngularVel += MathsUtils::ConvertToRadians(200 * 0.016f); break;
				case SDLK_o: world->Bodies.front()->AngularVel -= MathsUtils::ConvertToRadians(200 * 0.016f); break;

				default:
					break; 
				}
			}
			break;

			default:
				break;
			}
		}

		if (deltaTimeMilliseconds != 0)
		{
			//world->Update(DeltaTimeInSeconds);
			world->Update(0.016f);
			world->Render(renderer);
		}

		frameTime = SDL_GetTicks() - currentTime;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);

		oldTime = currentTime;
	}

	return 0;
}