#pragma once
#include "Manifold.h"
#include "Body.h"
#include "Vector2.h"
#include "MathsUtils.h"

#define COLLISION_SKIN_DISTANCE 0.1f


class Collision
{
private:
	struct ProjectionResult
	{
		Vector2 Normal;
		float Depth;

		ProjectionResult()
		{
			Normal = Vector2(0.0f, 0.0f);
			Depth = INFINITY;
		}
	};

	static bool GetMinAndMaxProjectionValues(ProjectionResult& result, const Body& bodyToTest, const Body& bodyA, const Body& bodyB)
	{
		size_t testBodyEdgeCount = bodyToTest.m_Edges.size();

		for (size_t i = 0; i < testBodyEdgeCount; i++)
		{
			float bodyA_MinimumProjection = INFINITY;
			float bodyA_MaximumProjection = -INFINITY;
			float bodyB_MinimumProjection = INFINITY;
			float bodyB_MaximumProjection = -INFINITY;

			//Construct line from the two points on the bodies edge.
			const Edge& edge = bodyToTest.GetEdge(i);
			Vector2 axisToProjectOnto = edge.GetNormal().GetNormalized();

			size_t bodyAVertexCount = bodyA.m_TransformedVertices.size();
			//Project each point of bodyA onto the axis to get the min and max values.
			for (size_t a = 0; a < bodyAVertexCount; a++)
			{
				Vector2 vertexToProject = bodyA.m_TransformedVertices[a];
				float projectedDistanceIntoAxis = MathsUtils::Dot(axisToProjectOnto, vertexToProject);
				bodyA_MinimumProjection = std::min(bodyA_MinimumProjection, projectedDistanceIntoAxis);
				bodyA_MaximumProjection = std::max(bodyA_MaximumProjection, projectedDistanceIntoAxis);
			}

			size_t bodyBVertexCount = bodyB.m_TransformedVertices.size();
			//Project each point of bodyB onto the axis to get the min and max values.
			for (size_t b = 0; b < bodyBVertexCount; b++)
			{
				Vector2 vertexToProject = bodyB.m_TransformedVertices[b];
				float projectedDistanceIntoAxis = MathsUtils::Dot(axisToProjectOnto, vertexToProject);
				bodyB_MinimumProjection = std::min(bodyB_MinimumProjection, projectedDistanceIntoAxis);
				bodyB_MaximumProjection = std::max(bodyB_MaximumProjection, projectedDistanceIntoAxis);
			}

			//
			//	-----|---------------------|-----|--------------|-----------------------------------
			//		a_min				b_min   a_max         b_max
			//
			//	if a_max is greater than b_min, theres an overlap
			//
			//	-----|---------------------|-----|--------------|-----------------------------------
			//		b_min				a_min   b_max         a_max
			//
			//	if b_max is greater than a_min, theres an overlap
			//	We check if this is false to see if we can detect a separating axis early
			//  and avoid the rest of the test!
			if ((bodyA_MinimumProjection >= bodyB_MaximumProjection) || (bodyB_MinimumProjection >= bodyA_MaximumProjection))
			{
				return false;
			}
			else
			{
				float depth = std::min(bodyB_MaximumProjection - bodyA_MinimumProjection, bodyA_MaximumProjection - bodyB_MinimumProjection);
				float dOne = bodyB_MaximumProjection - bodyA_MinimumProjection;
				float dTwo = bodyA_MaximumProjection - bodyB_MinimumProjection;

				if (depth < result.Depth)
				{
					result.Depth = depth;
					result.Normal = axisToProjectOnto;
				}
			}
		}

		return true;
	}

public:
	static bool CollisionBreak;

