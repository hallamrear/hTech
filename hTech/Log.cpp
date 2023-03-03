#include "pch.h"
#include "Text.h"
#include "Camera.h"
#include "Log.h"
#include "Colour.h"
#include "Transform.h"
#include "Console.h"
#include "UI.h"
#include "IRenderer.h"

Log* Log::m_Instance = nullptr;

Log::Log()
{
	mMessageQueue = std::deque<std::pair<LogLevel, std::string>>();
}

Log::~Log()
{
	mMessageQueue.clear();

	delete m_Instance;
	m_Instance = nullptr;
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
	mMessageQueue.push_back(std::make_pair(indicator, str));

	if (mMessageQueue.size() > Console::Query("MaxLogMessages"))
		mMessageQueue.pop_front();
}

Log* Log::Get()
{
	if (!m_Instance)
		m_Instance = new Log();

	return m_Instance;
}

void Log::Render_Impl(IRenderer& renderer)
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
			case LogLevel::LOG_ERROR:   colour = ImVec4(255,   0,   0, 255); break;
			case LogLevel::LOG_WARNING: colour = ImVec4(255, 255,   0, 255); break;
			case LogLevel::LOG_MESSAGE: colour = ImVec4(255, 255, 255, 255); break;

			default: colour = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); break;

			}

			ImGui::TextColored(colour, mMessageQueue[i].second.c_str());
		}
	}

	ImGui::SetNextItemWidth(-FLT_MAX);
	static std::string textInput;
	if(ImGui::InputText("##ConsoleInput", &textInput, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		Console::Run(textInput);
		textInput = "";
	}

	ImGui::End();
}

void Log::Update_Impl(float DeltaTime)
{
	
}

void Log::Render(IRenderer& renderer)
{
	Get()->Render_Impl(renderer);
}

void Log::Update(float DeltaTime)
{
	Get()->Update_Impl(DeltaTime);
}