#include "pch.h"
#include "Console.h"
#include <cstdarg>

Console* Console::mInstance = nullptr;

Console* Console::Get()
{
    if (mInstance == nullptr)
    {
        mInstance = new Console();
    }

    return mInstance;
}

Console::Console()
{
    if (mTerminalHandle == nullptr)
    {
        AllocConsole();
        mTerminalHandle = GetConsoleWindow();
        freopen_s((FILE**)stdin, "CONIN", "r", stdin);
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
    }

    Hide_Impl();

    mIntMap = std::unordered_map<std::string, int>();

    ReloadValues();
}

Console::~Console()
{

}

void Console::Hide_Impl()
{
    if (mTerminalHandle)
    {
        ShowWindow(mTerminalHandle, SW_HIDE);
    }
}

void Console::Show_Impl()
{
    if (mTerminalHandle)
    {
        ShowWindow(mTerminalHandle, SW_SHOW);

        std::cout.clear();
        std::cerr.clear();
        std::cin.clear();
    }
}

void Console::Run(std::string command)
{
    Get()->Run_Impl(command);
}

int Console::Query(std::string variable)
{
    return Get()->Query_Impl(variable);
}

void Console::Show()
{
    Get()->Show_Impl();
}

void Console::Hide()
{
    Get()->Hide_Impl();
}

bool Console::IsVisible()
{
    return Get()->IsVisible_Impl();
}

bool Console::IsVisible_Impl()
{
    return IsWindowVisible(mTerminalHandle) != false;
}

int Console::Query_Impl(std::string variable)
{
    auto found = mIntMap.find(variable);
    if (found != mIntMap.end())
    {
        return found->second;
    }

    Print("Variable not found. Returned 0.");
    return 0;
}

void Console::ReloadValues()
{
    mIntMap.clear();
    mNonVariableFunctions.clear();

    mIntMap.insert(std::make_pair("ShowConsole", 0));
    mIntMap.insert(std::make_pair("DrawLog", 0));
    mIntMap.insert(std::make_pair("WindowDimensionsW", 1280));
    mIntMap.insert(std::make_pair("WindowDimensionsH", 720));
    mIntMap.insert(std::make_pair("DrawColliders", 0));
    mIntMap.insert(std::make_pair("MaxLogMessages", 10));
    mIntMap.insert(std::make_pair("WindowCentreX", 0));
    mIntMap.insert(std::make_pair("WindowCentreY", 0));

    mNonVariableFunctions.insert(std::make_pair("reload", std::bind(&Console::ReloadValues, this)));
    mNonVariableFunctions.insert(std::make_pair("showConsole", std::bind(&Console::Show_Impl, this)));
    mNonVariableFunctions.insert(std::make_pair("hideConsole", std::bind(&Console::Hide_Impl, this)));
}

void Console::Run_Impl(std::string command)
{
    std::string commandToSplit = command;
    std::string space = " ";
    std::vector<std::string> splits;

    size_t lastPos = 0;
    size_t pos = 100000;
    while ((pos = commandToSplit.find(space)) != std::string::npos)
    {
        splits.push_back(commandToSplit.substr(0, pos));
        commandToSplit.erase(0, pos + space.length());
    }
    splits.push_back(commandToSplit);

    ParseCommand(splits);

    return;
}

void Console::ParseCommand(std::vector<std::string>& splits)
{
    std::string command = splits[0];
    splits.erase(splits.begin());

    std::string output = "";

    if (command == "print" || command == "call")
    {
        if (command[0] == 'p')
        {
            output = "Variable " + splits[0] + " : " + std::to_string(Query_Impl(splits[0]));
        }
        else
        {
            std::string funcName = splits[0];

            auto foundFunc = mNonVariableFunctions.find(funcName);
            if (foundFunc != mNonVariableFunctions.end())
            {
                output = command + " to " + funcName + "()";
                foundFunc->second();
            }
            else
            {
                output = "Function '" + splits[0] + "' not found.";
            }
            splits[0];
        }
    }
    else 
    {
        int arg = atoi(splits[0].c_str());

        auto foundItr = mIntMap.find(command);
        if (foundItr != mIntMap.end())
        {
            output = command + " set to " + std::to_string(arg);
            foundItr->second = arg;
        }
        else
        {
            output = "Variable '" + command + "' not found.";
        }
    }

    Print(output);
}

void Console::Print(std::string line)
{
    if (IsVisible_Impl())
    {
        //IMPLEMENT Actual console window.
        std::cout << line;

        if (line.back() != '\n')
        {
            std::cout << std::endl;
        }
    }
}