	static bool PolygonVsPolygon(Body* bodyA, Body* bodyB, Manifold& manifold)
	{
		if (Collision::CollisionBreak)
		{
			Collision::CollisionBreak = false;
		}

		ProjectionResult result;
		if (GetMinAndMaxProjectionValues(result, *bodyA, *bodyA, *bodyB) == false)
			return false;

		if (GetMinAndMaxProjectionValues(result, *bodyB, *bodyA, *bodyB) == false)
			return false;

		manifold.HasCollided = true;
		manifold.BodyA = bodyA;
		manifold.BodyB = bodyB;

		manifold.Normal = result.Normal;
		manifold.Depth = result.Depth;

		//If this direction check is not here then an object with fly through half the sides to the other.
		//								  Without the check, the first body with move into side D
		//          ________A_______	  through the second body and out side B.
		//         |                |
		//         |                |
		//         |                |
		//    D    |                |  B
		//         |                |
		//         |                |
		//         |________________|
		//					C

		Vector2 direction = manifold.BodyB->Pos - manifold.BodyA->Pos;
		if (direction.Dot(manifold.Normal) < 0.0f)
		{
			manifold.Normal = manifold.Normal * -1;
		}
			
		//manifold.Depth += 1.0f + (COLLISION_SKIN_DISTANCE);

		//We do not need to find any collision points if both bodies are static. 
		//However we should never get to this point but it's still nice to check.
		if(!(bodyA->IsStatic() && bodyB->IsStatic()))
			FindPolygonContactPoints(manifold, bodyA->m_TransformedVertices, bodyB->m_TransformedVertices);

		return true;
	};

	static void FindPolygonContactPoints(Manifold& manifold,
		const std::vector<Vector2>& verticesA, const std::vector<Vector2>& verticesB)
	{
		const float penTestDistance = 0.0005f;

		manifold.ContactPoints.clear();
		manifold.ContactPoints.push_back(Vector2(INFINITY, INFINITY));
		manifold.ContactPoints.push_back(Vector2(INFINITY, INFINITY));

		float minDistanceSquared = INFINITY;
		int pointCount = 0;

		for (int i = 0; i < verticesA.size(); i++)
		{
			Vector2 p = verticesA[i];

			for (int j = 0; j < verticesB.size(); j++)
			{
				Vector2 va = verticesB[j];
				Vector2 vb = verticesB[(j + 1) % verticesB.size()];

				MathsUtils::ClosestPointDistanceResult result = MathsUtils::FindClosestPointOnLine(p, va, vb);

				if (MathsUtils::AreFloatingPointsWithinTolerence(result.DistanceSquared, minDistanceSquared, penTestDistance))
				{
					if (!MathsUtils::AreTwoPointsWithinTolerence(result.ClosestPoint, manifold.ContactPoints[0], penTestDistance))
					{
						manifold.ContactPoints[1] = result.ClosestPoint;
						pointCount = 2;
					}
				}
				else if (result.DistanceSquared < minDistanceSquared)
				{
					minDistanceSquared = result.DistanceSquared;
					manifold.ContactPoints[0] = result.ClosestPoint;
					pointCount = 1;
				}
			}
		}

		for (int i = 0; i < verticesB.size(); i++)
		{
			Vector2 p = verticesB[i];

			for (int j = 0; j < verticesA.size(); j++)
			{
				Vector2 va = verticesA[j];
				Vector2 vb = verticesA[(j + 1) % verticesA.size()];

				MathsUtils::ClosestPointDistanceResult result = MathsUtils::FindClosestPointOnLine(p, va, vb);

				if (MathsUtils::AreFloatingPointsWithinTolerence(result.DistanceSquared, minDistanceSquared, penTestDistance))
				{
					if (!MathsUtils::AreTwoPointsWithinTolerence(result.ClosestPoint, manifold.ContactPoints[0], penTestDistance))
					{
						manifold.ContactPoints[1] = result.ClosestPoint;
						pointCount = 2;
					}
				}
				else if (result.DistanceSquared < minDistanceSquared)
				{
					minDistanceSquared = result.DistanceSquared;
					manifold.ContactPoints[0] = result.ClosestPoint;
					pointCount = 1;
				}
			}
		}

		if (pointCount == 1)
		{
			manifold.ContactPoints.erase(manifold.ContactPoints.begin() + 1);
		}
	}
};