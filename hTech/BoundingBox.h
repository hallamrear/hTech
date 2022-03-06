#pragma once
#include "Collision.h"


/// <summary>
/// A Bounding box class used by rigidbodies for collision detection. None of the collision detection is done using this class.
/// </summary>

class BoundingBox
	: public Collider
{
public:
	Vector2f TopLeft;
	Vector2f BottomRight;
	Vector2f Size;

	BoundingBox(Vector2f& position, float size_x, float size_y);
	virtual ~BoundingBox();
	virtual void Update(double deltaTime);
	virtual void Render(SDL_Renderer& renderer);
	virtual void GetColliderAsPoints(Vector2f points[]) const;

	virtual Vector2f FindFurthestPoint(Vector2f direction) const override;
};