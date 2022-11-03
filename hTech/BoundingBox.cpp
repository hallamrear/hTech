#include "pch.h"
#include "BoundingBox.h"
#include "Transform.h"
#include "Camera.h"

BoundingBox::BoundingBox(Transform& transform, float size_x, float size_y)
	: Collider(transform)
{
	mType = COLLIDER_TYPE::COLLIDER_AABB;

	Size.X = size_x;
	Size.Y = size_y;

	TopLeft = Vector2(mTransform.Position.X - (Size.X / 2.0f), mTransform.Position.Y + (Size.Y / 2.0f));
	BottomRight = Vector2(mTransform.Position.X + (Size.X / 2.0f), mTransform.Position.Y - (Size.Y / 2.0f));
};

BoundingBox::~BoundingBox()
{
	
};

void BoundingBox::Update(float DeltaTime)
{
	TopLeft = Vector2(mTransform.Position.X - (Size.X / 2), mTransform.Position.Y + (Size.Y / 2));
	BottomRight = Vector2(mTransform.Position.X + (Size.X / 2), mTransform.Position.Y - (Size.Y / 2));
}

void BoundingBox::Render(SDL_Renderer& renderer)
{
	bool b = Settings::Get()->GetDrawColliders();
	if (b)
	{
		SDL_Rect r{};
		Vector2 position = Camera::WorldToScreen(Vector2(mTransform.Position.X - (Size.X / 2), mTransform.Position.Y + (Size.Y / 2)));
		position = Camera::WorldToScreen(mTransform.Position);
		r.x = (int)position.X;
		r.y = (int)position.Y;
		r.w = (int)(Size.X);
		r.h = (int)(Size.Y);

		SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(&renderer, &r);
	}
}

void BoundingBox::GetColliderAsPoints(Vector2 points[]) const
{
	points[0] = Vector2(TopLeft.X, BottomRight.Y); //BottomLeft
	points[1] = BottomRight;
	points[2] = Vector2(BottomRight.X, TopLeft.Y); //TopRight
	points[3] = TopLeft;
}

Vector2 BoundingBox::FindFurthestPoint(Vector2 direction) const
{
	Vector2 maxPoint;
	float maxDistance = -FLT_MAX;

	Vector2 vertices[4];
	GetColliderAsPoints(vertices);

	for (int i = 0; i < 4; i++)
	{
		float distance = vertices[i].Dot(direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = vertices[i];
		}
	}

	return maxPoint;
}
