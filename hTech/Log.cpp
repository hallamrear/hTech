#include "pch.h"
#include "Text.h"
#include "Camera.h"
#include "Log.h"
#include "Colour.h"
#include "Transform.h"
#include "Console.h"

Log* Log::mInstance = nullptr;

Log::Log()
{
	mMessages = std::vector<std::pair<LogLevel, std::string>>();
	mTextElements = std::vector<Text*>();
	mMessageQueue = std::deque<std::pair<LogLevel, std::string>>();

	for (size_t i = 0; i < Console::Query("MaxLogMessages"); i++)
	{
		mTextElements.push_back(new Text(Vector2(), ""));
		mMessageQueue.emplace_back(LogLevel::LOG_MESSAGE, "");
	}
}

Log::~Log()
{
	mMessages.clear();

	delete mInstance;
	mInstance = nullptr;
}

void Log::LogMessage(LogLevel indicator, const char* str)
{
	Get()->LogMessage_Impl(indicator, std::string(str));
}

void Log::LogMessage(LogLevel indicator, std::string str)
{
	Get()->LogMessage_Impl(indicator, str);
}

void Log::LogMessage_Impl(LogLevel indicator, std::string str)
{
	mMessages.emplace_back(indicator, str);
	mMessageQueue.push_back(std::make_pair(indicator, str));

	if (mMessageQueue.size() > Console::Query("MaxLogMessages"))
		mMessageQueue.pop_front();

	printf("");
}

Log* Log::Get()
{
	if (!mInstance)
		mInstance = new Log();

	return mInstance;
}

void Log::Render_Impl(SDL_Renderer& renderer)
{
	SDL_Rect rect
	{
		50,
		50,
		(int)(Console::Query("WindowDimensionsW")) / 6,
		(int)(Console::Query("WindowDimensionsH")) - 100,
	};

	SDL_Color colour {64, 64, 64, (256 / 4) * 3};
	SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 64);
	SDL_RenderFillRect(&renderer, &rect);
	SDL_SetRenderDrawColor(&renderer, 255, 255, 0, 128);
	SDL_RenderDrawRect(&renderer, &rect);

	float horizontal = (Console::Query("WindowDimensionsW") / 20.0f);
	float vertical = 55;
	int count = Console::Query("MaxLogMessages") - 1;
	for (int i = count; i >= 0; i--)
	{
		vertical += 5;
		vertical += mTextElements[i]->GetTextureSize().Y;
		mTextElements[i]->SetPosition(Vector2(horizontal, vertical));
		mTextElements[i]->Render(renderer);
	}
}

void Log::Update_Impl(float DeltaTime)
{
	//todo : reorder how it updates the list

	for (auto& itr : mTextElements)
		itr->SetString("");

	int loopCount = Console::Query("MaxLogMessages") -1;

	for (int i = loopCount; i >= 0; i--)
	{
		mTextElements[i]->SetString(mMessageQueue[i].second);

		switch (mMessageQueue[i].first)
		{
		case LogLevel::LOG_MESSAGE:
			mTextElements[i]->SetColour(Colour(0, 0, 255, 255));
			break;

		case LogLevel::LOG_ERROR:
			mTextElements[i]->SetColour(Colour(255, 0, 0, 255));
			break;

		case LogLevel::LOG_WARNING:
			mTextElements[i]->SetColour(Colour(255, 255, 0, 255));
			break;

		default:
			break;
		}
	}	

	for (auto& itr : mTextElements)
		itr->Update(DeltaTime);
}

void Log::Render(SDL_Renderer& renderer)
{
	Get()->Render_Impl(renderer);
}

void Log::Update(float DeltaTime)
{
	Get()->Update_Impl(DeltaTime);
}