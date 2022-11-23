#include "pch.h"
#include "SpatialDivisionMethod.h"
#include "World.h"
#include "Camera.h"
#include "Log.h"
#include "Entity.h"
#include "Text.h"
#include "Rectangle.h"

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

void SpatialHash::Retrieve(class HTECH_FUNCTION_EXPORT WorldRectangle rect, std::vector<Entity*>& foundEntities)
{
	WorldRectangle reorientedRect = rect;

	if (rect.W < 0)
	{
		reorientedRect.X = rect.X + rect.W;
	}

	reorientedRect.W = abs(rect.W);

	if (rect.H < 0)
	{
		reorientedRect.Y = rect.Y - rect.H;
	}

	reorientedRect.H = abs(rect.H);

	Vector2 points[4]
	{
		{(float)reorientedRect.X,					 (float)reorientedRect.Y},
		{(float)reorientedRect.X + reorientedRect.W, (float)reorientedRect.Y},
		{(float)reorientedRect.X,					 (float)reorientedRect.Y - reorientedRect.H},
		{(float)reorientedRect.X + reorientedRect.W, (float)reorientedRect.Y - reorientedRect.H},
	};

	std::vector<Vector2> IDs = std::vector<Vector2>();

	int step = WORLD_TILE_SIZE / 4;
	for (int x = floorf(points[0].X); x <= points[3].X; x += step)
	{
		for (int y = points[3].Y; y <= points[0].Y; y += step)
		{
			IDs.push_back(
				Vector2(
					floorf(x / (float)WORLD_TILE_SIZE),
					floorf(y / (float)WORLD_TILE_SIZE)
				));
		}
	}

	std::unique(IDs.begin(), IDs.end());

	foundEntities.clear();

	if (IDs.size() > 0)
	{
		for (auto& ID : IDs)
		{
			std::unordered_map<Vector2, HashBucket>::iterator itr = mMap.find(ID);
			if (itr != mMap.end())
			{
				if (itr->second.Count() > 0)
				{
					foundEntities.reserve(foundEntities.size() + itr->second.Count());
					foundEntities.insert(foundEntities.end(), itr->second.GetData().begin(), itr->second.GetData().end());
				}
			}
		}
	}

	size_t count = foundEntities.size();
	for (size_t i = 0; i < count; i++)
	{
		Vector2 position = foundEntities[i]->GetTransform().Position;

		if ((position.X > points[0].X && position.X < points[3].X && position.Y > points[3].Y && position.Y < points[0].Y) == false)
		{
			foundEntities[i] = nullptr;
		}
	}

	foundEntities.erase(
		std::remove_if(
			foundEntities.begin(),
			foundEntities.end(),
			[](Entity const* ptr) { return ptr == nullptr; }
		),
		foundEntities.end()
	);
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
	for (const auto& itr : mMap)
	{
		SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);

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
	Data = std::vector<Entity*>();
}

HashBucket::~HashBucket()
{
	Data.clear();
}

std::vector<Entity*>& HashBucket::GetData()
{
	return Data;
}

size_t HashBucket::Count() const
{
	return Data.size();
}

void HashBucket::Clear()
{
	Data.clear();
}

void HashBucket::Insert(Entity* entity)
{
	Data.push_back(entity);
}

void HashBucket::Remove(Entity* entity)
{
	std::vector<Entity*>::iterator found = std::find(Data.begin(), Data.end(), entity);
	if (found != Data.end())
	{
		Data.erase(found);
	}

	//if it equals .end then it does not exist in this bucket.
}
