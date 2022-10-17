#pragma once
#include "Collision.h"
#include <vector>

class BoundingPolygon :
    public Collider
{
private:
	float& mRotation;
	std::vector<Vector2> mOriginalPoints;
	std::vector<Vector2> mTransformedPoints;

public:
	int PointCount = 0;

	BoundingPolygon(Transform& transform, float& rotation, const std::vector<Vector2> points, const int& pointCount);

	virtual void Update(double deltaTime);
	virtual void Render(SDL_Renderer& renderer);

	virtual Vector2 FindFurthestPoint(Vector2 direction) const;
	virtual void GetColliderAsPoints(Vector2 points[]) const;
};

