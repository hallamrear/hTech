#pragma once
#include "Manifold.h"
#include "Body.h"
#include "Vector2.h"
#include "MathsUtils.h"
#include "World.h"

struct Manifold;

struct CollisionTest
{
	//Largest distance of seperation between plane and polygon vertex.
	float SeperationDistance = FLT_MIN;

	/*
	//index of the face used in the calculation of the seperation distance.
	//	  |		S
	//P   |-------------->
	//	  |
	*/
	int PlaneIndex = -1;

	CollisionTest()
	{

	}

	CollisionTest(float seperationDistance = FLT_MIN, int index = -1)
	{
		SeperationDistance = seperationDistance;
		PlaneIndex = index;
	}
};

struct Collision
{
	struct SATHitData
	{
		Body* EdgeOwner;
		int EdgeID;
		float Depth;
		Vector2 AxisDirection;

		SATHitData(Body* edgeOwner, int edgeid, float depth, Vector2 axisDir)
		{
			EdgeOwner = edgeOwner;
			EdgeID = edgeid;
			Depth = depth;
			AxisDirection = axisDir;
		}
	};

	struct SATResult
	{
		bool HasCollided = false;
		std::vector<SATHitData> hitData = std::vector<SATHitData>();

		SATResult()
		{

		};

		void Clear()
		{
			HasCollided = false;
			hitData.clear();
		}
	};

	static bool PrimVsPrim(Body* bodyA, Body* bodyB, Manifold* manifold)
	{
		std::vector<Vector2>& bodyAVertices = bodyA->m_Vertices;
		std::vector<Vector2>& bodyBVertices = bodyB->m_Vertices;
		std::vector<Edge*>& bodyAEdges = bodyA->m_Edges;
		std::vector<Edge*>& bodyBEdges = bodyB->m_Edges;

		//Test every side of bodyA for a collision.
		for (size_t e = 0; e < bodyAEdges.size(); e++)
		{
			float bodyA_MinimumProjection = INFINITY;
			float bodyA_MaximumProjection = -INFINITY;
			float bodyB_MinimumProjection = INFINITY;
			float bodyB_MaximumProjection = -INFINITY;

			//Construct line from the two points on the bodies edge.
			const Edge& edge = bodyA->GetEdge(e);
			Vector2 axisToProjectOnto = Vector2(*edge.B - *edge.A);
			axisToProjectOnto = edge.GetNormal();
			axisToProjectOnto = axisToProjectOnto.GetNormalized();

			//Project each point of bodyA onto the axis to get the min and max values.
			for (size_t i = 0; i < bodyAVertices.size(); i++)
			{
				Vector2 vertexToProject = bodyAVertices[i];
				float projectedDistanceIntoAxis = MathsUtils::Dot(axisToProjectOnto, vertexToProject);
				bodyA_MinimumProjection = std::min(bodyA_MinimumProjection, projectedDistanceIntoAxis);
				bodyA_MaximumProjection = std::max(bodyA_MaximumProjection, projectedDistanceIntoAxis);
			}

			//Project each point of bodyB onto the axis to get the min and max values.
			for (size_t i = 0; i < bodyBVertices.size(); i++)
			{
				Vector2 vertexToProject = bodyBVertices[i];
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
			if (((bodyB_MaximumProjection >= bodyA_MinimumProjection) && (bodyA_MaximumProjection >= bodyB_MinimumProjection)) == false)
			{
				return false;
			}
			else
			{
				float depth = std::min(bodyB_MaximumProjection - bodyA_MinimumProjection, bodyA_MaximumProjection - bodyB_MinimumProjection);

				if (depth < manifold->Depth)
				{
					manifold->Depth = depth;
					manifold->Normal = axisToProjectOnto;
				}
			}
		}

		// // // // // // // // // // // // //
		// // // // // // // // // // // // //
		// // // // // // // // // // // // //

		//Test every side of bodyB for a collision.
		for (size_t e = 0; e < bodyBEdges.size(); e++)
		{
			float bodyA_MinimumProjection = INFINITY;
			float bodyA_MaximumProjection = -INFINITY;
			float bodyB_MinimumProjection = INFINITY;
			float bodyB_MaximumProjection = -INFINITY;

			//Construct line from the two points on the bodies edge.
			const Edge& edge = bodyB->GetEdge(e);
			Vector2 axisToProjectOnto = Vector2(*edge.B - *edge.A);
			axisToProjectOnto = edge.GetNormal();
			axisToProjectOnto = axisToProjectOnto.GetNormalized();

			//Project each point of bodyA onto the axis to get the min and max values.
			for (size_t i = 0; i < bodyAVertices.size(); i++)
			{
				Vector2 vertexToProject = bodyAVertices[i];
				float projectedDistanceIntoAxis = MathsUtils::Dot(axisToProjectOnto, vertexToProject);
				bodyA_MinimumProjection = std::min(bodyA_MinimumProjection, projectedDistanceIntoAxis);
				bodyA_MaximumProjection = std::max(bodyA_MaximumProjection, projectedDistanceIntoAxis);
			}

			//Project each point of bodyB onto the axis to get the min and max values.
			for (size_t i = 0; i < bodyBVertices.size(); i++)
			{
				Vector2 vertexToProject = bodyBVertices[i];
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
			if (((bodyB_MaximumProjection >= bodyA_MinimumProjection) && (bodyA_MaximumProjection >= bodyB_MinimumProjection)) == false)
			{
				return false;
			}
			else
			{
				float depth = std::min(bodyB_MaximumProjection - bodyA_MinimumProjection, bodyA_MaximumProjection - bodyB_MinimumProjection);

				if (depth < manifold->Depth)
				{
					manifold->Depth = depth;
					manifold->Normal = axisToProjectOnto;
				}
			}
		}

		manifold->HasCollided = true;
		manifold->BodyA_Reference = bodyA;
		manifold->BodyB_Incident = bodyB;

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

		Vector2 direction = manifold->BodyB_Incident->Pos - manifold->BodyA_Reference->Pos;
		if (direction.Dot(manifold->Normal) > 0.0f)
		{
			manifold->Normal = manifold->Normal * -1;
		}

		return true;
	}
};