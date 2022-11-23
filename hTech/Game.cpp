#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "World.h"
#include "Log.h"
#include "Settings.h"
#include "Time.h"
#include "PhysicsWorld.h"
#include <iostream>
#include "UI.h"

SDL_Renderer* Game::Renderer = nullptr;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}

int main(int argc, char* argv[])
{
	WindowDetails details;
	details.dimensions = Vector2(1280.0f, 720.0f);
	details.title = "hTech";
	details.position = Vector2(200.0f, 200.0f);

	Game* game = new Game();
	game->Initialise(argc, argv, details);

	if (game->GetIsInitialised())
	{
		game->Start();
	}

	return 0;
}

Game::Game()
{
	mIsInitialised = false;
	mIsRunning = false;
	mWindow = nullptr;
}

Game::~Game()
{
	if (mIsInitialised)
		Shutdown();
}

void Game::Start()
{
	if(GetIsInitialised())
	{
		SetIsRunning(true);

		const int FPS = 60;
		const int frameDelay = 1000 / FPS;
		Uint32 currentTime = 0, deltaTime = 0, oldTime = 0;
		Uint32 frameTime = 0;
		float DeltaTime = 0.0;
		Time::Get(DeltaTime);

		while(GetIsRunning())
		{
			oldTime = SDL_GetTicks();

			while (GetIsRunning())
			{
				currentTime = SDL_GetTicks();
				deltaTime = currentTime - oldTime;

				DeltaTime = deltaTime / 1000.0f;

				if (deltaTime != 0)
				{
					HandleEvents();
					Update(DeltaTime);
					Render();
				}

				frameTime = SDL_GetTicks() - currentTime;
				if (frameDelay > frameTime)
					SDL_Delay(frameDelay - frameTime);

				oldTime = currentTime;
			}
		}
	}
}

/// <summary>
/// Sets the window state and resizes to the enum state passed in.
/// </summary>
/// <param name="state">SCREEN_STATE enum defining either fullscreen, borderless fullscreen or windowed mode.</param>
void Game::SetFullscreen(SCREEN_STATE state)
{
	switch (state)
	{
	case SCREEN_STATE::WINDOW_FULLSCREEN:
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		auto Width = DM.w;
		auto Height = DM.h;
		SDL_SetWindowSize(mWindow, Width, Height);
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN);
	}
		break;
	case SCREEN_STATE::WINDOW_BORDERLESS_FULLSCREEN:
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case SCREEN_STATE::WINDOW_WINDOWED:
		SDL_SetWindowFullscreen(mWindow, 0);
		break;
	}

	int w, h;
	SDL_GetWindowSize(mWindow, &w, &h);
	Settings::Get()->SetWindowDimensions(Vector2((float)w, (float)h));
}

/// <summary>
/// Creates a screenshot of the current frame.
/// </summary>
/// <param name="name">Name of the output file.</param>
void Game::TakeScreenshot(std::string name)
{
	int w, h;
	Vector2 dimensions = Settings::Get()->GetWindowDimensions();
	w = (int)dimensions.X;
	h = (int)dimensions.Y;

	SDL_Surface* sshot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(Game::Renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);

	struct tm _time;
	std::string str = name;
	time_t now = time(nullptr);
	localtime_s(&_time, &now);
	char* buffer = new char[256];
	strftime(buffer, 256, "%d-%m-%Y %H-%M-%S", &_time);

	if (str == "")
	{
		str = buffer;
		str += ".bmp";
	}

	SDL_SaveBMP(sshot, str.c_str());
	SDL_FreeSurface(sshot);

	str = "Screenshot taken: " + str;

	Log::LogMessage(LogLevel::LOG_MESSAGE, str.c_str());
}

void Game::Initialise(int argc, char* argv[], WindowDetails details)
{
	mIsInitialised = (InitialiseSystems(details) && InitialiseApplicationControls());
	if(mIsInitialised == false)
	{
		Shutdown();
	}
}

