#pragma once

struct SDL_Renderer;
class Rigidbody;
class Entity;
class TextElement;

class World
{
private:
	TextElement*			mSceneGraphText;
	static World*			mInstance;
	std::vector<Entity*>	mEntityList;

	//Entity&				CreateEntity_Impl(); 
	//void					DestroyEntity_Impl(Entity* entity);
	Rigidbody&				CreateRigidbody_Impl();
	void					DestroyRigidbody_Impl(Rigidbody* entity);
	void					ClearupEntities();

	void					Update_Impl(double);
	void					Render_Impl(SDL_Renderer&);

protected:
							World();
							~World();
	static World*			Get();

public:
	//static Entity&		CreateEntity();
	//static void			DestroyEntity(Entity& entity);
	static Rigidbody&		CreateRigidbody();
	static void				DestroyRigidbody(Rigidbody& entity);

	static void				Update(double);
	static void				Render(SDL_Renderer&);
};

