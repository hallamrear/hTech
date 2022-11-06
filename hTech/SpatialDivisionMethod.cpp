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
	index.X = floorf(index.X / (float)WORLD_TILE_SIZE);
	index.Y = floorf(index.Y / (float)WORLD_TILE_SIZE);	
	return index;
}

SpatialHash::SpatialHash(int sizeX, int sizeY)
{
	mSizeX = sizeX;
	mSizeY = sizeY;

	mMap = std::unordered_map<Vector2, HashBucket>();

	//for (int i = 0; i < mSizeX; i++)
	//{
	//	for (int j = 0; j < mSizeY; j++)
	//	{
	//		mMap.insert(std::make_pair<Vector2, HashBucket>(Vector2(i - (mSizeX / 2), j - (mSizeY / 2)), HashBucket()));
	//	}
	//}

	mText = new Text();
}

SpatialHash::~SpatialHash()
{
	mMap.clear();
}

void SpatialHash::Clear()
{
	for (auto& itr : mMap)
	{
		itr.second.Clear();
	}
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
		//mMap.insert(std::make_pair<Vector2, HashBucket>(Vector2(i - (mSizeX / 2), j - (mSizeY / 2)), HashBucket()));
		mMap.insert(std::pair<Vector2, HashBucket>(index, HashBucket()));
		mMap.find(index)->second.Insert(entity);
		//Log::LogMessage(LogLevel::LOG_ERROR, "Error adding an entity to the hash map.");
		//throw;
	}
}

void SpatialHash::Remove(Entity* entity)
{
	Vector2 index = GetIDFromEntity(entity);

	std::unordered_map<Vector2, HashBucket>::iterator foundItem = mMap.find(index);
	if (foundItem != mMap.end())
	{
		foundItem->second.Remove(entity);
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Error removing an entity to the hash map.");
		throw;
	}
}

void SpatialHash::Update(float deltaTime)
{

}

void SpatialHash::Render(SDL_Renderer& renderer)
{
	SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
	for (const auto& itr : mMap)
	{
		Vector2 wPosition = Vector2(itr.first.X * (float)WORLD_TILE_SIZE, (itr.first.Y + 1) * (float)WORLD_TILE_SIZE);
		Vector2 sPosition = Camera::WorldToScreen(wPosition);
		SDL_Rect rect;
		rect.x = sPosition.X;
		rect.y = sPosition.Y;
		rect.w = WORLD_TILE_SIZE;
		rect.h = WORLD_TILE_SIZE;
		SDL_RenderDrawRect(&renderer, &rect);

		if (itr.second.Count() != 0)
		{
			Vector2 offset = wPosition + Vector2(WORLD_TILE_SIZE / 2, -WORLD_TILE_SIZE / 2 + (WORLD_TILE_SIZE / 4));
			mText->SetPosition(Camera::WorldToScreen(offset));
			mText->SetString(std::to_string(itr.second.Count()));
			mText->Update(0.0f);
			mText->Render(renderer);

			std::string str = "";
			offset = wPosition + Vector2(WORLD_TILE_SIZE / 2, -WORLD_TILE_SIZE / 2 - (WORLD_TILE_SIZE / 4));
			mText->SetPosition(Camera::WorldToScreen(offset));
			str = std::to_string((int)itr.first.X) + ", " + std::to_string((int)itr.first.Y);
			mText->SetString(str);
			mText->Update(0.0f);
			mText->Render(renderer);
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

void HashBucket::Clear()
{
	mBucket.clear();
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
