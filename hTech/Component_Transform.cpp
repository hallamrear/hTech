#include "pch.h"
#include "Entity.h"
#include "Component_Transform.h"

TransformComponent::TransformComponent(Entity& entity) : Component("Transform Component", entity)
{
	mTransform.Position = Vector2();
	mTransform.Rotation = 0.0f;
}

TransformComponent::TransformComponent(Transform transform, Entity& entity) : Component("Transform Component", entity)
{
	mTransform = transform;
}

TransformComponent::~TransformComponent()
{
	mTransform.Position.X = FLT_MAX;
	mTransform.Position.Y = FLT_MAX;
	mTransform.Rotation = FLT_MAX;
}

Transform& TransformComponent::GetTransform()
{
	return mTransform;
}
