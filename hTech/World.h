#pragma once

struct SDL_Renderer;
class Entity;
class Text;

class World
{
private:
	static World*			mInstance;
	std::vector<Entity*>	mEntityList;

	Entity*					CreateEntity_Impl(); 
	void					DestroyEntity_Impl(Entity* entity);
	void					ClearupEntities();

	void					Update_Impl(float DeltaTime);
	void					Render_Impl(SDL_Renderer&);

protected:
							World();
							~World();
	static World*			Get();

public:
	static Entity*			CreateEntity();
	static void				DestroyEntity(Entity* entity);

	static void				Update(float DeltaTime);
	static void				Render(SDL_Renderer&);
};

