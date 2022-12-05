#pragma once
#include <stringbuffer.h>
#include <prettywriter.h>

#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

class HTECH_FUNCTION_EXPORT Entity;
struct SDL_Renderer;

class HTECH_FUNCTION_EXPORT Component
{
private:

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
	virtual void RenderProperties() = 0;
	virtual void Serialize(Serializer& writer) const = 0;
	virtual void Deserialize(SerializedValue& value) = 0;
};
