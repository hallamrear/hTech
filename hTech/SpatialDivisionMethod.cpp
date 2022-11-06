#include "pch.h"
#include "SpatialDivisionMethod.h"
#include "Rectangle.h"
#include "World.h"
#include "Camera.h"
#include "Log.h"
#include "Entity.h"
#include "Text.h"

Vector2 SpatialHash::GetIDFromEntity(Entity* entity)
{
	Vector2 index = entity->GetTransform().Position;
	index.X = (int)index.X / WORLD_TILE_SIZE;
	index.Y = (int)index.Y / WORLD_TILE_SIZE;
	return index;
}

SpatialHash::SpatialHash(int sizeX, int sizeY)
{
	mSizeX = sizeX;
	mSizeY = sizeY;

	mMap = std::unordered_map<Vector2, HashBucket>();

	for (int i = 0; i < mSizeX; i++)
	{
		for (int j = 0; j < mSizeY; j++)
		{
			mMap.insert(std::make_pair<Vector2, HashBucket>(Vector2(i - (mSizeX / 2), j - (mSizeY / 2)), HashBucket()));
		}
	}
}

SpatialHash::~SpatialHash()
{
	mMap.clear();
}

void SpatialHash::Retrieve(Rectangle_ rect)
{
}

void SpatialHash::Insert(Entity* entity)
{
	Vector2 index = GetIDFromEntity(entity);

	std::unordered_map<Vector2, HashBucket>::iterator foundItem = mMap.find(index);
	if(foundItem != mMap.end())
	{
		foundItem->second.Insert(entity);
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Error adding an entity to the hash map.");
		throw;
	}
}

void SpatialHash::Remove(Entity* entity)
{

}

void SpatialHash::Update(float deltaTime)
{

}

void SpatialHash::Render(SDL_Renderer& renderer)
{
	SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
	for (const auto& itr : mMap)
	{
		if (itr.second.Count() != 0)
		{
			Vector2 position = Camera::WorldToScreen(Vector2(itr.first.X * (float)WORLD_TILE_SIZE, itr.first.Y * (float)WORLD_TILE_SIZE));
			SDL_Rect rect{ position.X, position.Y , WORLD_TILE_SIZE, WORLD_TILE_SIZE };
			SDL_RenderDrawRect(&renderer, &rect);
		}
	}
}

HashBucket::HashBucket()
{
	mBucket = std::vector<Entity*>();
}

HashBucket::~HashBucket()
{
	mBucket.clear();
}

size_t HashBucket::Count() const
{
	return mBucket.size();
}

void HashBucket::Insert(Entity* entity)
{
	mBucket.push_back(entity);
}

void HashBucket::Remove(Entity* entity)
{
	std::vector<Entity*>::iterator found = std::find(mBucket.begin(), mBucket.end(), entity);
	if (found != mBucket.end())
	{
		mBucket.erase(found);
	}

	//if it equals .end then it does not exist in this bucket.
}