bool Game::InitialiseWindow(const char* title, int xpos, int ypos, int width, int height, unsigned int flags, bool isFullscreen)
{
	if (mWindow)
		SDL_DestroyWindow(mWindow);
		
	if (title == "")
		title = "No Window Title Given";

	if (xpos == 0)
		xpos = SDL_WINDOWPOS_CENTERED;

	if (ypos == 0)
		ypos = SDL_WINDOWPOS_CENTERED;

	if (width == 0)
		width = 800;

	if (height == 0)
		height = 600;

	if (isFullscreen)
		flags |= SDL_WINDOW_FULLSCREEN;

	mWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

	if (mWindow)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Window created.");
		return true;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Window failed to create.");
		return false;
	}
}

bool Game::InitialiseGraphics()
{
	if (!mWindow || mWindow == nullptr)
		return false;

	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	if (Game::Renderer)
		SDL_DestroyRenderer(Game::Renderer);

	Game::Renderer = SDL_CreateRenderer(mWindow, -1, 0);

	if (Game::Renderer)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Renderer created.");
		return true;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Renderer failed to create.");
		return false;
	}
}

bool Game::InitialiseApplicationControls()
{
	InputManager::Bind(IM_KEY_CODE::IM_KEY_F1, IM_KEY_STATE::IM_KEY_PRESSED,
		[this]
		{
			TakeScreenshot("");
		}); 
	
	InputManager::Bind(IM_KEY_CODE::IM_KEY_F2, IM_KEY_STATE::IM_KEY_PRESSED,
	[this]
	{
			Settings::Get()->SetDrawLog(!Settings::Get()->GetDrawLog());
	});

	return true;
}

bool Game::InitialiseSystems(WindowDetails details)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		if (TTF_Init() < 0)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "Error initializing SDL_ttf");
			Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		}

		Log::LogMessage(LogLevel::LOG_MESSAGE, "Subsystem created.");

		if (InitialiseWindow(details.title.c_str(), (int)details.position.X, (int)details.position.Y, (int)details.dimensions.X, (int)details.dimensions.Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED, false) == false)
			return false;

		Settings::Get()->SetWindowDimensions(details.dimensions);


		int w, h;
		SDL_GetWindowSize(mWindow, &w, &h);
		Settings::Get()->SetWindowDimensions(Vector2((float)w, (float)h));

		if (InitialiseGraphics() == false)
			return false;

		SDL_SetRenderDrawBlendMode(Game::Renderer, SDL_BLENDMODE_BLEND);
			
		return true;
	}
	else
		return false;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(Game::Renderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();

	mIsInitialised = false;
}

void Game::HandleEvents()
{
	SDL_Event event;
	bool hadWheelEvent = false;
	while (SDL_PollEvent(&event))
	{
		if(!hadWheelEvent)
		{
			if (event.type == SDL_MOUSEWHEEL)
			{
				hadWheelEvent = true;

				if (event.wheel.y > 0)
				{
					InputManager::Get()->MouseScrollUpdate(IM_SCROLL_DIRECTION::IM_SCROLL_UP);
				}
				else if (event.wheel.y < 0)
				{
					InputManager::Get()->MouseScrollUpdate(IM_SCROLL_DIRECTION::IM_SCROLL_DOWN);
				}
			}
		}

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				InputManager::Get()->MousePressUpdate(event.button.button, true);
				UI::OnMouseClick();
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				InputManager::Get()->MousePressUpdate(event.button.button, false);
			}
				break;

		case SDL_MOUSEMOTION:
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			InputManager::Get()->MousePositionUpdate(x, y);
			break;
			
		case SDL_QUIT:
			mIsRunning = false;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				int w, h;
				SDL_GetWindowSize(mWindow, &w, &h);
				Settings::Get()->SetWindowDimensions(Vector2((float)w, (float)h));
			}
			break;

		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
				mIsRunning = false;
			else
				InputManager::Get()->KeyPressUpdate(event.key.keysym.sym, true);
			break;

		case SDL_KEYUP:
			InputManager::Get()->KeyPressUpdate(event.key.keysym.sym, false);
			break;

		default:
			break;
		}
	}
}

void Game::Update(float DeltaTime)
{
	InputManager::Update();
	Physics::Update(DeltaTime);
	World::Update(DeltaTime);
	UI::Update(DeltaTime);

	if(Settings::Get()->GetDrawLog())
		Log::Update(DeltaTime);
}

void Game::Render()
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	World::Render(*Renderer);
	UI::Render(*Renderer);

	if (Settings::Get()->GetDrawLog())
		Log::Render(*Game::Renderer);

	SDL_RenderPresent(Game::Renderer);
}
