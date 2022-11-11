#pragma once
#include <unordered_map>
#include <vector>
#include "Vector2.h"

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
class HashBucket;

class SpatialHash
{
private:
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
