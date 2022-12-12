#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Collision.h"
#include <vector>

class HTECH_FUNCTION_EXPORT BoundingPolygon :
    public Collider
{
private:
	float& mRotation;
	std::vector<Vector2> mOriginalPoints;
	std::vector<Vector2> mTransformedPoints;

public:
	size_t PointCount = 0;

	BoundingPolygon(Transform& transform, float& rotation, const std::vector<Vector2> points, const int& pointCount);
	BoundingPolygon(Transform& origin);

	virtual void Update(float DeltaTime);
	virtual void Render(SDL_Renderer& renderer);

	virtual Vector2 FindFurthestPoint(Vector2 direction) const;
	virtual void GetColliderAsPoints(Vector2 points[]) const;

	void AddPoint(Vector2 point);

	void Serialize(Serializer& writer) const override;
	void Deserialize(SerializedValue& value) override;
	void RenderProperties() override;
};

