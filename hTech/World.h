#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "SpatialDivisionMethod.h"
#include "Transform.h"
#include "Entity.h"

#define WORLD_TILE_SIZE 256
#define WORLD_TILE_COUNT_X 16
#define WORLD_TILE_COUNT_Y 16

struct SDL_Renderer;
class HTECH_FUNCTION_EXPORT Entity;
class HTECH_FUNCTION_EXPORT Text;

#include "Rectangle.h"
#include <stringbuffer.h>
#include <prettywriter.h>

class HTECH_FUNCTION_EXPORT World
{
private:
	static World*								m_Instance;
	std::unordered_map<std::string, Entity*>	m_EntityMap;
	SpatialHash*								m_WorldHashMap;

	Entity*					CreateEntity_Impl(std::string Name = "unnamed", Transform SpawnTransform = Transform(), Entity* Parent = nullptr);
	void					DestroyEntity_Impl(Entity* entity);
	void					ClearupDeadEntities();

	void					Update_Impl(float DeltaTime);
	void					Render_Impl(SDL_Renderer&);
	Entity*					GetEntityByName_Impl(std::string name);
	void					QuerySpaceForEntities_Impl(WorldRectangle rect, std::vector<Entity*>& entities);
	Entity*					FindNearestEntityToPosition_Impl(Vector2 WorldPosition);

	void Serialize_Impl(Serializer& writer) const;
	void Deserialize_Impl(Deserializer& reader);
	void ClearAllEntities();

protected:
							World();
							~World();
	static World*			Get();

public:
	static Entity*			FindNearestEntityToPosition(Vector2 WorldPosition);
	static void				QuerySpaceForEntities(WorldRectangle rect, std::vector<Entity*>& entities);
	static Entity*			CreateEntity(std::string Name = "unnamed", Transform SpawnTransform = Transform(), Entity* Parent = nullptr);
	static void				DestroyEntity(Entity* entity);
	static Entity*			GetEntityByName(std::string name);

	static void				Serialize(Serializer& writer);
	static void				Deserialize(Deserializer& reader);

	static void				Update(float DeltaTime);
	static void				Render(SDL_Renderer&);

	static void				UnloadAll();
};