#pragma once
#include "Component.h"
#include "Vector2.h"
#include "Transform.h"

class TransformComponent : public Component
{
protected:
	Transform mTransform;

public:
	TransformComponent(Entity& entity);
	TransformComponent(Transform transform, Entity& entity);
	~TransformComponent();

	Transform& GetTransform();
};

