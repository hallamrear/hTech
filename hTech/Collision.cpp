#include "pch.h"
#include "Collision.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "BoundingPolygon.h"
#include "Entity.h"
#include "Component_Transform.h"
#include "Component_Rigidbody.h"

Collider::Collider(const Transform& transform) : m_EntityTransform(transform), m_Type(COLLIDER_TYPE::COLLIDER_UNKNOWN), m_IsOverlap(false)
{

}

Vector2 Collision::FindClosestPointOnPolygon(const BoundingSphere& circle, const Collider& polygon, const int polygonVertexCount)
{
	Vector2* vertices = new Vector2[polygonVertexCount];
	polygon.GetColliderAsPoints(vertices);

	Vector2 result;
	float minDistance = INFINITY;

	for (int i = 0; i < polygonVertexCount; i++)
	{
		Vector2 v = vertices[i];
		float distance = Vector2(v - circle.GetEntityTransform().Position).GetMagnitude();

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
}

bool Collision::CheckCollision_AABBvsSPHERE(const BoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold)
{
	Vector2 size = one.GetSize();
	
	OrientedBoundingBox obb{ one.GetEntityTransform(), size.X, size.Y };
	return SeperatingAxisTheory_PolygonCircle(4, obb, two, manifold);
}

bool Collision::CheckCollision_SPHEREvsSPHERE(const BoundingSphere& one, const BoundingSphere& two, CollisionManifold* const manifold)
{
	Vector2 p1 = one.GetEntityTransform().Position;
	Vector2 p2 = two.GetEntityTransform().Position;
	Vector2 distance = p2 - p1;
	Vector2 distanceN = distance.GetNormalized();

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
		Vector2 contact = one.GetEntityTransform().Position + distanceN * dtp;
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
	Vector2* shapeOnePoints = new Vector2[shapeOnePointCount];
	one.GetColliderAsPoints(shapeOnePoints);
	Vector2* shapeTwoPoints = new Vector2[shapeTwoPointCount];
	two.GetColliderAsPoints(shapeTwoPoints);

	manifold->Normal = Vector2(FLT_MAX, FLT_MAX);
	manifold->Depth = FLT_MAX;

	//Check shape one in each direction
	for (int a = 0; a < shapeOnePointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeOnePointCount;

		Vector2 axisProj = Vector2(-(shapeOnePoints[b].Y - shapeOnePoints[a].Y), (shapeOnePoints[b].X - shapeOnePoints[a].X));
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

		Vector2 axisProj = Vector2(-(shapeTwoPoints[b].Y - shapeTwoPoints[a].Y), (shapeTwoPoints[b].X - shapeTwoPoints[a].X));
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

	manifold->Depth /= manifold->Normal.GetMagnitude();
	manifold->Normal = manifold->Normal.GetNormalized();

	Vector2 direction = two.GetEntityTransform().Position - one.GetEntityTransform().Position;

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
	Vector2* shapeOnePoints = new Vector2[polygonVertexCount];
	polygonCollider.GetColliderAsPoints(shapeOnePoints);

	Vector2 axisProj = Vector2(0.0f, 0.0f);
	manifold->Normal = Vector2(0.0f, 0.0f);
	manifold->Depth = FLT_MAX;

	//Check shape one in each direction
	for (int a = 0; a <= polygonVertexCount; a++)
	{
		//wraparound
		int b = (a + 1) % polygonVertexCount;

		axisProj = Vector2(-(shapeOnePoints[b].Y - shapeOnePoints[a].Y), (shapeOnePoints[b].X - shapeOnePoints[a].X));
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
		const Vector2 directionScaled = axisProj * circleCollider.Radius;
		const Vector2 p1 = circleCollider.GetEntityTransform().Position + directionScaled;
		const Vector2 p2 = circleCollider.GetEntityTransform().Position - directionScaled;
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

	Vector2 closestPoint = FindClosestPointOnPolygon(circleCollider, polygonCollider, polygonVertexCount);
	axisProj = closestPoint - circleCollider.GetEntityTransform().Position;
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
	Vector2 directionScaled = axisProj * circleCollider.Radius;
	Vector2 p1 = circleCollider.GetEntityTransform().Position + directionScaled;
	Vector2 p2 = circleCollider.GetEntityTransform().Position - directionScaled;
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

	manifold->Depth /= manifold->Normal.GetMagnitude();
	manifold->Normal = manifold->Normal.GetNormalized();

	Vector2 direction = polygonCollider.GetEntityTransform().Position - circleCollider.GetEntityTransform().Position;

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
	Vector2* shapeOnePoints = new Vector2[shapeOnePointCount];
	one.GetColliderAsPoints(shapeOnePoints);
	Vector2* shapeTwoPoints = new Vector2[shapeTwoPointCount];
	two.GetColliderAsPoints(shapeTwoPoints);

	manifold->Normal = Vector2(0.0f, 0.0f);
	manifold->Depth = FLT_MAX;

	//Check shape one in each direction
	for (int a = 0; a < shapeOnePointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeOnePointCount;
		
		Vector2 edge = shapeTwoPoints[b] - shapeTwoPoints[a];

		//Get the axis for projection at 90 to edge
		Vector2 axisProj = Vector2(-edge.Y, edge.X);
		axisProj = axisProj.GetNormalized();

		Vector2 vertex;
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

		Vector2 edge = shapeTwoPoints[b] - shapeTwoPoints[a];

		//Get the axis for projection at 90 to edge
		Vector2 axisProj = Vector2(-edge.Y, edge.X);
		axisProj = axisProj.GetNormalized();

		Vector2 vertex;
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

	Vector2 direction = two.GetEntityTransform().Position - one.GetEntityTransform().Position;

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

bool Collision::CheckCollision_POLYGONvsPOLYGON(const BoundingPolygon& one, const BoundingPolygon& two, CollisionManifold* const manifold)
{
	return SeperatingAxisTheory_PolygonPolygon(one.GetPointCount(), one, two.GetPointCount(), two, manifold);
}

bool Collision::CheckCollision_POLYGONvsOBB(const BoundingPolygon& one, const OrientedBoundingBox& two, CollisionManifold* const manifold)
{
	return SeperatingAxisTheory_PolygonPolygon(one.GetPointCount(), one, 4, two, manifold);
}

//You have to pass in a created collision manifold else itll just return false.
bool Collision::CheckCollision(const Collider& one, const Collider& two, CollisionManifold* const manifold)
{
	if (!manifold)
		return false;

	manifold->HasCollided = false;
	manifold->Depth = 0.0f;
	manifold->Normal = Vector2();
	manifold->ObjA = nullptr;
	manifold->ObjB = nullptr;

	COLLIDER_TYPE colliderOneType = one.GetType();
	COLLIDER_TYPE colliderTwoType = two.GetType();
	
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_AABB && colliderTwoType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsAABB(dynamic_cast<const BoundingBox&>(one), dynamic_cast<const BoundingBox&>(two), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_SPHERE && colliderTwoType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_SPHEREvsSPHERE(dynamic_cast<const BoundingSphere&>(one), dynamic_cast<const BoundingSphere&>(two), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_AABB && colliderTwoType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_AABBvsSPHERE(dynamic_cast<const BoundingBox&>(one), dynamic_cast<const BoundingSphere&>(two), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_SPHERE && colliderTwoType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsSPHERE(dynamic_cast<const BoundingBox&>(two), dynamic_cast<const BoundingSphere&>(one), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_OBB && colliderTwoType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_OBBvsSPHERE(dynamic_cast<const OrientedBoundingBox&>(one), dynamic_cast<const BoundingSphere&>(two), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_SPHERE && colliderTwoType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_OBBvsSPHERE(dynamic_cast<const OrientedBoundingBox&>(two), dynamic_cast<const BoundingSphere&>(one), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_AABB && colliderTwoType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_AABBvsOBB(dynamic_cast<const BoundingBox&>(one), dynamic_cast<const OrientedBoundingBox&>(two), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_OBB && colliderTwoType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsOBB(dynamic_cast<const BoundingBox&>(two), dynamic_cast<const OrientedBoundingBox&>(one), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_OBB && colliderTwoType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_OBBvsOBB(dynamic_cast<const OrientedBoundingBox&>(one), dynamic_cast<const OrientedBoundingBox&>(two), manifold);


	if (colliderOneType == COLLIDER_TYPE::COLLIDER_POLYGON && colliderTwoType == COLLIDER_TYPE::COLLIDER_POLYGON)
		return CheckCollision_POLYGONvsPOLYGON(dynamic_cast<const BoundingPolygon&>(one), dynamic_cast<const BoundingPolygon&>(two), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_POLYGON && colliderTwoType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_POLYGONvsOBB(dynamic_cast<const BoundingPolygon&>(one), dynamic_cast<const OrientedBoundingBox&>(two), manifold);
	if (colliderOneType == COLLIDER_TYPE::COLLIDER_OBB && colliderTwoType == COLLIDER_TYPE::COLLIDER_POLYGON)
		return CheckCollision_POLYGONvsOBB(dynamic_cast<const BoundingPolygon&>(two), dynamic_cast<const OrientedBoundingBox&>(one), manifold);


	//throw;
	return false;
}

void Collision::ResolveCollision(RigidbodyComponent& one, RigidbodyComponent& two, CollisionManifold* const manifold)
{
	if (!manifold)
		return;

	//Do nothing, do not resolve
	//Static - Static
	if (one.GetIsStatic() == true && two.GetIsStatic() == true)
		return;			  

	Vector2 relativeVelocity = Vector2(two.GetVelocity().X - one.GetVelocity().X, two.GetVelocity().Y - one.GetVelocity().Y);
	Vector2 relativeNormal = manifold->Normal.GetNormalized();

	//objects are moving apart
	if (MathsHelp::Dot(relativeVelocity, relativeNormal) > 0.0f)
		return;

	//Equations are based on
	//https://www.chrishecker.com/Rigid_Body_Dynamics

	////Static - Dynamic
	if (one.GetIsStatic() == true && two.GetIsStatic() == false)
	{
		two.GetEntity().GetTransform().Translate(relativeNormal * (manifold->Depth));
		return;
	}
	//Dynamic - Static
	else if (one.GetIsStatic() == false && two.GetIsStatic() == true)
	{
		one.GetEntity().GetTransform().Translate(relativeNormal * (-manifold->Depth));
		return;
	}
	//Dynamic - Dynamic
	//Move both away from each other.
	else
	{
		one.GetEntity().GetTransform().Translate((relativeNormal * -1) * (manifold->Depth / 2.0f));
		two.GetEntity().GetTransform().Translate(relativeNormal * (manifold->Depth / 2.0f));
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

void Collider::RenderProperties()
{
	ImGui::Checkbox("Is Overlap?", &m_IsOverlap);
}

const Transform& Collider::GetEntityTransform() const
{
	return m_EntityTransform;
}

const COLLIDER_TYPE Collider::GetType() const
{
	return m_Type;
}

const bool Collider::IsOverlap() const
{
	return m_IsOverlap;
}

void Collider::Serialize(Serializer& writer) const
{
	writer.String("Is Overlap");
	writer.Bool(m_IsOverlap);
}

void Collider::Deserialize(SerializedValue& value)
{
	auto propertiesMember = value.FindMember("Physics Properties");

	if (propertiesMember->value.IsObject())
	{
		auto properties = propertiesMember->value.GetObjectA();

		auto overlapMember = properties.FindMember("Is Overlap");

		if (overlapMember->value.IsBool())
		{
			m_IsOverlap = overlapMember->value.GetBool();
		}
	}
}
