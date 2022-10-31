#include "pch.h"
#include "BoundingSphere.h"
#include "Settings.h"
#include "Camera.h"
#include "Transform.h"

BoundingSphere::BoundingSphere(Transform& transform, float radius)
	: Collider(transform)
{
	mType = COLLIDER_TYPE::COLLIDER_SPHERE;
	Radius = radius;
}

BoundingSphere::~BoundingSphere()
{
	
}

void BoundingSphere::Update(float DeltaTime)
{
	
}

void BoundingSphere::Render(SDL_Renderer& renderer)
{
	if(Settings::Get()->GetDrawColliders())
	{
		SDL_SetRenderDrawColor(&renderer, 0, 255, 255, 255);
		Vector2 centre = Camera::WorldToScreen(mTransform.Position);
		SDL_RenderDrawPoint(&renderer, (int)centre.X, (int)centre.Y);
		Vector2 point;
		for (double angle = 0; angle <= 2 * M_PI; angle += 0.25f)
		{
			point.X = mTransform.Position.X + Radius * (float)cos(angle);
			point.Y = mTransform.Position.Y + Radius * (float)sin(angle);
			point = Camera::WorldToScreen(point);
			SDL_RenderDrawPoint(&renderer, (int)point.X, (int)point.Y);
		}
		SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
	}
}

Vector2 BoundingSphere::FindFurthestPoint(Vector2 direction) const
{
	return mTransform.Position + (direction.GetNormalized() * Radius);
}

void BoundingSphere::GetColliderAsPoints(Vector2 points[]) const
{

}
