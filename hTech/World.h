#pragma once
#include "SpatialDivisionMethod.h"
#include "Transform.h"

#define WORLD_TILE_SIZE 256
#define WORLD_TILE_COUNT_X 16
#define WORLD_TILE_COUNT_Y 16

struct SDL_Renderer;
class Entity;
class Text;

#include "Rectangle.h"

class World
{
private:
	std::vector<Entity*> mSelectedEntities;
	Vector2 rectStart, rectEnd;
	bool mIsDraggingRect = false;
	WorldRectangle selectionRect = WorldRectangle(0,0,0,0);

	static World*			mInstance;
	std::vector<Entity*>	mEntityList;
	SpatialHash*			mWorldHashMap;

	Entity*					CreateEntity_Impl(std::string Name = "unnamed", Transform SpawnTransform = Transform(), Entity* Parent = nullptr);
	void					DestroyEntity_Impl(Entity* entity);
	void					ClearupEntities();

	void					Update_Impl(float DeltaTime);
	void					Render_Impl(SDL_Renderer&);
	Entity*					GetEntityByName_Impl(std::string name);
	void					QuerySpaceForEntities_Impl(WorldRectangle rect, std::vector<Entity*>& entities);

protected:
							World();
							~World();
	static World*			Get();

public:
	static void				QuerySpaceForEntities(WorldRectangle rect, std::vector<Entity*>& entities);
	static Entity*			CreateEntity(std::string Name = "unnamed", Transform SpawnTransform = Transform(), Entity* Parent = nullptr);
	static void				DestroyEntity(Entity* entity);

	static Entity*			GetEntityByName(std::string name);

	static void				Update(float DeltaTime);
	static void				Render(SDL_Renderer&);
};

