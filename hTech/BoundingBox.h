#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Collision.h"

/// <summary>
/// A Bounding box class HTECH_FUNCTION_EXPORT used by rigidbodies for collision detection. None of the collision detection is done using this class.
/// </summary>

class HTECH_FUNCTION_EXPORT BoundingBox
	: public Collider
{
public:
	Vector2 TopLeft;
	Vector2 BottomRight;
	Vector2 Size;

	BoundingBox(Transform& transform, float size_x, float size_y);
	virtual ~BoundingBox();
	virtual void Update(float DeltaTime);
	virtual void Render(SDL_Renderer& renderer);
	virtual void GetColliderAsPoints(Vector2 points[]) const;

	virtual Vector2 FindFurthestPoint(Vector2 direction) const override;
};