#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <unordered_map>

struct SDL_Texture;

class HTECH_FUNCTION_EXPORT ITexture;

class HTECH_FUNCTION_EXPORT TextureCache
{
private:
	TextureCache();

	static TextureCache* m_Instance;
	std::unordered_map<std::string, ITexture*> m_TextureMap;

	ITexture* GetTexture_Internal(const std::string& texture_path);
	void UnloadAll_Impl();
	void RenderProperties_Impl();

public:
	static TextureCache* Get();
	static ITexture* GetTexture(const std::string& texture_path);
	static void UnloadAll();
	static void RenderProperties();

	~TextureCache();
};

