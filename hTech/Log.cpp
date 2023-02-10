#include "pch.h"
#include "Text.h"
#include "Camera.h"
#include "Log.h"
#include "Colour.h"
#include "Transform.h"
#include "Console.h"
#include "UI.h"

Log* Log::m_Instance = nullptr;

Log::Log()
{
	mMessages = std::vector<std::pair<LogLevel, std::string>>();
	m_TextElements = std::vector<Text*>();
	mMessageQueue = std::deque<std::pair<LogLevel, std::string>>();
}

Log::~Log()
{
	mMessages.clear();

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
	mMessages.emplace_back(indicator, str);
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

void Log::Render_Impl(SDL_Renderer& renderer)
{
	ImGui::Begin("Log | Console", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImVec4 colour = ImVec4(0, 0, 0, 0);

	int count = Console::Query("MaxLogMessages") - 1;

	//if (m_TextElements.size() < count)
	//{
	//	count = m_TextElements.size();
	//}

	if (count != 0)
	{
		for (int i = count; i >= 0; i--)
		{
			switch (mMessageQueue[i].first)
			{
			case LogLevel::LOG_ERROR:
			{
				colour = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			}
			break;

			case LogLevel::LOG_WARNING:
			{
				colour = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
			}
			break;

			case LogLevel::LOG_MESSAGE:
			{
				colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
			default:
				colour = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
				break;
			}
			ImGui::TextColored(ImVec4(255, 0, 0, 255), mMessageQueue[i].second.c_str());
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
	//todo : reorder how it updates the list

	for (auto& itr : m_TextElements)
		itr->SetString("");

	int loopCount = Console::Query("MaxLogMessages") -1;

	if (m_TextElements.size() < loopCount)
	{
		loopCount = m_TextElements.size();
	}

	if (loopCount != 0)
	{
		for (int i = loopCount; i >= 0; i--)
		{
			if (m_TextElements[i] != nullptr)
			{
				m_TextElements[i]->SetString(mMessageQueue[i].second);

				switch (mMessageQueue[i].first)
				{
				case LogLevel::LOG_MESSAGE:
					m_TextElements[i]->SetColour(Colour(0, 0, 255, 255));
					break;

				case LogLevel::LOG_ERROR:
					m_TextElements[i]->SetColour(Colour(255, 0, 0, 255));
					break;

				case LogLevel::LOG_WARNING:
					m_TextElements[i]->SetColour(Colour(255, 255, 0, 255));
					break;

				default:
					break;
				}
			}
		}
	}

	for (auto& itr : m_TextElements)
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