#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "pch.h"

#include "JSON.h"
#include "Vector2.h"
#include "Helper.h"
#include "Console.h"
#include "Transform.h"
#include <list>
#include <array>
#include <vector>

class HTECH_FUNCTION_EXPORT BoundingSphere;
class HTECH_FUNCTION_EXPORT BoundingBox;
class HTECH_FUNCTION_EXPORT RigidbodyComponent;
class HTECH_FUNCTION_EXPORT OrientedBoundingBox;
class HTECH_FUNCTION_EXPORT BoundingPolygon;
class HTECH_FUNCTION_EXPORT Entity;
struct SDL_Renderer;

class HTECH_FUNCTION_EXPORT CollisionManifold
{
public:
	bool HasCollided = false;
	float Depth = 0.0f;
	Vector2 Normal;

	RigidbodyComponent* ObjA = nullptr;
	RigidbodyComponent* ObjB = nullptr;
};



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
 
class HTECH_FUNCTION_EXPORT Collision
{
private:
	static Vector2 FindClosestPointOnPolygon(const BoundingSphere& circle, const Collider& polygon, const int polygonVertexCoun);
	static bool CheckCollision_AABBvsAABB(const BoundingBox& one, const BoundingBox& two, CollisionManifold* const manifold);
	static bool CheckCollision_OBBvsSPHERE(const OrientedBoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold);
	static bool CheckCollision_AABBvsSPHERE(const BoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold);
	static bool CheckCollision_SPHEREvsSPHERE(const BoundingSphere& one, const BoundingSphere& two, CollisionManifold* const manifold);
	static bool CheckCollision_OBBvsOBB(const OrientedBoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold);
	static bool CheckCollision_AABBvsOBB(const BoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold);
	static bool SeperatingAxisTheory_PolygonPolygon(const int shapeOnePointCount, const Collider& one, const int shapeTwoPointCount, const Collider& two, CollisionManifold* manifold);
	static bool SeperatingAxisTheory_PolygonCircle(const int shapeOnePointCount, const Collider& polygonCollider, const BoundingSphere& circleCollider, CollisionManifold* manifold);
	static bool SeperatingAxisTheory_Depreciated(const int shapeOnePointCount, const Collider& one, const int shapeTwoPointCount, const Collider& two, CollisionManifold* manifold);

	static bool CheckCollision_POLYGONvsPOLYGON(const BoundingPolygon& one, const BoundingPolygon& two, CollisionManifold* const manifold);
	static bool CheckCollision_POLYGONvsOBB(const BoundingPolygon& one, const OrientedBoundingBox& two, CollisionManifold* const manifold);


public:
	static bool CheckCollision(const Collider& one, const Collider& two, CollisionManifold* manifold);
	static void ResolveCollision(RigidbodyComponent& one, RigidbodyComponent& two, CollisionManifold* const manifold);
};