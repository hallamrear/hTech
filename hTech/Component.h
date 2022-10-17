#pragma once

class Entity;
struct SDL_Renderer;

class Component
{
protected:
	bool mIsEnabled = true;
	std::string mComponentName = "Unnamed Component";
	Entity& Parent;

	Component(std::string componentName, Entity& parent) : mComponentName(componentName), Parent(parent) {};

public:
	virtual ~Component() = 0;

	virtual void SetIsEnabled(bool state);
	bool GetIsEnabled() const;
	std::string GetComponentName() const;
	Entity& GetEntity() const;

	virtual void Update(float DeltaTime);
	virtual void Render(SDL_Renderer& renderer);
};

