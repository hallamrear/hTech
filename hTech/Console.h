#pragma once
#include <map>
class Console
{
private:
	std::unordered_map<std::string, int> m_IntMap;
	std::unordered_map<std::string, std::function<void()>> m_NonVariableFunctionMap;

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
};