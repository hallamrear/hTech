#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <unordered_map>

struct SDL_Texture;

class HTECH_FUNCTION_EXPORT Texture;

class HTECH_FUNCTION_EXPORT TextureCache
{
private:
	TextureCache();

	static TextureCache* mInstance;
	std::unordered_map<std::string, Texture*> mTextures;

	Texture* GetTexture_Internal(const std::string& texture_path);
	void UnloadAll_Impl();

public:
	static TextureCache* Get();
	static Texture* GetTexture(const std::string& texture_path);
	static void UnloadAll();

	~TextureCache();
};

