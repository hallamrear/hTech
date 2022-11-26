#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include <unordered_map>
#include <vector>
#include "Vector2.h"

#define HASH_CLEANUP_COOLDOWN 5.0f

namespace std {
	template <>
	struct hash<Vector2>
	{
		std::size_t operator()(const Vector2& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<float>()(k.X) ^ (hash<float>()(k.Y) << 1)) >> 1);
		};
	};
}

class WorldRectangle;
class Entity;
class Text;

class HTECH_FUNCTION_EXPORT HashBucket
{
private:
	std::vector<Entity*> Data;

public:
	HashBucket();
	~HashBucket();

	std::vector<Entity*>& GetData();
	size_t Count() const;
	void Clear();
	void Insert(Entity* entity);
	void Remove(Entity* entity);
};

class HTECH_FUNCTION_EXPORT SpatialHash
{
private:
	float mCleanupTimeElapsed = 0.0f;
	Text* mText;
	int mSizeX;
	int mSizeY;
	std::unordered_map<Vector2, HashBucket> mMap;
	Vector2 GetIDFromEntity(Entity* entity);

public:
	SpatialHash(int sizeX, int sizeY);
	~SpatialHash();

	void Clear();
	void Retrieve(class WorldRectangle rect, std::vector<Entity*>& foundEntities);
	void Insert(Entity* entity);
	void Remove(Entity* entity);

	void Update(float deltaTime);
	void Render(SDL_Renderer&);
};
