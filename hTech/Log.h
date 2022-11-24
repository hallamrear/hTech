#pragma once
#include <string>

 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <queue>

class BoundingBox;
class MenuObject;
class Text;
struct SDL_Renderer;

enum class LogLevel
{
	LOG_ERROR = 0,
	LOG_WARNING,
	LOG_MESSAGE,
};

class HTECH_FUNCTION_EXPORT Log
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

