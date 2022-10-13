#include "pch.h"
#include "Collision.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "BoundingPolygon.h"
#include "Entity.h"
#include "Rigidbody.h"
   
Vector2f Collision::FindClosestPointOnPolygon(const BoundingSphere& circle, const Collider& polygon, const int polygonVertexCount)
{
	Vector2f* vertices = new Vector2f[polygonVertexCount];
	polygon.GetColliderAsPoints(vertices);

	Vector2f result;
	float minDistance = INFINITY;

	for (int i = 0; i < polygonVertexCount; i++)
	{
		Vector2f v = vertices[i];
		float distance = Vector2f(v - circle.mOrigin).GetMagnitude();

		if (distance < minDistance)
		{
			minDistance = distance;
			result = v;
		}
	}

	delete[] vertices;
	vertices = nullptr;

	return result;
}

bool Collision::CheckCollision_AABBvsAABB(const BoundingBox& one, const BoundingBox& two, CollisionManifold* const manifold)
{
	//Old
	//bool collision = ((one.TopLeft.X <= two.BottomRight.X && one.BottomRight.X >= two.TopLeft.X) && (one.TopLeft.Y <= two.BottomRight.Y && one.BottomRight.Y >= two.TopLeft.Y));

	//we know boxes have 4 points :)
	return Collision::SeperatingAxisTheory_PolygonPolygon(4, one, 4, two, manifold);
}

bool Collision::CheckCollision_OBBvsSPHERE(const OrientedBoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold)
{
	return SeperatingAxisTheory_PolygonCircle(4, one, two, manifold);

	//todo : make sure this shit aint broke
	//Vector2f corners[4];
	//one.GetBoxAsPoints(corners);
	////transform sphere center into obb spaceand perform aabb test
	//Vector2f sphereCentreAABBSpace = two.mOrigin - one.mOrigin;
	//corners[0] -= one.mOrigin;
	//corners[1] -= one.mOrigin;
	//corners[2] -= one.mOrigin;
	//corners[3] -= one.mOrigin;
	////Rotate all around 0,0 by -one.mRotaiton;
	//sphereCentreAABBSpace = RotatePointAroundOriginDegrees(sphereCentreAABBSpace, 360.0f - one.Rotation, Vector2f());
	//corners[0] = RotatePointAroundOriginDegrees(corners[0], 360.0f - one.Rotation, Vector2f());
	//corners[1] = RotatePointAroundOriginDegrees(corners[1], 360.0f - one.Rotation, Vector2f());
	//corners[2] = RotatePointAroundOriginDegrees(corners[2], 360.0f - one.Rotation, Vector2f());
	//corners[3] = RotatePointAroundOriginDegrees(corners[3], 360.0f - one.Rotation, Vector2f());
	//float dist = 0, minimum = INFINITY, maximum = -INFINITY;
	//Vector2f extentsMin, extentsMax;
	//for (int i = 0; i < 3; i++)
	//{
	//	if (corners[i].X < extentsMin.X)
	//		extentsMin.X = corners[i].X;
	//	if (corners[i].Y < extentsMin.Y)
	//		extentsMin.Y = corners[i].Y;
	//	if (corners[i].X > extentsMax.X)
	//		extentsMax.X = corners[i].X;
	//	if (corners[i].Y > extentsMax.Y)
	//		extentsMax.Y = corners[i].Y;
	//}
	////we square it to avoid using square roots for
	////each calculation and can use double radius at the end
	////MIN---------.
	////-			  -
	////-		      -
	////.----------MAX
	//if (sphereCentreAABBSpace.X < extentsMin.X)
	//	dist += pow(extentsMin.X - sphereCentreAABBSpace.X, 2.0f);
	//else if (sphereCentreAABBSpace.X > extentsMax.X)
	//	dist += pow(sphereCentreAABBSpace.X - extentsMax.X, 2.0f);
	//if (sphereCentreAABBSpace.Y < extentsMin.Y)
	//	dist += pow(extentsMin.Y - sphereCentreAABBSpace.Y, 2.0f);
	//else if (sphereCentreAABBSpace.Y > extentsMax.Y)
	//	dist += pow(sphereCentreAABBSpace.Y - extentsMax.Y, 2.0f);
	//return dist <= two.Radius * two.Radius;
}

