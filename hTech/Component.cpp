#include "pch.h"
#include "Component.h"

Component::~Component()
{
	mIsEnabled = false;
	mComponentName = "";
}

void Component::SetIsEnabled(bool state)
{
	mIsEnabled = state;
}

bool Component::GetIsEnabled() const
{
	return mIsEnabled;
}

std::string Component::GetComponentName() const
{
	return mComponentName;
}

Entity& Component::GetEntity() const
{
	return Parent;
}

void Component::Update(float DeltaTime)
{

}

void Component::Render(SDL_Renderer& renderer)
{

}

void Component::Serialize(Serializer& writer) const
{
	writer.String("Component Name"); writer.String(mComponentName.c_str());
	writer.String("IsEnabled");      writer.Bool(mIsEnabled);
}

void Component::Deserialize(SerializedValue& value)
{
	value["IsEnabled"].IsBool() ? mIsEnabled = value["IsEnabled"].GetBool() : true;
}