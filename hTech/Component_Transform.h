#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Component.h"
#include "Vector2.h"
#include "Transform.h"

class HTECH_FUNCTION_EXPORT TransformComponent : public Component
{
protected:
	Transform mTransform;

public:
	TransformComponent(Entity& entity);
	TransformComponent(Transform transform, Entity& entity);
	~TransformComponent();

	Transform& GetTransform();
};