//todo : make SAT
bool Collision::CheckCollision_AABBvsSPHERE(const BoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold)
{
	//todo : this can be improved by using obb -> aabb and running it here rather than vice versa
	float rotation = 0.0f;
	OrientedBoundingBox obb{ one.mOrigin, rotation, one.Size.X, one.Size.Y };
	return SeperatingAxisTheory_PolygonCircle(4, obb, two, manifold);
}

bool Collision::CheckCollision_SPHEREvsSPHERE(const BoundingSphere& one, const BoundingSphere& two, CollisionManifold* const manifold)
{
	Vector2f p1 = one.mOrigin;
	Vector2f p2 = two.mOrigin;
	Vector2f distance = p2 - p1;
	Vector2f distanceN = distance.GetNormalized();

	// Calculate the sum of the radii, then square it
	float sumRadii = one.Radius + two.Radius;

	if (distance.GetMagnitudeSquared() <= (sumRadii * sumRadii))
	{
		// A and B are touching
		manifold->HasCollided = true;
		manifold->Normal = distanceN;
		manifold->Depth = fabsf(distance.GetMagnitude() - sumRadii) * 0.5f;

		//todo : contact points for sphere/sphere
		float dtp = one.Radius - manifold->Depth;
		Vector2f contact = one.mOrigin + distanceN * dtp;
	}

	return manifold->HasCollided;
};

bool Collision::CheckCollision_OBBvsOBB(const OrientedBoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold)
{
	//we know boxes have 4 points :)
	return Collision::SeperatingAxisTheory_PolygonPolygon(4, one, 4, two, manifold);
}

bool Collision::CheckCollision_AABBvsOBB(const BoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold)
{
	return SeperatingAxisTheory_PolygonPolygon(4, one, 4, two, manifold);
}

bool Collision::SeperatingAxisTheory_PolygonPolygon(const int shapeOnePointCount, const Collider& one, const int shapeTwoPointCount, const Collider& two, CollisionManifold* manifold)
{
	Vector2f* shapeOnePoints = new Vector2f[shapeOnePointCount];
	one.GetColliderAsPoints(shapeOnePoints);
	Vector2f* shapeTwoPoints = new Vector2f[shapeTwoPointCount];
	two.GetColliderAsPoints(shapeTwoPoints);

	manifold->Normal = Vector2f(FLT_MAX, FLT_MAX);
	manifold->Depth = FLT_MAX;

	//Check shape one in each direction
	for (int a = 0; a < shapeOnePointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeOnePointCount;

		Vector2f axisProj = Vector2f(-(shapeOnePoints[b].Y - shapeOnePoints[a].Y), (shapeOnePoints[b].X - shapeOnePoints[a].X));
		axisProj = axisProj.GetNormalized();

		float min_r1 = INFINITY, max_r1 = -INFINITY, min_r2 = INFINITY, max_r2 = -INFINITY;
		for (int P = 0; P < shapeOnePointCount; P++)
		{
			//project each point onto line 
			float q_one = shapeOnePoints[P].Dot(axisProj);

			//get the min and max of the projection extents
			min_r1 = std::min(min_r1, q_one);
			max_r1 = std::max(max_r1, q_one);
		}

		for (int P = 0; P < shapeTwoPointCount; P++)
		{
			//project each point onto line 
			float q_two = shapeTwoPoints[P].Dot(axisProj);

			//get the min and max of the projection extents
			min_r2 = std::min(min_r2, q_two);
			max_r2 = std::max(max_r2, q_two);
		}

		//if they overlap, continue else if they dont, theyre not colliding so can return
		if (!(max_r2 >= min_r1) && (max_r1 >= min_r2))
		{
			delete[] shapeOnePoints;
			shapeOnePoints = nullptr;
			delete[] shapeTwoPoints;
			shapeTwoPoints = nullptr;

			manifold->HasCollided = false;
			return manifold->HasCollided;
		}

		float axisDepth = std::min(max_r2 - min_r1, max_r1 - min_r2);

		if (axisDepth < manifold->Depth)
		{
			manifold->Depth = axisDepth;
			manifold->Normal = axisProj;
		}
	}

	for (int a = 0; a < shapeTwoPointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeTwoPointCount;

		Vector2f axisProj = Vector2f(-(shapeTwoPoints[b].Y - shapeTwoPoints[a].Y), (shapeTwoPoints[b].X - shapeTwoPoints[a].X));
		axisProj = axisProj.GetNormalized();

		float min_r1 = INFINITY, max_r1 = -INFINITY, min_r2 = INFINITY, max_r2 = -INFINITY;
		for (int P = 0; P < shapeOnePointCount; P++)
		{
			//project each point onto line 
			float q_one = shapeOnePoints[P].Dot(axisProj);

			//get the min and max of the projection extents
			min_r1 = std::min(min_r1, q_one);
			max_r1 = std::max(max_r1, q_one);
		}

		for (int P = 0; P < shapeTwoPointCount; P++)
		{
			//project each point onto line 
			float q_two = shapeTwoPoints[P].Dot(axisProj);

			//get the min and max of the projection extents
			min_r2 = std::min(min_r2, q_two);
			max_r2 = std::max(max_r2, q_two);
		}

		//if they overlap, continue else if they dont, theyre not colliding so can return
		if (!(max_r2 >= min_r1) && (max_r1 >= min_r2))
		{
			delete[] shapeOnePoints;
			shapeOnePoints = nullptr;
			delete[] shapeTwoPoints;
			shapeTwoPoints = nullptr;

			manifold->HasCollided = false;
			return manifold->HasCollided;
		}

		float axisDepth = std::min(max_r2 - min_r1, max_r1 - min_r2);

		if (axisDepth < manifold->Depth)
		{
			manifold->Depth = axisDepth;
			manifold->Normal = axisProj;
		}
	}

	//todo : if you uncomment this, remove the axisproj.getnormalized
	manifold->Depth /= manifold->Normal.GetMagnitude();
	manifold->Normal = manifold->Normal.GetNormalized();

	Vector2f direction = two.mOrigin - one.mOrigin;

	if (direction.Dot(manifold->Normal) < 0.0f)
	{
		manifold->Normal = manifold->Normal * -1;
	}

	delete[] shapeOnePoints;
	shapeOnePoints = nullptr;
	delete[] shapeTwoPoints;
	shapeTwoPoints = nullptr;

	manifold->HasCollided = true;
	return manifold->HasCollided;
}

