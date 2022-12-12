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

void TransformComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);

	writer.String("Position");
	writer.StartObject();
	writer.String("X"); writer.Double((double)mTransform.Position.X);
	writer.String("Y"); writer.Double((double)mTransform.Position.Y);
	writer.EndObject();

	writer.String("Rotation");
	writer.StartObject();
	writer.String("degrees"); writer.Double((double)mTransform.Rotation);
	writer.EndObject();
}

void TransformComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);

	auto positionMember = value.FindMember("Position");
	if (positionMember->value.IsObject())
	{
		mTransform.Position.X = (float)positionMember->value["X"].GetDouble();
		mTransform.Position.Y = (float)positionMember->value["Y"].GetDouble();		
	}

	auto rotationMember = value.FindMember("Rotation");
	if (rotationMember->value.IsObject())
	{
		mTransform.Rotation = (float)rotationMember->value["degrees"].GetDouble();
	}
}

void TransformComponent::RenderProperties()
{
	ImGui::Text("Position: ");
	ImGui::SameLine();
	ImGui::InputFloat("##posX:", &mTransform.Position.X); 
	ImGui::SameLine();
	ImGui::InputFloat("##posY", &mTransform.Position.Y);
	ImGui::InputFloat("Rotation:", &mTransform.Rotation);
	if (ImGui::Button("Reset to 0, 0"))
	{
		mTransform.Position.X = 0.0f;
		mTransform.Position.Y = 0.0f;
	}
}