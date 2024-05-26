#include "pch.h"
#include "Rendering/RenderLayer.h"
#include "Entity/Entity.h"
#include "System/Editor.h"

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
	renderer.StartBatch();
	for (auto& itr : m_Entities)
	{
		itr->Render(renderer);
	}
	renderer.EndBatch();
	renderer.FlushBatch();
}
