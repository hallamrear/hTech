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