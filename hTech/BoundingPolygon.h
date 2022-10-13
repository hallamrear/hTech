#pragma once
#include "Collision.h"
#include <vector>

class BoundingPolygon :
    public Collider
{
private:
	float& mRotation;
	std::vector<Vector2f> mOriginalPoints;
	std::vector<Vector2f> mTransformedPoints;

public:
	int PointCount = 0;

	BoundingPolygon(Vector2f& origin, float& rotation, const std::vector<Vector2f> points, const int& pointCount);

	virtual void Update(double deltaTime);
	virtual void Render(SDL_Renderer& renderer);

	virtual Vector2f FindFurthestPoint(Vector2f direction) const;
	virtual void GetColliderAsPoints(Vector2f points[]) const;

};

