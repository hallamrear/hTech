#pragma once
#include "Manifold.h"
#include "Body.h"
#include "Vector2.h"
#include "MathsUtils.h"
#include <stdexcept>

namespace Collision
{
	//Implementation of the GJK algorithm for collision detection.
	class GJK
	{
	private:
		Vector2 m_SupportDirection = Vector2();
		std::vector<Vector2> m_Vertices = std::vector<Vector2>();

		bool GJK_AddSupportToTriangle(const Vector2& direction, const Body& bodyA, const Body& bodyB)
		{
			Vector2 vertex = bodyA.GetSupportVertex(direction) - bodyB.GetSupportVertex(direction * -1);
			if(MathsUtils::Dot(direction, vertex) > 0)
			{
				m_Vertices.push_back(vertex);
				return true;
			}

			return false;
		}

	public:
		enum class SimplexEvolution
		{
			NoIntersection,
			StillEvolving,
			FoundIntersection
		};

		GJK::SimplexEvolution EvolveSimplex(const Body& bodyA, const Body& bodyB)
		{
			switch (m_Vertices.size())
			{
			case 0:
			{
				//Get the first triangle point from the direction of action.
				m_SupportDirection = bodyB.Pos - bodyA.Pos;
				m_SupportDirection = m_SupportDirection.GetNormalized();
			}
			break;

			case 1:
			{
				//Get our second sample triangle point from the furthest in the
				//opposite direction to maximise polygon coverage.
				m_SupportDirection *= -1;
			}
			break;

			case 2:
			{
				//Get the edge of the triangle.
				Vector2 b = m_Vertices[1];
				Vector2 c = m_Vertices[0];

				//Line between the first two vertices to make the edge.
				Vector2 cb = b - c;
				//Line from the first vertex to the origin.
				Vector2 c0 = c *= -1;

				//Use the triple cross product to calculate a direction
				//perpendicular to the line cb in the direction of origin
				//to get the final triangle point in this direction.
				m_SupportDirection = MathsUtils::TripleProduct(cb.GetNormalized(), c0.GetNormalized(), cb.GetNormalized());
			}
			break;

			case 3:
			{
				// calculate if the simplex contains the origin
				Vector2 a = m_Vertices[2];
				Vector2 b = m_Vertices[1];
				Vector2 c = m_Vertices[0];
				
				Vector2 a0 = (a * -1).GetNormalized(); // v2 to the origin
				Vector2 ab = (b - a ).GetNormalized(); // v2 to v1
				Vector2 ac = (c - a ).GetNormalized(); // v2 to v0
				
				Vector2 abPerp = MathsUtils::TripleProduct(ac, ab, ab);
				Vector2 acPerp = MathsUtils::TripleProduct(ab, ac, ac);

				if (abPerp.Dot(a0) > 0) 
				{
					// the origin is outside line ab
					// get rid of c and add a new support 
					// in the direction of abPerp
				
					auto itr = std::find(m_Vertices.begin(), m_Vertices.end(), c);
					
					if(itr != m_Vertices.end())
					{
						m_Vertices.erase(itr);
						m_SupportDirection = abPerp;
					}
					else
					{
						throw std::runtime_error("You could not find c in the vector.");
					}

				}
				else if (acPerp.Dot(a0) > 0)
				{
					// the origin is outside line ac
					// get rid of b and add a new support 
					// in the direction of acPerp
					auto itr = std::find(m_Vertices.begin(), m_Vertices.end(), b);

					if (itr != m_Vertices.end())
					{
						m_Vertices.erase(itr);
						m_SupportDirection = acPerp;
					}
					else
					{
						throw std::runtime_error("You could not find b in the vector.");
					}
					m_SupportDirection = acPerp;
				}
				else {
					// the origin is inside both ab and ac,
					// so it must be inside the triangle!
					return GJK::SimplexEvolution::FoundIntersection;
				}
			}
			break;

			default:
				throw std::range_error("Cannot have a simplex triangle with more than 3 vertices.");
				break;
			}

			if (GJK_AddSupportToTriangle(m_SupportDirection, bodyA, bodyB))
			{
				return GJK::SimplexEvolution::StillEvolving;
			}
			else
			{
				return GJK::SimplexEvolution::NoIntersection;
			}

		}

		void BuildManifold(Manifold& manifold)
		{

		}
	};

	static bool PolygonVsPolygon(Body* bodyA, Body* bodyB, Manifold& manifold)
	{
		GJK gjk;

		GJK::SimplexEvolution result = GJK::SimplexEvolution::StillEvolving;
		while (result == GJK::SimplexEvolution::StillEvolving)
		{
			result = gjk.EvolveSimplex(*bodyA, *bodyB);
		}

		if (result == GJK::SimplexEvolution::FoundIntersection)
		{
			gjk.BuildManifold(manifold);
			return true;
		}

		return false;
	};
};