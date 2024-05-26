#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Entity/Components/Component.h"
#include "DataTypes/Vector2.h"
#include "DataTypes/Transform.h"

class HTECH_FUNCTION_EXPORT TransformComponent : public Component
{
protected:
	Transform m_Transform;
	void RenderProperties();

public:
	TransformComponent(Entity& entity);
	TransformComponent(Transform transform, Entity& entity);
	~TransformComponent();

	Transform GetTransform() const;
	void SetTransform(const Transform& transform);

	void Update(float deltaTime) override;
	void Serialize(Serializer& writer) const override;
	void Deserialize(SerializedValue& value) override;
};

