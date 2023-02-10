#include "pch.h"
#include "ScriptObject.h"

ScriptObject::ScriptObject(Entity* const parent) : m_ParentEntity(parent)
{

}

Entity* const ScriptObject::GetEntity()
{
	return m_ParentEntity;
}

ScriptObject::~ScriptObject()
{

}

void ScriptObject::Start()
{

}

void ScriptObject::OnEnable()
{

}

void ScriptObject::OnDisable()
{

}

void ScriptObject::OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other)
{

}

void ScriptObject::OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other)
{

}