#pragma once
#include "Manifold.h"
#include "Body.h"
#include "Vector2.h"
#include "World.h"

class Body;
class Manifold;

struct Collision
{
	static bool SeperatingAxisTheory_Original(const Body& bodyA, const Body& bodyB, Manifold* manifold)
	{
		Vector2* shapeOnePoints = new Vector2[4];
		shapeOnePoints[0] = bodyA.TL;
		shapeOnePoints[1] = bodyA.TR;
		shapeOnePoints[2] = bodyA.BR;
		shapeOnePoints[3] = bodyA.BL;
		Vector2* shapeTwoPoints = new Vector2[4];
		shapeTwoPoints[0] = bodyB.TL;
		shapeTwoPoints[1] = bodyB.TR;
		shapeTwoPoints[2] = bodyB.BR;
		shapeTwoPoints[3] = bodyB.BL;

		manifold->Normal = Vector2(FLT_MAX, FLT_MAX);
		manifold->Depth = FLT_MAX;

		//Check shape one in each direction
		for (int a = 0; a < 4; a++)
		{
			//wraparound
			int b = (a + 1) % 4;

			Vector2 axisProj = Vector2(-(shapeOnePoints[b].Y - shapeOnePoints[a].Y), (shapeOnePoints[b].X - shapeOnePoints[a].X));
			axisProj = axisProj.GetNormalized();

			float min_r1 = INFINITY, max_r1 = -INFINITY, min_r2 = INFINITY, max_r2 = -INFINITY;
			for (int P = 0; P < 4; P++)
			{
				//project each point onto line 
				float q_one = shapeOnePoints[P].Dot(axisProj);

				//get the min and max of the projection extents
				min_r1 = std::min(min_r1, q_one);
				max_r1 = std::max(max_r1, q_one);
			}

			for (int P = 0; P < 4; P++)
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

		for (int a = 0; a < 4; a++)
		{
			//wraparound
			int b = (a + 1) % 4;

			Vector2 axisProj = Vector2(-(shapeTwoPoints[b].Y - shapeTwoPoints[a].Y), (shapeTwoPoints[b].X - shapeTwoPoints[a].X));
			axisProj = axisProj.GetNormalized();

			float min_r1 = INFINITY, max_r1 = -INFINITY, min_r2 = INFINITY, max_r2 = -INFINITY;
			for (int P = 0; P < 4; P++)
			{
				//project each point onto line 
				float q_one = shapeOnePoints[P].Dot(axisProj);

				//get the min and max of the projection extents
				min_r1 = std::min(min_r1, q_one);
				max_r1 = std::max(max_r1, q_one);
			}

			for (int P = 0; P < 4; P++)
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

		Vector2 direction = bodyB.Pos - bodyA.Pos;

		if (direction.Dot(manifold->Normal) < 0.0f)
		{
			manifold->Normal = manifold->Normal * -1;
		}

		World::DebugPointsToRenderThisFrame.push_back(bodyA.Pos);
		World::DebugPointsToRenderThisFrame.push_back(bodyB.Pos);

		delete[] shapeOnePoints;
		shapeOnePoints = nullptr;
		delete[] shapeTwoPoints;
		shapeTwoPoints = nullptr;

		manifold->HasCollided = true;
		return manifold->HasCollided;
	};

	static bool SeperatingAxisTheory(const Body& bodyA, const Body& bodyB, Manifold* manifold)
	{

	}

	static bool HasCollided(const Body& bodyA, const Body& bodyB, Manifold* manifold)
	{

	}
};