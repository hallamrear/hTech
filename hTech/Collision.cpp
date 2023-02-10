#include "pch.h"
#include "Collision.h"
#include "Transform.h"
#include "Collider.h"
#include "CollisionManifold.h"
#include "Component_Rigidbody.h"

#define EPA_ITERATIONS 100
#define GJK_MAX_ITERATIONS 500

Vector2 Collision::GJK::GetMinkowskiDifferenceSupportVertex(const Vector2& direction, Collider* bodyA, Collider* bodyB)
{
	Vector2 directionNorm = direction.GetNormalized();
	Vector2 supportA = bodyA->FindFurthestPoint(directionNorm);
	Vector2 oppositeDir = directionNorm * -1;
	Vector2 supportB = bodyB->FindFurthestPoint(oppositeDir);
	return (supportA - supportB);
}

Vector2 Collision::GJK::GetSupportVertexOfSimplex(const Vector2& direction)
{
	Vector2 directionNorm = direction.GetNormalized();

	float furthestDistance = -100000.0f;
	int furthestVertexIndex = -1;
	float projectedDistance = -100000.0f;

	for (size_t i = 0; i < m_SimplexPoints.size(); i++)
	{
		Vector2 vertex = m_SimplexPoints[i];

		projectedDistance = Utils::Maths::Dot(vertex, directionNorm);

		if (projectedDistance >= furthestDistance)
		{
			furthestDistance = projectedDistance;
			furthestVertexIndex = i;
		}
	}

	if (furthestVertexIndex == -1)
	{
		throw std::out_of_range("Furthest Index is -1. Calculation error somewhere here.");
	}

	return m_SimplexPoints[furthestVertexIndex];
}

bool Collision::GJK::IsOverlapping(RigidbodyComponent* bodyA, RigidbodyComponent* bodyB, CollisionManifold* manifold)
{
	m_SimplexPoints.clear();

	bool isOverlapping = DetermineCollisionUsingSimplex(bodyA->GetCollider(), bodyB->GetCollider());

	if (isOverlapping && manifold != nullptr)
	{
		GetCollisionDetails(bodyA, bodyB, manifold);
	}

	return isOverlapping;
}

bool Collision::GJK::AreVectorsInSameDirection(const Vector2& A, const Vector2& B)
{
	return (Utils::Maths::Dot(A, B) > 0);
}

Vector2 Collision::GJK::EPA(Collider* bodyA, Collider* bodyB)
{
	Vector2 MTV = Vector2();
	std::vector<Vector2> simplex = m_SimplexPoints;

	Vector2  minNormal;
	float  minDistance = FLT_MAX;
	size_t minIndex = 0;

	size_t iterations = 0;
	while (minDistance == FLT_MAX)
	{
		iterations++;

		if (iterations > EPA_ITERATIONS)
		{
			break;
		}

		for (size_t i = 0; i < simplex.size(); i++)
		{
			Vector2 a = simplex[i];
			Vector2 b = simplex[(i + 1) % simplex.size()];

			Vector2 ab = b - a;

			Vector2 normal = Vector2(ab.Y, -ab.X).GetNormalized();
			float distance = Utils::Maths::Dot(normal, a);

			if (distance < 0)
			{
				normal *= -1;
				distance *= -1;
			}

			if (distance < minDistance)
			{
				minNormal = normal;
				minDistance = distance;
				minIndex = i;
			}
		}

		Vector2 support = GetMinkowskiDifferenceSupportVertex(minNormal, bodyA, bodyB);
		float sDistance = Utils::Maths::Dot(minNormal, support);

		if (fabsf(sDistance - minDistance) > 0.0000001f)
		{
			minDistance = FLT_MAX;
			simplex.insert(simplex.begin() + minIndex + 1, support);
		}
	}

	if (minDistance == FLT_MAX)
	{
		return Vector2();
	}

	if (abs(minNormal.X) <= 0.01f)
		minNormal.X = 0.0f;
	if (abs(minNormal.Y) <= 0.01f)
		minNormal.Y = 0.0f;

	MTV = minNormal * (minDistance);

	return MTV;
}

