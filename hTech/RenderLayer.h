#pragma once

class Entity;

class RenderLayer
{
protected:
	std::list<Entity*> m_Entities;

public:
	RenderLayer();
	~RenderLayer();

	void AddEntity(Entity& entity);
	void RemoveEntity(Entity& entity);
	void Render(IRenderer& renderer);
};