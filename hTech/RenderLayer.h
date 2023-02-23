#pragma once
#include <list>
#include <string>

class Entity;

enum class RENDER_LAYER
{
	BACKGROUND = 0, 
	DEFAULT = 1,
	FOREGROUND = 2,
	UI = 3,
	LAYER_TO_BE_REMOVED_WHEN_I_HOOK_UP_LAYERS_TO_COMPONENT = 4,
	COUNT = 5
};

class RenderLayer
{
protected:
	std::list<Entity*> m_Entities;

public:
	RenderLayer();
	~RenderLayer();

	std::list<Entity*>& GetEntitiesFromLayer();

	void AddEntity(Entity& entity);
	void RemoveEntity(Entity& entity);
	void Render(IRenderer& renderer);
};