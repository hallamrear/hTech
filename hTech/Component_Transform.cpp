#include "pch.h"
#include "Component_Transform.h"

TransformComponent::TransformComponent() : Component("Transform Component")
{
	mTransform.Position = Vector2();
	mTransform.Rotation = 0.0f;
}

TransformComponent::TransformComponent(Transform transform) : Component("Transform Component")
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
