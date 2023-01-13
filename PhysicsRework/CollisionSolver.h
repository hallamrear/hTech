#pragma once
#include "Vector2.h"
#include <vector>

class Body;
struct Manifold;

class CollisionSolver
{
protected:

	/// <summary>
	/// Contains the data regarding a specific contact point of a collision.
	/// </summary>
	struct ContactPoint
	{
		/* The first body involved in the collision */
		Body* BodyA;
		/* The second body involved in the collision */
		Body* BodyB;
		/* The contact point between the two bodies */ 
		Vector2 Position = Vector2::Zero;
		/* The direction of the shortest distance of intersection */ 
		Vector2 Normal = Vector2::Zero;
		/* Intersection distance along collision normal */			
		float   Depth = 0.0f;
		/* Vector from BodyA->Position to contact point position */ 
		Vector2 AC;
		/* Vector from BodyB->Position to contact point position */  
		Vector2 BC;
	};

	/// <summary>
	/// Used to scale the values between multiple contact points.
	/// </summary>
	float m_PointSplit;

	/// <summary>
	/// Holds the various contact points of a collision.
	/// </summary>
	std::vector<ContactPoint> m_ContactPoints;

	/// <summary>
	/// Converts a collision manifold into a series of contact points
	/// that are used in the physics step.
	/// </summary>
	/// <param name="manifold">Manifold containing the collision data.</param>
	void ConstuctContactPointsFromManifold(const Manifold& manifold);

public:
	/// <summary>
	/// Uses the manifold to fill out a vector containing the various points on contact.
	/// </summary>
	/// <param name="manifold">Manifold containing the collision data.</param>
	CollisionSolver(const Manifold& manifold);

	/// <summary>
	/// Deconstructor
	/// </summary>
	virtual ~CollisionSolver() = 0;

	/// <summary>
	/// Various collision solver methods may require a precalculation step.
	/// </summary>
	virtual void Prestep() = 0;

	/// <summary>
	/// This function runs the physics code that affects the rigidbodies.
	/// </summary>
	virtual void PhysicsStep() = 0;

	/// <summary>
	/// Removes the intersection of the two bodies by directly altering the position of each.
	/// </summary>
	virtual void PositionResolution();
};

/// <summary>
/// Implementation of Collision solver specifically to work with the GJK collision detection.
/// </summary>
class GJKCollisionSolver : public CollisionSolver
{
protected:

public:
	GJKCollisionSolver(const Manifold& manifold) : CollisionSolver(manifold) { };
	~GJKCollisionSolver() {};

	void Prestep();
	void PhysicsStep();
};