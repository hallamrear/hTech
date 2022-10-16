#pragma once
#include "Vector2.h"
#include "Helper.h"
#include <list>
#include <array>
#include <vector>

class BoundingSphere;
class BoundingBox;
class Rigidbody;
class OrientedBoundingBox;
class BoundingPolygon;
struct SDL_Renderer;
class Entity;

class CollisionManifold
{
public:
	bool HasCollided = false;
	float Depth = 0.0f;
	Vector2 Normal;

	Rigidbody* ObjA = nullptr;
	Rigidbody* ObjB = nullptr;
};

enum class COLLIDER_TYPE
{
	COLLIDER_UNKNOWN = -1,
	COLLIDER_AABB = 0,
	COLLIDER_SPHERE,
	COLLIDER_OBB,

	COLLIDER_POLYGON
};

//OBB - Red outline
//AABB - Green outline
//BS - Blue outline
class Collider
{
public:
	Vector2& mOrigin;
	COLLIDER_TYPE mType;
	bool IsOverlap;

	Collider(Vector2& origin) : mOrigin(origin), mType(COLLIDER_TYPE::COLLIDER_UNKNOWN), IsOverlap(false) { }

	virtual void Update(double deltaTime) = 0;
	virtual void Render(SDL_Renderer& renderer) = 0;

	virtual Vector2 FindFurthestPoint(Vector2 direction) const = 0;
	virtual void GetColliderAsPoints(Vector2 points[]) const = 0;
};
 
class Collision
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
	static void ResolveCollision(Rigidbody& one, Rigidbody& two, CollisionManifold* const manifold);
};