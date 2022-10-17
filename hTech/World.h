#pragma once

struct SDL_Renderer;
class Entity;
class TextElement;

class World
{
private:
	TextElement*			mSceneGraphText;
	static World*			mInstance;
	std::vector<Entity*>	mEntityList;

	Entity*					CreateEntity_Impl(); 
	void					DestroyEntity_Impl(Entity* entity);
	void					ClearupEntities();

	void					Update_Impl(double);
	void					Render_Impl(SDL_Renderer&);

protected:
							World();
							~World();
	static World*			Get();

public:
	static Entity*			CreateEntity();
	static void				DestroyEntity(Entity* entity);

	static void				Update(double);
	static void				Render(SDL_Renderer&);
};