bool Collision::SeperatingAxisTheory_PolygonCircle(const int polygonVertexCount, const Collider& polygonCollider, const BoundingSphere& circleCollider, CollisionManifold* manifold)
{
	Vector2f* shapeOnePoints = new Vector2f[polygonVertexCount];
	polygonCollider.GetColliderAsPoints(shapeOnePoints);

	Vector2f axisProj = Vector2f(0.0f, 0.0f);
	manifold->Normal = Vector2f(0.0f, 0.0f);
	manifold->Depth = FLT_MAX;

	//Check shape one in each direction
	for (int a = 0; a <= polygonVertexCount; a++)
	{
		//wraparound
		int b = (a + 1) % polygonVertexCount;

		axisProj = Vector2f(-(shapeOnePoints[b].Y - shapeOnePoints[a].Y), (shapeOnePoints[b].X - shapeOnePoints[a].X));
		axisProj = axisProj.GetNormalized();

		///Projection bit
		float min_r1 = INFINITY, max_r1 = -INFINITY, min_r2 = INFINITY, max_r2 = -INFINITY;
		for (int P = 0; P < polygonVertexCount; P++)
		{
			//project each point onto line 
			float q_two = shapeOnePoints[P].Dot(axisProj);

			//get the min and max of the projection extents
			min_r2 = std::min(min_r2, q_two);
			max_r2 = std::max(max_r2, q_two);
		}
		///
		//Circle Projection
		Vector2f directionScaled = axisProj * circleCollider.Radius;
		Vector2f p1 = circleCollider.mOrigin + directionScaled;
		Vector2f p2 = circleCollider.mOrigin - directionScaled;
		//get the min and max of the projection extents
		min_r1 = p1.Dot(axisProj);
		max_r1 = p2.Dot(axisProj);

		if (min_r1 > max_r1)
		{
			float t = min_r1;
			min_r1 = max_r1;
			max_r1 = t;
		}
		///

		//if they overlap, continue else if they dont, theyre not colliding so can return
		if (!(max_r2 >= min_r1) && (max_r1 >= min_r2))
		{
			delete[] shapeOnePoints;
			shapeOnePoints = nullptr;

			manifold->HasCollided = false;
			return manifold->HasCollided;
		}

		float axisDepth = std::min(max_r2 - min_r1, max_r1 - min_r2);

		if (axisDepth < manifold->Depth)
		{
			manifold->Depth = axisDepth;
			manifold->Normal = axisProj;
		}
	}

	Vector2f closestPoint = FindClosestPointOnPolygon(circleCollider, polygonCollider, polygonVertexCount);
	axisProj = closestPoint - circleCollider.mOrigin;
	axisProj = axisProj.GetNormalized();

	///Projection bit
	float min_r1 = INFINITY, max_r1 = -INFINITY, min_r2 = INFINITY, max_r2 = -INFINITY;
	for (int P = 0; P < polygonVertexCount; P++)
	{
		//project each point onto line 
		float q_two = shapeOnePoints[P].Dot(axisProj);

		//get the min and max of the projection extents
		min_r2 = std::min(min_r2, q_two);
		max_r2 = std::max(max_r2, q_two);
	}
	///
	//Circle Projection
	Vector2f directionScaled = axisProj * circleCollider.Radius;
	Vector2f p1 = circleCollider.mOrigin + directionScaled;
	Vector2f p2 = circleCollider.mOrigin - directionScaled;
	//get the min and max of the projection extents
	min_r1 = p1.Dot(axisProj);
	max_r1 = p2.Dot(axisProj);

	if (min_r1 > max_r1)
	{
		float t = min_r1;
		min_r1 = max_r1;
		max_r1 = t;
	}
	///

	//if they overlap, continue else if they dont, theyre not colliding so can return
	if (!(max_r2 >= min_r1) && (max_r1 >= min_r2))
	{
		delete[] shapeOnePoints;
		shapeOnePoints = nullptr;

		manifold->HasCollided = false;
		return manifold->HasCollided;
	}

	float axisDepth = std::min(max_r2 - min_r1, max_r1 - min_r2);

	if (axisDepth < manifold->Depth)
	{
		manifold->Depth = axisDepth;
		manifold->Normal = axisProj;
	}

	//todo : if you uncomment this, remove the axisproj.getnormalized
	manifold->Depth /= manifold->Normal.GetMagnitude();
	manifold->Normal = manifold->Normal.GetNormalized();

	Vector2f direction = polygonCollider.mOrigin - circleCollider.mOrigin;

	if (direction.Dot(manifold->Normal) < 0.0f)
	{
		manifold->Normal = manifold->Normal * -1;
	}

	delete[] shapeOnePoints;
	shapeOnePoints = nullptr;

	manifold->HasCollided = true;
	return manifold->HasCollided;
}

