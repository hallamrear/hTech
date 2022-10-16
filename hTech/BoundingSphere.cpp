#include "pch.h"
#include "BoundingSphere.h"
#include "Settings.h"
#include "Camera.h"

BoundingSphere::BoundingSphere(Vector2& position, float radius)
	: Collider(position)
{
	mType = COLLIDER_TYPE::COLLIDER_SPHERE;
	Radius = radius;
}

BoundingSphere::~BoundingSphere()
{
	
}

void BoundingSphere::Update(double deltaTime)
{
	
}

void BoundingSphere::Render(SDL_Renderer& renderer)
{
	if(Settings::Get()->GetDrawColliders())
	{
		SDL_SetRenderDrawColor(&renderer, 0, 255, 255, 255);
		Vector2 centre = Camera::WorldToScreen(mOrigin);
		SDL_RenderDrawPoint(&renderer, (int)centre.X, (int)centre.Y);
		Vector2 point;
		for (double angle = 0; angle <= 2 * M_PI; angle += 0.25f)
		{
			point.X = mOrigin.X + Radius * (float)cos(angle);
			point.Y = mOrigin.Y + Radius * (float)sin(angle);
			point = Camera::WorldToScreen(point);
			SDL_RenderDrawPoint(&renderer, (int)point.X, (int)point.Y);
		}
		SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
	}
}

Vector2 BoundingSphere::FindFurthestPoint(Vector2 direction) const
{
	return mOrigin + (direction.GetNormalized() * Radius);
}

void BoundingSphere::GetColliderAsPoints(Vector2 points[]) const
{

}
