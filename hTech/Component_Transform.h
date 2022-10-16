#pragma once
#include "Component.h"
#include "Vector2.h"
#include "Transform.h"

class TransformComponent : public Component
{
protected:
	Transform mTransform;

public:
	TransformComponent();
	TransformComponent(Transform transform);
	~TransformComponent();

	Transform& GetTransform();
};