bool Collision::SeperatingAxisTheory_Depreciated(const int shapeOnePointCount, const Collider& one, const int shapeTwoPointCount, const Collider& two, CollisionManifold* manifold)
{
	Vector2f* shapeOnePoints = new Vector2f[shapeOnePointCount];
	one.GetColliderAsPoints(shapeOnePoints);
	Vector2f* shapeTwoPoints = new Vector2f[shapeTwoPointCount];
	two.GetColliderAsPoints(shapeTwoPoints);

	manifold->Normal = Vector2f(0.0f, 0.0f);
	manifold->Depth = FLT_MAX;

	//Check shape one in each direction
	for (int a = 0; a < shapeOnePointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeOnePointCount;
		
		Vector2f edge = shapeTwoPoints[b] - shapeTwoPoints[a];

		//Get the axis for projection at 90 to edge
		Vector2f axisProj = Vector2f(-edge.Y, edge.X);
		axisProj = axisProj.GetNormalized();

		Vector2f vertex;
		float minS1 = INFINITY, maxS1 = -INFINITY, minS2 = INFINITY, maxS2 = -INFINITY;
		for (int i = 0; i < shapeOnePointCount; i++)
		{
			vertex = shapeOnePoints[i];
			float projected = vertex.Dot(axisProj);

			minS1 = std::min(minS1, projected);
			maxS1 = std::max(maxS1, projected);
		}

		for (int i = 0; i < shapeTwoPointCount; i++)
		{
			vertex = shapeTwoPoints[i];
			float projected = vertex.Dot(axisProj);

			minS2 = std::min(minS2, projected);
			maxS2 = std::max(maxS2, projected);
		}

		//if they overlap, continue else if they dont, theyre not colliding so can return
		//if (minS1 >= maxS2 || minS2 >= maxS1)
		if (!(maxS2 >= minS1) && (maxS1 >= minS2))
		{
			delete[] shapeOnePoints;
			shapeOnePoints = nullptr;
			delete[] shapeTwoPoints;
			shapeTwoPoints = nullptr;

			manifold->HasCollided = false;
			return manifold->HasCollided;
		}

		float axisDepth = std::min(maxS2 - minS1, maxS1 - minS2);

		if (axisDepth < manifold->Depth)
		{
			manifold->Depth = axisDepth;
			manifold->Normal = axisProj;
		}
	}

	//Check shape two in each direction
	for (int a = 0; a < shapeTwoPointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeTwoPointCount;

		Vector2f edge = shapeTwoPoints[b] - shapeTwoPoints[a];

		//Get the axis for projection at 90 to edge
		Vector2f axisProj = Vector2f(-edge.Y, edge.X);
		axisProj = axisProj.GetNormalized();

		Vector2f vertex;
		float minS1 = INFINITY, maxS1 = -INFINITY, minS2 = INFINITY, maxS2 = -INFINITY;
		for (int i = 0; i < shapeOnePointCount; i++)
		{
			vertex = shapeOnePoints[i];
			float projected = vertex.Dot(axisProj);

			minS1 = std::min(minS1, projected);
			maxS1 = std::max(maxS1, projected);
		}

		for (int i = 0; i < shapeTwoPointCount; i++)
		{
			vertex = shapeTwoPoints[i];
			float projected = vertex.Dot(axisProj);

			minS2 = std::min(minS2, projected);
			maxS2 = std::max(maxS2, projected);
		}

		//if they overlap, continue else if they dont, theyre not colliding so can return
		//if (minS1 >= maxS2 || minS2 >= maxS1) 
		if (!(maxS2 >= minS1) && (maxS1 >= minS2))
		{
			delete[] shapeOnePoints;
			shapeOnePoints = nullptr;
			delete[] shapeTwoPoints;
			shapeTwoPoints = nullptr;

			manifold->HasCollided = false;
			return manifold->HasCollided;
		}

		float axisDepth = std::min(maxS2 - minS1, maxS1 - minS2);

		if (axisDepth < manifold->Depth)
		{
			manifold->Depth = axisDepth;
			manifold->Normal = axisProj;
		}
	}

	//todo: Add method to get centre of all vertices rather than origins
	//e.g. center = FindPointCloudCenterMean(verticesA);
	manifold->Depth /= manifold->Normal.GetMagnitude();
	manifold->Normal = manifold->Normal.GetNormalized();

	Vector2f direction = two.mOrigin - one.mOrigin;

	if (direction.Dot(manifold->Normal) < 0.0f)
	{
		manifold->Normal = manifold->Normal * -1;
	}

	delete[] shapeOnePoints;
	shapeOnePoints = nullptr;
	delete[] shapeTwoPoints;
	shapeTwoPoints = nullptr;

	//todo : construct manifold
	manifold->HasCollided = true;
	return manifold->HasCollided;
}




