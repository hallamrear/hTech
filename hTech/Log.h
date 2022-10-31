#pragma once
#include <queue>

class BoundingBox;
class MenuObject;
class Text;

enum class LogLevel
{
	LOG_ERROR = 0,
	LOG_WARNING,
	LOG_MESSAGE,
};

class Log
{
private:
	std::vector<Text*> mTextElements;
	static Log* mInstance;

	Log();
	~Log();
	static Log* Get();
	std::vector<std::pair<LogLevel, std::string>> mMessages;
	std::deque<std::pair<LogLevel, std::string>> mMessageQueue;
	void LogMessage_Impl(LogLevel indicator, std::string str);
	void Render_Impl(SDL_Renderer& renderer);
	void Update_Impl(float DeltaTime);

public:
	static void LogMessage(LogLevel indicator, std::string str);
	static void LogMessage(LogLevel indicator, const char* str);

	static void Render(SDL_Renderer& renderer);
	static void Update(float DeltaTime);
};