void Collision::GJK::GetCollisionDetails(RigidbodyComponent* bodyA, RigidbodyComponent* bodyB, CollisionManifold* manifold)
{
	Vector2 MTV = EPA(bodyA->GetCollider(), bodyB->GetCollider());

	if (MTV != Vector2())
	{
		manifold->BodyA = bodyA;
		manifold->BodyB = bodyB;
		manifold->Depth = MTV.GetMagnitude();
		manifold->Normal = MTV.GetNormalized();
		manifold->HasCollided = true;
		manifold->ContactPoints.push_back(Vector2());
	}
	else
	{
		manifold->HasCollided = false;
		manifold->BodyA = nullptr;
		manifold->BodyB = nullptr;
		manifold->Depth = 0.0f;
		manifold->Normal = Vector2();
		manifold->HasCollided = false;
	}
}

bool Collision::GJK::DetermineCollisionUsingSimplex(Collider* bodyA, Collider* bodyB)
{
	//Set out initial search direciton in the direciton of the second body.
	Vector2 searchDirection = Vector2(bodyB->GetEntityTransform().Position - bodyA->GetEntityTransform().Position);
	if(searchDirection.GetMagnitude() > 0.0001)
		searchDirection = searchDirection.GetNormalized();
	else
		searchDirection = Vector2(0.0f, 1.0f);

	///Get the initial point of the simplex.
	Vector2 supportVertex = GetMinkowskiDifferenceSupportVertex(searchDirection, bodyA, bodyB);
	Vector2 lastVertex;
	m_SimplexPoints.push_back(supportVertex);

	//Get our second search direction for the loop
	searchDirection *= -1;

	int iterations = 0;

	while (true)
	{
		iterations++;

		if (iterations >= GJK_MAX_ITERATIONS)
		{
			return false;
		}

		supportVertex = GetMinkowskiDifferenceSupportVertex(searchDirection, bodyA, bodyB);
		lastVertex = m_SimplexPoints.back();

		//Early exit condition if the new point is not along the search direction
		//aka has returned a point already furthest along in the direction.
		if (Utils::Maths::Dot(supportVertex, searchDirection) <= 0.0f)
		{
			return false;
		}

		m_SimplexPoints.push_back(supportVertex);

		//Iterate through our minkowski differences to build up the simplex.
		if (CalculateNextSimplexPoint(searchDirection))
		{
			return true;
		}
	}

}

bool Collision::GJK::CalculateNextSimplexPoint(Vector2& searchDirection)
{
	switch (m_SimplexPoints.size())
	{
	case 2: //Simplex is currently a line.
	{
		return CalculateSimplexPointFromLine(searchDirection);
	}
	break;

	case 3: //Simplex is currently a triangle.
	{
		return CalculateSimplexPointFromTriangle(searchDirection);
	}
	break;

	default:
		//This should never reach here.
		throw std::out_of_range("Furthest Index is -1. Calculation error somewhere here.");
		return false;
		break;
	}
}

bool Collision::GJK::CalculateSimplexPointFromTriangle(Vector2& searchDirection)
{
	Vector2 a = m_SimplexPoints[0];
	Vector2 b = m_SimplexPoints[1];
	Vector2 c = m_SimplexPoints[2];
	Vector2 ab = (b - a).GetNormalized();				 //a->b
	Vector2 ac = (c - a).GetNormalized();				 //a->c
	Vector2 ao = (-a).GetNormalized();				 //a->o

	Vector2 abf = Utils::Maths::TripleProduct(ac, ab, ab).GetNormalized();
	Vector2 acf = Utils::Maths::TripleProduct(ab, ac, ac).GetNormalized();

	if (AreVectorsInSameDirection(abf, ao))
	{
		m_SimplexPoints.erase(m_SimplexPoints.begin() + 2);
		return CalculateSimplexPointFromLine(searchDirection);
	}

	if (AreVectorsInSameDirection(acf, ao))
	{
		m_SimplexPoints.erase(m_SimplexPoints.begin() + 1);
		return CalculateSimplexPointFromLine(searchDirection);
	}

	return true;
}

bool Collision::GJK::CalculateSimplexPointFromLine(Vector2& searchDirection)
{
	Vector2 a = m_SimplexPoints[0];
	Vector2 b = m_SimplexPoints[1];

	//Vector from a->b
	Vector2 ab = b - a;
	//Vector from a->origin (0,0)
	Vector2 ao = -a;

	if (AreVectorsInSameDirection(ab, ao))
	{
		//If the vectors are in the same direction,
		//get a third vector perpendicular to these
		//to use to get a full triangle.
		searchDirection = Utils::Maths::TripleProduct(ab, ao, ab).GetNormalized();
	}
	else
	{
		m_SimplexPoints.erase(m_SimplexPoints.begin() + 1);
		searchDirection = ao.GetNormalized();
	}

	return false;
}
