#include "pch.h"
#include "Console.h"
#include "ImGuiIncludes.h"

Console* Console::m_Instance = nullptr;

Console* Console::Get()
{
    if (m_Instance == nullptr)
    {
        m_Instance = new Console();
    }

    return m_Instance;
}

Console::Console()
{
    m_IntMap = std::unordered_map<std::string, int>();
    mMessageQueue = std::deque<std::pair<LogLevel, std::string>>();

    ReloadValues();
}

Console::~Console()
{
    mMessageQueue.clear();

}

void Console::Hide_Impl()
{
    Run_Impl("DrawLog 0");
}

void Console::Show_Impl()
{
    Run_Impl("DrawLog 1");
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
    return Query_Impl("DrawLog") != 0;
}

int Console::Query_Impl(std::string variable)
{
    auto found = m_IntMap.find(variable);
    if (found != m_IntMap.end())
    {
        return found->second;
    }

    Print("Variable not found. Returned 0.");
    return 0;
}

void Console::ReloadValues()
{
    m_IntMap.clear();
    m_NonVariableFunctionMap.clear();

    m_IntMap.insert(std::make_pair("ShowConsole", 0));
    m_IntMap.insert(std::make_pair("DrawLog", 0));
    m_IntMap.insert(std::make_pair("WindowDimensionsW", 1280));
    m_IntMap.insert(std::make_pair("WindowDimensionsH", 720));
    m_IntMap.insert(std::make_pair("DrawColliders", 0));
    m_IntMap.insert(std::make_pair("DrawHashMap", 0));
    m_IntMap.insert(std::make_pair("MaxLogMessages", 50));
    m_IntMap.insert(std::make_pair("WindowCentreX", 0));
    m_IntMap.insert(std::make_pair("WindowCentreY", 0));

    m_NonVariableFunctionMap.insert(std::make_pair("reload", std::bind(&Console::ReloadValues, this)));
    m_NonVariableFunctionMap.insert(std::make_pair("showConsole", std::bind(&Console::Show_Impl, this)));
    m_NonVariableFunctionMap.insert(std::make_pair("hideConsole", std::bind(&Console::Hide_Impl, this)));
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

            auto foundFunc = m_NonVariableFunctionMap.find(funcName);
            if (foundFunc != m_NonVariableFunctionMap.end())
            {
                output = command + " to " + funcName + "()";
                foundFunc->second();
            }
            else
            {
                output = "Function '" + splits[0] + "' not found.";
            }
        }
    }
    else 
    {
        int arg = atoi(splits[0].c_str());

        auto foundItr = m_IntMap.find(command);
        if (foundItr != m_IntMap.end())
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
    if (line.back() != '\n')
    {
        line += '\n';
    }

    //We pass it a number outside the enum to get the alternative colour in the switch.
    Console::LogMessage(LogLevel::LOG_MESSAGE, line.c_str());
}

void Console::LogMessage(LogLevel indicator, const char* str)
{
    Get()->LogMessage_Impl(indicator, std::string(str));
}

void Console::LogMessage(LogLevel indicator, std::string str)
{
    Get()->LogMessage_Impl(indicator, str);
}

void Console::LogMessage_Impl(LogLevel indicator, std::string str)
{
    mMessageQueue.push_back(std::make_pair(indicator, str));

    if (mMessageQueue.size() > Console::Query("MaxLogMessages"))
        mMessageQueue.pop_front();
}

void Console::Render_Impl(IRenderer& renderer)
{
    ImGui::Begin("Log | Console", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

    ImVec4 colour = ImVec4(0, 0, 0, 0);

    int count = Console::Query("MaxLogMessages");

    if (mMessageQueue.size() < count)
        count = mMessageQueue.size();

    if (count != 0)
    {
        for (int i = 0; i <= count - 1; i++)
        {
            switch (mMessageQueue[i].first)
            {
            case LogLevel::LOG_ERROR:   colour = ImVec4(255, 0, 0, 255); break;
            case LogLevel::LOG_WARNING: colour = ImVec4(255, 255, 0, 255); break;
            case LogLevel::LOG_MESSAGE: colour = ImVec4(255, 255, 255, 255); break;

            default: colour = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); break;

            }

            ImGui::TextColored(colour, mMessageQueue[i].second.c_str());
        }
    }

    ImGui::Separator();
    ImGui::SetNextItemWidth(-FLT_MAX);
    static std::string textInput;
    if (ImGui::InputText("##ConsoleInput", &textInput[0], ImGuiInputTextFlags_EnterReturnsTrue))
    {
        Console::Run(textInput);
        textInput = "";
    }

    ImGui::End();
}

void Console::Update_Impl(float DeltaTime)
{

}

void Console::Render(IRenderer& renderer)
{
    Get()->Render_Impl(renderer);
}

void Console::Update(float DeltaTime)
{
    Get()->Update_Impl(DeltaTime);
}