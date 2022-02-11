#include <assert.h>
#include "Game.h"
#include "Log.h"
#include "Texture.h"

#define HTECH_EXPORTS
#include "pch.h"
#include "TextureCache.h"

TextureCache* TextureCache::mInstance = nullptr;

TextureCache::~TextureCache()
{
	if(mTextures.empty())
	{
		for(auto& itr : mTextures)
		{
			delete itr.second;
			itr.second = nullptr;
		}

		mTextures.clear();
	}
}

Texture* TextureCache::GetTexture_Internal(const std::string& texture_path)
{
	auto itr = mTextures.find(texture_path);

	if(itr == mTextures.end())
	{
		mTextures.insert(std::make_pair(texture_path, new Texture(texture_path)));
		itr = mTextures.find(texture_path);
	}

	assert(itr->second != nullptr);

	return itr->second;
}

TextureCache* TextureCache::Get()
{
	if (mInstance)
		return mInstance;
	else
		mInstance = new TextureCache();

	return mInstance;
}

Texture* TextureCache::GetTexture(const std::string& texture_path)
{
	return Get()->GetTexture_Internal(texture_path);
}

