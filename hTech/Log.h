#pragma once
#include <string>

 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <queue>

class BoundingBox;
class MenuObject;
class Text;
class IRenderer;

enum class LogLevel
{
	LOG_ERROR = 0,
	LOG_WARNING = 1,
	LOG_MESSAGE = 2,
};

class HTECH_FUNCTION_EXPORT Log
{
private:
	static Log* m_Instance;

	Log();
	~Log();
	static Log* Get();
	std::deque<std::pair<LogLevel, std::string>> mMessageQueue;
	void LogMessage_Impl(LogLevel indicator, std::string str);
	void Render_Impl(IRenderer& renderer);
	void Update_Impl(float DeltaTime);

public:
	static void LogMessage(LogLevel indicator, std::string str);
	static void LogMessage(LogLevel indicator, const char* str);

	static void Render(IRenderer& renderer);
	static void Update(float DeltaTime);
};

