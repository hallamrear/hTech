#pragma once
#include "Manifold.h"
#include "Body.h"
#include "Vector2.h"
#include "MathsUtils.h"
#include <stdexcept>
#include "World.h"

namespace Collision
{
	/// <summary>
	/// -- GJK --
	/// GJK works by trying to find a triangle which fits within the
	/// minkowski difference of two bodies and encapsulates the origin.
	/// If the tri contains the orgin and the fits inside the mink diff then
	/// the minkdiff also must contain the origin.
	///
	/// -- EPA --
	/// The Expanding Polytope Algorithm works well with Minkowski differences as
	/// they both require support functions and simplexes and so can be built off
	/// each other.
	/// Whereas GJK uses support functions to find the simplex containing the origin, EPA uses them
	/// to find the boundary of the minkdiff that is closest to the origin.a
	/// 
	/// We are assuming our vertices are always in clockwise winding order.
	/// </summary>
	class GJK
	{
	private:
		struct ClosestEdgeDetails
		{
			int EdgeID;
			float Distance;
			Vector2 Normal;
		};

		std::vector<Vector2> m_SimplexPoints = std::vector<Vector2>();

		/// <summary>
		/// Checks if the dot product is greater than 0 to determine whether of not the two vectors are in the same direction.
		/// </summary>
		/// <param name="A"></param>
		/// <param name="B"></param>
		/// <returns></returns>
		bool AreVectorsInSameDirection(const Vector2& A, const Vector2& B);

		/// <summary>
		/// Finds the MTV (minimum translation vector) of the two objects.
		/// </summary>
		/// <param name="bodyA"></param>
		/// <param name="bodyB"></param>
		/// <returns>A vector representing the shortest distance to dislodge the two objects.</returns>
		Vector2 EPA(Body* bodyA, Body* bodyB);

		/// <summary>
		/// This function fills the manifold with the appropriate collision data using the EPA method.
		/// </summary>
		/// <param name="bodyA"></param>
		/// <param name="bodyB"></param>
		/// <param name="manifold"></param>
		void GetCollisionDetails(Body* bodyA, Body* bodyB, Manifold* manifold);

		/// <summary>
		/// This function runs the GJK algorithm to determine whether of not a collision has taken place.
		/// </summary>
		/// <param name="bodyA"></param>
		/// <param name="bodyB"></param>
		/// <returns></returns>
		bool DetermineCollisionUsingSimplex(Body* bodyA, Body* bodyB);
		
		/// <summary>
		/// This function determines our next simplex point based on the current size of the simplex.
		/// It also performs checks for the origin to determine collision.
		/// </summary>
		/// <param name="searchDirection"></param>
		/// <returns></returns>
		bool CalculateNextSimplexPoint(Vector2& searchDirection);

		/// <summary>
		/// This function determines our next simplex point based on the current size of the simplex.
		/// It also performs checks for the origin to determine collision.
		/// </summary>
		/// <param name="searchDirection"></param>
		/// <returns></returns>
		bool CalculateSimplexPointFromTriangle(Vector2& searchDirection);

		/// <summary>
		/// This function determines our next simplex point based on the current size of the simplex.
		/// It also performs checks for the origin to determine collision.
		/// </summary>
		/// <param name="searchDirection"></param>
		/// <returns></returns>
		bool CalculateSimplexPointFromLine(Vector2& searchDirection);

		/// <summary>
		/// Gets the point representing the minkwoski difference between the furthest point
		///  in the direction on A and the furthest point the negative direction on B.
		/// 
		/// A.GetSupport(dir) - B.GetSupport(-dir)
		/// 
		/// </summary>
		/// <param name="direction"></param>
		/// <param name="bodyA"></param>
		/// <param name="bodyB"></param>
		/// <returns></returns>
		Vector2 GetMinkowskiDifferenceSupportVertex(const Vector2& direction, Body* bodyA, Body* bodyB);

		/// <summary>
		/// Returns the furthest point in a given direction in the Simplex.
		/// </summary>
		/// <param name="direction"></param>
		/// <returns></returns>
		Vector2 GetSupportVertexOfSimplex(const Vector2& direction);

	public:
		
		/// <summary>
		/// Returns whether body A and body B are colliding. This will also fill out the manifold if the manifold passed in is not nullptr.
		/// </summary>
		/// <param name="bodyA"></param>
		/// <param name="bodyB"></param>
		/// <param name="manifold"></param>
		/// <returns></returns>
		bool IsOverlapping(Body* bodyA, Body* bodyB, Manifold* manifold);
	};

	static bool PolygonVsPolygon(Body* bodyA, Body* bodyB)
	{
		GJK gjk;
		return gjk.IsOverlapping(bodyA, bodyB, nullptr);
	}

	static bool PolygonVsPolygon(Body* bodyA, Body* bodyB, Manifold& manifold)
	{
		GJK gjk;
		return gjk.IsOverlapping(bodyA, bodyB, &manifold);
	};
};