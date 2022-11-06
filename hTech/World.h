#pragma once

struct SDL_Renderer;
class Entity;
class Text;

class World
{
private:
	std::vector<SDL_Point> mWorldDebugGridLayoutPoints;

	static World*			mInstance;
	std::vector<Entity*>	mEntityList;

	Entity*					CreateEntity_Impl(std::string name = "unnamed");
	void					DestroyEntity_Impl(Entity* entity);
	void					ClearupEntities();

	void					Update_Impl(float DeltaTime);
	void					Render_Impl(SDL_Renderer&);
	Entity*					GetEntityByName_Impl(std::string name);

protected:
							World();
							~World();
	static World*			Get();

public:
	static Entity*			CreateEntity(std::string name = "unnamed");
	static void				DestroyEntity(Entity* entity);

	static Entity*			GetEntityByName(std::string name);

	static void				Update(float DeltaTime);
	static void				Render(SDL_Renderer&);
};

