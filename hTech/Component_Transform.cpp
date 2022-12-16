#include "pch.h"
#include "Entity.h"
#include "Component_Transform.h"

TransformComponent::TransformComponent(Entity& entity) : Component("Transform Component", entity)
{
	m_Transform.Position = Vector2();
	m_Transform.Rotation = 0.0f;
}

TransformComponent::TransformComponent(Transform transform, Entity& entity) : Component("Transform Component", entity)
{
	m_Transform = transform;
}

TransformComponent::~TransformComponent()
{
	m_Transform.Position.X = FLT_MAX;
	m_Transform.Position.Y = FLT_MAX;
	m_Transform.Rotation = FLT_MAX;
}

Transform& TransformComponent::GetTransform()
{
	return m_Transform;
}

void TransformComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);

	writer.String("Position");
	writer.StartObject();
	writer.String("X"); writer.Double((double)m_Transform.Position.X);
	writer.String("Y"); writer.Double((double)m_Transform.Position.Y);
	writer.EndObject();

	writer.String("Rotation");
	writer.StartObject();
	writer.String("degrees"); writer.Double((double)m_Transform.Rotation);
	writer.EndObject();
}

void TransformComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);

	auto positionMember = value.FindMember("Position");
	if (positionMember->value.IsObject())
	{
		m_Transform.Position.X = (float)positionMember->value["X"].GetDouble();
		m_Transform.Position.Y = (float)positionMember->value["Y"].GetDouble();		
	}

	auto rotationMember = value.FindMember("Rotation");
	if (rotationMember->value.IsObject())
	{
		m_Transform.Rotation = (float)rotationMember->value["degrees"].GetDouble();
	}
}

void TransformComponent::RenderProperties()
{
	ImGui::Text("Position: ");
	ImGui::SameLine();
	ImGui::InputFloat("##posX:", &m_Transform.Position.X); 
	ImGui::SameLine();
	ImGui::InputFloat("##posY", &m_Transform.Position.Y);
	ImGui::InputFloat("Rotation:", &m_Transform.Rotation);
	if (ImGui::Button("Reset to 0, 0"))
	{
		m_Transform.Position.X = 0.0f;
		m_Transform.Position.Y = 0.0f;
	}
}