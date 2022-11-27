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

void TransformComponent::RenderProperties()
{
	float floats[2];
	floats[0] = mTransform.Position.X;
	floats[1] = mTransform.Position.Y;
	ImGui::InputFloat2("Position:", floats);
	mTransform.Position.X = floats[0];
	mTransform.Position.Y = floats[1];
	ImGui::InputFloat("Rotation:", &mTransform.Rotation);
}