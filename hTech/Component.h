#pragma once
#include "JSON.h"
#include <string>

#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

class HTECH_FUNCTION_EXPORT Entity;
struct SDL_Renderer;

class HTECH_FUNCTION_EXPORT Component
{
private:

protected:
	bool m_IsEnabled = true;
	std::string m_ComponentName = "Unnamed Component";
	Entity& m_ParentEntity;

	Component(std::string componentName, Entity& parent) : m_ComponentName(componentName), m_ParentEntity(parent) {};


public:
	virtual ~Component() = 0;

	virtual void SetIsEnabled(bool state);
	bool GetIsEnabled() const;
	std::string GetComponentName() const;
	Entity& GetEntity() const;

	virtual void Update(float DeltaTime);
	virtual void Render(SDL_Renderer& renderer);
	virtual void RenderProperties() = 0;
	virtual void Serialize(Serializer& writer) const = 0;
	virtual void Deserialize(SerializedValue& value) = 0;
};
