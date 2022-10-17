#pragma once
#include "Collision.h"

/// <summary>
/// A Bounding Sphere class used by rigidbodies for collision detection. None of the collision detection is done using this class.
/// </summary>

class BoundingSphere
	: public Collider
{
public:
	float Radius = 0;

	BoundingSphere(Transform& transform, float radius);
	virtual ~BoundingSphere();

	/// <summary>
	/// </summary>
	/// <param name="deltaTime">Time between the last frame and this frame.</param>
	virtual void Update(double deltaTime);

	/// <summary>
	/// Renders the circle using SDL_RenderDrawPoint and an SDL_Renderer.
	/// </summary>
	/// <param name="renderer">SDL_Renderer to use for rendering the circles points.</param>
	virtual void Render(SDL_Renderer& renderer);

	/// <summary>
	/// Returns a point on the circle in the direction from the centre.
	/// </summary>
	/// <param name="direction">Direction from the centre of the circle to get the point from.</param>
	/// <returns>Position on the bounding circle.</returns>
	virtual Vector2 FindFurthestPoint(Vector2 direction) const override;

	/// <summary>
	/// Returns a series of points representing the outline of the circle.
	/// </summary>
	/// <param name="points">Array of points on the circle outline.</param>
	virtual void GetColliderAsPoints(Vector2 points[]) const override;
};