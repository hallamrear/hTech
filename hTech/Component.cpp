#include "pch.h"
#include "Component.h"

Component::~Component()
{
	m_IsEnabled = false;
	m_ComponentName = "";
}

void Component::SetIsEnabled(bool state)
{
	m_IsEnabled = state;
}

bool Component::GetIsEnabled() const
{
	return m_IsEnabled;
}

std::string Component::GetComponentName() const
{
	return m_ComponentName;
}

Entity& Component::GetEntity() const
{
	return m_ParentEntity;
}

void Component::Update(float DeltaTime)
{

}

void Component::Render(IRenderer& renderer)
{

}

void Component::Serialize(Serializer& writer) const
{
	writer.String("Component Name"); writer.String(m_ComponentName.c_str());
	writer.String("IsEnabled");      writer.Bool(m_IsEnabled);
}

void Component::Deserialize(SerializedValue& value)
{
	if (value["IsEnabled"].IsBool())
	{
		bool isEnabled = value["IsEnabled"].GetBool();
		m_IsEnabled = isEnabled;
	}
}