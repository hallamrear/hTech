#include "pch.h"
#include "Collider.h"
#include "Transform.h"
#include "imgui.h"

Collider::Collider(const Transform& transform) : m_EntityTransform(transform), m_Type(COLLIDER_TYPE::COLLIDER_UNKNOWN), m_IsOverlap(false)
{

}

void Collider::RenderProperties()
{
	ImGui::Checkbox("Is Overlap?", &m_IsOverlap);
}

const Transform& Collider::GetEntityTransform() const
{
	return m_EntityTransform;
}

const COLLIDER_TYPE Collider::GetType() const
{
	return m_Type;
}

const bool Collider::IsOverlap() const
{
	return m_IsOverlap;
}

void Collider::SetIsOverlap(const bool& state)
{
	m_IsOverlap = state;
}

void Collider::Serialize(Serializer& writer) const
{
	writer.String("Is Overlap");
	writer.Bool(m_IsOverlap);
}

void Collider::Deserialize(SerializedValue& value)
{
	auto propertiesMember = value.FindMember("Physics Properties");

	if (propertiesMember->value.IsObject())
	{
		auto properties = propertiesMember->value.GetObjectA();

		auto overlapMember = properties.FindMember("Is Overlap");

		if (overlapMember->value.IsBool())
		{
			m_IsOverlap = overlapMember->value.GetBool();
		}
	}
}
