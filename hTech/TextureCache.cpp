#include "pch.h"
#include <assert.h>
#include "Game.h"
#include "Log.h"
#include "Texture.h"
#include "TextureCache.h"
#include "ProjectLoader.h"
#include "imgui.h"

TextureCache* TextureCache::m_Instance = nullptr;

TextureCache::~TextureCache()
{
	if(m_TextureMap.empty())
	{
		for(auto& itr : m_TextureMap)
		{
			delete itr.second;
			itr.second = nullptr;
		}

		m_TextureMap.clear();
	}
}

TextureCache::TextureCache()
{
	m_TextureMap = std::unordered_map<std::string, Texture*>();
}

Texture* TextureCache::GetTexture_Internal(const std::string& texture_path)
{
	if (texture_path == "")
		return nullptr;

	std::string texturePathStandardised = texture_path;
	
	for (size_t i = 0; i < texture_path.size(); i++)
	{
		texturePathStandardised[i] = tolower(texture_path[i]);
	}

	auto itr = m_TextureMap.find(texturePathStandardised);

	if (itr == m_TextureMap.end())
	{
		std::string fullPath = ""; std::string projectName = ProjectLoader::GetLoadedProjectName();
		ProjectLoader::GetEngineProjectsLocation(fullPath);
		fullPath += (projectName + "\\Assets\\" + texturePathStandardised);

		if (std::filesystem::exists(fullPath))
		{
			m_TextureMap.insert(std::make_pair(texturePathStandardised, new Texture(fullPath, texture_path)));
			itr = m_TextureMap.find(texturePathStandardised);
		}
		else
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "TextureCache -> Error loading texture file at [" + fullPath + "]");
			return nullptr;
		}
	}

	assert(itr->second != nullptr);

	return itr->second;
}

void TextureCache::UnloadAll_Impl()
{
	for (auto itr : m_TextureMap)
	{
		delete itr.second;
		itr.second = nullptr;
	}

	m_TextureMap.clear();
}

TextureCache* TextureCache::Get()
{
	if (m_Instance)
		return m_Instance;
	else
		m_Instance = new TextureCache();

	return m_Instance;
}

Texture* TextureCache::GetTexture(const std::string& texture_path)
{
	return Get()->GetTexture_Internal(texture_path);
}

void TextureCache::UnloadAll()
{
	return Get()->UnloadAll_Impl();
}

void TextureCache::RenderProperties()
{
	return Get()->RenderProperties_Impl();
}

void TextureCache::RenderProperties_Impl()
{
	ImTextureID IMtexture = nullptr;
	
	for (auto& itr : m_TextureMap)
	{
		SDL_Texture* texture = &itr.second->GetSDLTexture();
		IMtexture = (void*)texture;
		ImGui::Image(IMtexture, Vector2(64.0f, 64.0f));
		ImGui::Text(itr.first.c_str());
		texture = nullptr;
	}
	
	IMtexture = nullptr;
}