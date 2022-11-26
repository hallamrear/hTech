#pragma once
#include <map>
class Console
{
private:
	std::unordered_map<std::string, int> mIntMap;

	HWND mTerminalHandle;
	static Console* mInstance;
	static Console* Get();

	Console();
	~Console();

	void ReloadValues_Impl();
	void Run_Impl(std::string command);
	void ParseCommand(std::vector<std::string>& splits);
	void Print(std::string line);
	void Show_Impl();
	void Hide_Impl();
	bool IsVisible_Impl();
	int  Query_Impl(std::string variable);

public:
	static void ReloadValues();
	static void Run(std::string command);
	static int  Query(std::string variable);
	static void Show();
	static void Hide();
	static bool IsVisible();
};