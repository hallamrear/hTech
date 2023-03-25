#include "pch.h"
#include "FontLoader.h"
#include <SDL_ttf.h>
#include "Console.h"
#include <filesystem>
#include "Engine.h"

FontLoader* FontLoader::m_Instance = nullptr;

FontLoader::FontLoader()
{
	std::filesystem::path p = std::filesystem::path(Engine::GetEXELocation());
	m_DefaultFontLocation = p.parent_path().string() + "\\" + m_DefaultFontLocation;
	m_FontMap = std::unordered_map<int, TTF_Font*>();
}

TTF_Font* FontLoader::GetFont_Impl(const int& fontSize)
{
	auto itr = m_FontMap.find(fontSize);

	//If we find the font at this size, return it.
	if (itr != m_FontMap.end())
	{
		return itr->second;
	}
	else //Create the font at that size.
	{
		TTF_Font* font = TTF_OpenFont(m_DefaultFontLocation.c_str(), (int)fontSize);

		//If the font is good, add it to the map.
		if (font)
		{
			m_FontMap.insert(std::make_pair(fontSize, font));
		}
		else //Cleanup, return nullptr.
		{
			std::string str = TTF_GetError();
			Console::LogMessage(LogLevel::LOG_ERROR, "FAILED TO LOAD FONT");
			Console::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
			TTF_CloseFont(font);
		}
	}

	return nullptr;
}

void FontLoader::UnloadAll_Impl()
{
	for (auto itr : m_FontMap)
	{
		TTF_CloseFont(itr.second);
		delete itr.second;
		itr.second = nullptr;
	}

	m_FontMap.clear();
}

void FontLoader::RenderProperties_Impl()
{
}

FontLoader* FontLoader::Get()
{
	if (!m_Instance)
	{
		m_Instance = new FontLoader();
	}

	return m_Instance;
}

FontLoader::~FontLoader()
{
	if (m_FontMap.empty())
	{
		for (auto& itr : m_FontMap)
		{
			delete itr.second;
			itr.second = nullptr;
		}

		m_FontMap.clear();
	}
}
