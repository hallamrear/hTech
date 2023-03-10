#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <queue>

enum class LogLevel
{
	LOG_ERROR = 0,
	LOG_WARNING = 1,
	LOG_MESSAGE = 2,
};

class Console
{
private:
	std::unordered_map<std::string, int> m_IntMap;
	std::unordered_map<std::string, std::function<void()>> m_NonVariableFunctionMap;
	std::deque<std::pair<LogLevel, std::string>> mMessageQueue;
	void LogMessage_Impl(LogLevel indicator, std::string str);
	void Render_Impl(IRenderer& renderer);
	void Update_Impl(float DeltaTime);

	static Console* m_Instance;
	static Console* Get();

	Console();
	~Console();

	void ReloadValues();
	void Run_Impl(std::string command);
	void ParseCommand(std::vector<std::string>& splits);
	void Print(std::string line);
	void Show_Impl();
	void Hide_Impl();
	bool IsVisible_Impl();
	int  Query_Impl(std::string variable);

public:
	static void Run(std::string command);
	static int  Query(std::string variable);
	static void Show();
	static void Hide();
	static bool IsVisible();

public:
	static void LogMessage(LogLevel indicator, std::string str);
	static void LogMessage(LogLevel indicator, const char* str);

	static void Render(IRenderer& renderer);
	static void Update(float DeltaTime);
};