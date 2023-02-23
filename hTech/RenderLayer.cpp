#include "pch.h"
#include "RenderLayer.h"
#include "Entity.h"
#include "Editor.h"

RenderLayer::RenderLayer()
{

}

RenderLayer::~RenderLayer()
{
	m_Entities.clear();
}

std::list<Entity*>& RenderLayer::GetEntitiesFromLayer()
{
	return m_Entities;
}

void RenderLayer::AddEntity(Entity& entity)
{
	m_Entities.push_back(&entity);
}

void RenderLayer::RemoveEntity(Entity& entity)
{
	m_Entities.remove(&entity);
}

void RenderLayer::Render(IRenderer& renderer)
{
	for (auto& itr : m_Entities)
	{
		itr->Render(renderer);
	}
}
