#pragma once

class Transform;

//OBB  - Red outline
//AABB - Green outline
//BS   - Blue outline
enum class COLLIDER_TYPE : int
{
	COLLIDER_UNKNOWN = -1,
	COLLIDER_AABB = 0,
	COLLIDER_SPHERE,
	COLLIDER_OBB,
	COLLIDER_POLYGON
};

class HTECH_FUNCTION_EXPORT Collider
{
protected:
	const Transform& m_EntityTransform;
	COLLIDER_TYPE m_Type;
	bool m_IsOverlap;

public:
	Collider(const Transform& transform);

	virtual void Update(float DeltaTime) = 0;
	virtual void Render(SDL_Renderer& renderer) = 0;

	virtual Vector2 FindFurthestPoint(Vector2 direction) const = 0;
	virtual void GetColliderAsPoints(Vector2 points[]) const = 0;

	virtual void Serialize(Serializer& writer) const;
	virtual void Deserialize(SerializedValue& value);
	virtual void RenderProperties();

	const Transform& GetEntityTransform() const;
	const COLLIDER_TYPE GetType() const;
	const bool IsOverlap() const;
};