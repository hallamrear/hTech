#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Collider.h"

/// <summary>
/// A Bounding box class HTECH_FUNCTION_EXPORT used by rigidbodies for collision detection. None of the collision detection is done using this class.
/// </summary>

class HTECH_FUNCTION_EXPORT BoundingBox
	: public Collider
{
protected:
	Vector2 m_TopLeft;
	Vector2 m_BottomRight;
	Vector2 m_Size;

public:
	const Vector2 GetTopLeft() const;
	const Vector2 GetBottomRight() const;
	const Vector2 GetSize() const;	

	BoundingBox(const Transform& transform, float size_x, float size_y);
	virtual ~BoundingBox();
	virtual void Update(float DeltaTime);
	virtual void Render(IRenderer& renderer);
	virtual void GetColliderAsPoints(Vector2 points[]) const;

	virtual Vector2 FindFurthestPoint(Vector2 direction) const override;

	void Serialize(Serializer& writer) const override;
	void Deserialize(SerializedValue& value) override;
	void RenderProperties() override;
};