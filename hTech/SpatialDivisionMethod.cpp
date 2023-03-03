#include "pch.h"
#include "SpatialDivisionMethod.h"
#include "World.h"
#include "Camera.h"
#include "Log.h"
#include "Entity.h"
#include "Text.h"
#include "IRenderer.h"

Vector2 SpatialHash::GetIDFromEntity(Entity* entity)
{
	Vector2 index = entity->GetTransform().Position;
	index.X = floorf(index.X / (float)WORLD_TILE_SIZE);
	index.Y = floorf(index.Y / (float)WORLD_TILE_SIZE);	
	return index;
}

SpatialHash::SpatialHash(int sizeX, int sizeY)
{
	m_SizeX = sizeX;
	m_SizeY = sizeY;

	m_HashBucketMap = std::unordered_map<Vector2, HashBucket>();

	m_DebugText = new Text();
}

SpatialHash::~SpatialHash()
{
	m_HashBucketMap.clear();
}

void SpatialHash::Clear()
{
	for (auto& itr : m_HashBucketMap)
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
	for (int x = (int)points[0].X; x <= (int)points[3].X; x += step)
	{
		for (int y = (int)points[3].Y; y <= (int)points[0].Y; y += step)
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
			std::unordered_map<Vector2, HashBucket>::iterator itr = m_HashBucketMap.find(ID);
			if (itr != m_HashBucketMap.end())
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
		Vector2 Position = foundEntities[i]->GetTransform().Position;

		if ((Position.X > points[0].X && Position.X < points[3].X && Position.Y > points[3].Y && Position.Y < points[0].Y) == false)
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

	std::unordered_map<Vector2, HashBucket>::iterator foundItem = m_HashBucketMap.find(index);

	if(foundItem != m_HashBucketMap.end())
	{
		foundItem->second.Insert(entity);
	}
	else
	{
		//m_HashBucketMap.insert(std::make_pair<Vector2, HashBucket>(Vector2(i - (m_SizeX / 2), j - (m_SizeY / 2)), HashBucket()));
		m_HashBucketMap.insert(std::pair<Vector2, HashBucket>(index, HashBucket()));
		m_HashBucketMap.find(index)->second.Insert(entity);
		//Log::LogMessage(LogLevel::LOG_ERROR, "Error adding an entity to the hash map.");
		//throw;
	}
}

void SpatialHash::Remove(Entity* entity)
{
	Vector2 index = GetIDFromEntity(entity);

	std::unordered_map<Vector2, HashBucket>::iterator foundItem = m_HashBucketMap.find(index);
	if (foundItem != m_HashBucketMap.end())
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
	m_CleanupTimeElapsed += deltaTime;

	if (m_CleanupTimeElapsed > HASH_CLEANUP_COOLDOWN)
	{
		m_CleanupTimeElapsed = 0.0f;

		for (auto itr = m_HashBucketMap.begin(); itr != m_HashBucketMap.end();)
		{
			if (itr->second.Count() == 0)
			{
				itr = m_HashBucketMap.erase(itr);
			}
			else
				++itr;
		}
	}
}

void SpatialHash::Render(IRenderer& renderer)
{
	for (const auto& itr : m_HashBucketMap)
	{
		renderer.SetPrimativeDrawColour(Colour::Blue);

		Vector2 wPosition = Vector2((itr.first.X * WORLD_TILE_SIZE) + (WORLD_TILE_SIZE / 2), (itr.first.Y * WORLD_TILE_SIZE) + (WORLD_TILE_SIZE / 2));
		WorldRectangle wsRect = WorldRectangle(wPosition.X, wPosition.Y, WORLD_TILE_SIZE, WORLD_TILE_SIZE);
		renderer.Render_WorldSpaceRectangle(wsRect, RENDER_LAYER::FOREGROUND, false);

		if (itr.second.Count() != 0)
		{
			Vector2 offset = wPosition + Vector2(WORLD_TILE_SIZE / 2, -WORLD_TILE_SIZE / 2 + (WORLD_TILE_SIZE / 4));
			m_DebugText->SetPosition(Camera::WorldToScreen(offset));
			m_DebugText->SetString(std::to_string(itr.second.Count()));
			m_DebugText->Update(0.0f);
			m_DebugText->Render(renderer);

			std::string str = "";
			offset = wPosition + Vector2(WORLD_TILE_SIZE / 2, -WORLD_TILE_SIZE / 2 - (WORLD_TILE_SIZE / 4));
			m_DebugText->SetPosition(Camera::WorldToScreen(offset));
			str = std::to_string((int)itr.first.X) + ", " + std::to_string((int)itr.first.Y);
			m_DebugText->SetString(str);
			m_DebugText->Update(0.0f);
			m_DebugText->Render(renderer);
		}
	}
}

HashBucket::HashBucket()
{
	m_Data = std::vector<Entity*>();
}

HashBucket::~HashBucket()
{
	m_Data.clear();
}

std::vector<Entity*>& HashBucket::GetData()
{
	return m_Data;
}

size_t HashBucket::Count() const
{
	return m_Data.size();
}

void HashBucket::Clear()
{
	m_Data.clear();
}

void HashBucket::Insert(Entity* entity)
{
	m_Data.push_back(entity);
}

void HashBucket::Remove(Entity* entity)
{
	std::vector<Entity*>::iterator found = std::find(m_Data.begin(), m_Data.end(), entity);
	if (found != m_Data.end())
	{
		m_Data.erase(found);
	}

	//if it equals .end then it does not exist in this bucket.
}
