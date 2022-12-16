#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Collision.h"
#include <vector>

class HTECH_FUNCTION_EXPORT BoundingPolygon :
    public Collider
{
private:
	size_t m_PointCount = 0;
	const float& m_Rotation;
	std::vector<Vector2> m_OriginalPoints;
	std::vector<Vector2> m_TransformedPoints;

public:

	BoundingPolygon(const Transform& transform, const std::vector<Vector2> points, const int& pointCount);
	BoundingPolygon(const Transform& origin);

	virtual void Update(float DeltaTime);
	virtual void Render(SDL_Renderer& renderer);

	virtual Vector2 FindFurthestPoint(Vector2 direction) const;
	virtual void GetColliderAsPoints(Vector2 points[]) const;

	const size_t GetPointCount() const;
	void AddPoint(Vector2 point);

	void Serialize(Serializer& writer) const override;
	void Deserialize(SerializedValue& value) override;
	void RenderProperties() override;
};