bool Collision::CheckCollision_POLYGONvsPOLYGON(const BoundingPolygon& one, const BoundingPolygon& two, CollisionManifold* const manifold)
{
	return SeperatingAxisTheory_PolygonPolygon(one.PointCount, one, two.PointCount, two, manifold);
}

bool Collision::CheckCollision_POLYGONvsOBB(const BoundingPolygon& one, const OrientedBoundingBox& two, CollisionManifold* const manifold)
{
	return SeperatingAxisTheory_PolygonPolygon(one.PointCount, one, 4, two, manifold);
}




//You have to pass in a created collision manifold else itll just return false.
bool Collision::CheckCollision(const Collider& one, const Collider& two, CollisionManifold* const manifold)
{
	if (!manifold)
		return false;

	manifold->HasCollided = false;
	manifold->Depth = 0.0f;
	manifold->Normal = Vector2f();
	manifold->ObjA = nullptr;
	manifold->ObjB = nullptr;
	
	//todo : improve
	//1) std::Variant
	//2) 2d array of funciton pointers, indexed by collider_type. most likely out the question

	if (one.mType == COLLIDER_TYPE::COLLIDER_AABB && two.mType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsAABB(dynamic_cast<const BoundingBox&>(one), dynamic_cast<const BoundingBox&>(two), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_SPHERE && two.mType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_SPHEREvsSPHERE(dynamic_cast<const BoundingSphere&>(one), dynamic_cast<const BoundingSphere&>(two), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_AABB && two.mType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_AABBvsSPHERE(dynamic_cast<const BoundingBox&>(one), dynamic_cast<const BoundingSphere&>(two), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_SPHERE && two.mType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsSPHERE(dynamic_cast<const BoundingBox&>(two), dynamic_cast<const BoundingSphere&>(one), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_OBB && two.mType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_OBBvsSPHERE(dynamic_cast<const OrientedBoundingBox&>(one), dynamic_cast<const BoundingSphere&>(two), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_SPHERE && two.mType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_OBBvsSPHERE(dynamic_cast<const OrientedBoundingBox&>(two), dynamic_cast<const BoundingSphere&>(one), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_AABB && two.mType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_AABBvsOBB(dynamic_cast<const BoundingBox&>(one), dynamic_cast<const OrientedBoundingBox&>(two), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_OBB && two.mType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsOBB(dynamic_cast<const BoundingBox&>(two), dynamic_cast<const OrientedBoundingBox&>(one), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_OBB && two.mType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_OBBvsOBB(dynamic_cast<const OrientedBoundingBox&>(one), dynamic_cast<const OrientedBoundingBox&>(two), manifold);


	if (one.mType == COLLIDER_TYPE::COLLIDER_POLYGON && two.mType == COLLIDER_TYPE::COLLIDER_POLYGON)
		return CheckCollision_POLYGONvsPOLYGON(dynamic_cast<const BoundingPolygon&>(one), dynamic_cast<const BoundingPolygon&>(two), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_POLYGON && two.mType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_POLYGONvsOBB(dynamic_cast<const BoundingPolygon&>(one), dynamic_cast<const OrientedBoundingBox&>(two), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_OBB && two.mType == COLLIDER_TYPE::COLLIDER_POLYGON)
		return CheckCollision_POLYGONvsOBB(dynamic_cast<const BoundingPolygon&>(two), dynamic_cast<const OrientedBoundingBox&>(one), manifold);


	//throw;
	return false;
}

void Collision::ResolveCollision(Rigidbody& one, Rigidbody& two, CollisionManifold* const manifold)
{
	if (!manifold)
		return;

	//Do nothing, do not resolve
	//Static - Static
	if (one.GetIsStatic() == true && two.GetIsStatic() == true)
		return;			  

	Vector2f relativeVelocity = Vector2f(two.GetVelocity().X - one.GetVelocity().X, two.GetVelocity().Y - one.GetVelocity().Y);
	Vector2f relativeNormal = manifold->Normal.GetNormalized();

	//objects are moving apart
	if (HelperFunctions::Dot(relativeVelocity, relativeNormal) > 0.0f)
		return;

	//Equations are based on
	//https://www.chrishecker.com/Rigid_Body_Dynamics

	////Static - Dynamic
	if (one.GetIsStatic() == true && two.GetIsStatic() == false)
	{
		two.GetTransform().AdjustPosition(relativeNormal * (manifold->Depth));
		return;
	}
	//Dynamic - Static
	else if (one.GetIsStatic() == false && two.GetIsStatic() == true)
	{
		one.GetTransform().AdjustPosition(relativeNormal * (-manifold->Depth));
		return;
	}
	//Dynamic - Dynamic
	//Move both away from each other.
	else
	{
		one.GetTransform().AdjustPosition((relativeNormal * -1) * (manifold->Depth / 2.0f));
		two.GetTransform().AdjustPosition(relativeNormal * (manifold->Depth / 2.0f));
	}

	/*
	//Get minimum restitution;
	float e = std::min(one.GetRestitution(), two.GetRestitution());
	float impulse = -(1.0f + e) * relativeVelocity.Dot(relativeNormal);
	impulse /= (one.GetInverseMass() + two.GetInverseMass());

	one.AddVelocity(relativeNormal * impulse * -one.GetInverseMass());
	two.AddVelocity(relativeNormal * impulse * two.GetInverseMass());
	*/


	//Get minimum restitution;
	float e = std::min(one.GetRestitution(), two.GetRestitution());
	float impulse = -(1.0f + e) * relativeVelocity.Dot(relativeNormal);
	impulse /= (one.GetInverseMass() + two.GetInverseMass());

	one.AddVelocity(relativeNormal * impulse / one.GetMass());
	two.AddVelocity(relativeNormal * impulse / two.GetMass() * -1);

}