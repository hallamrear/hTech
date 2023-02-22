#include "pch.h"
#include "RenderLayer.h"
#include "Entity.h"

RenderLayer::RenderLayer()
{
}

RenderLayer::~RenderLayer()
{
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
		itr->Render(renderer);
}
