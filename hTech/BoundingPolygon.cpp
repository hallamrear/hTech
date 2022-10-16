#include "pch.h"
#include "BoundingPolygon.h"
#include "Camera.h"

BoundingPolygon::BoundingPolygon(Vector2& origin, float& rotation, const std::vector<Vector2> points, const int& pointCount) : Collider(origin), mRotation(rotation)
{
	mType = COLLIDER_TYPE::COLLIDER_POLYGON;
	PointCount = pointCount;
	mOriginalPoints = std::vector<Vector2>();
	mOriginalPoints = points;

	for (int i = 0; i < PointCount; i++)
	{
		mTransformedPoints.push_back(mOrigin + mOriginalPoints[i]);
	}
}

void BoundingPolygon::Update(double deltaTime)
{
	for (int i = 0; i < PointCount; i++)
	{
		mTransformedPoints[i] = HelperFunctions::RotatePointAroundOriginDegrees(mOrigin + mOriginalPoints[i], mRotation, mOrigin);
	}
}

void BoundingPolygon::Render(SDL_Renderer& renderer)
{
	if (Settings::Get()->GetDrawColliders())
	{
		PointCount = mOriginalPoints.size();

		switch (PointCount)
		{
		case 0:
			break;
		case 1:
			SDL_RenderDrawPoint(&renderer, mTransformedPoints[0].X, mTransformedPoints[0].Y);
			break;
		default:
			Vector2 pOne = mTransformedPoints[0];
			Vector2 pTwo;

			for (size_t i = 0; i < PointCount; i++)
			{
				pOne = mTransformedPoints[i % PointCount];
				pTwo = mTransformedPoints[(i + 1) % PointCount];

				Vector2 screenSpaceOne = Camera::WorldToScreen(pOne);
				Vector2 screenSpaceTwo = Camera::WorldToScreen(pTwo);

				SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);
				SDL_RenderDrawLine(&renderer, screenSpaceOne.X, screenSpaceOne.Y, screenSpaceTwo.X, screenSpaceTwo.Y);
			}
			break;
		}
	}
}

Vector2 BoundingPolygon::FindFurthestPoint(Vector2 direction) const
{
	return Vector2();
}

void BoundingPolygon::GetColliderAsPoints(Vector2 points[]) const
{
	for (int i = 0; i < PointCount; i++)
	{
		points[i] = mTransformedPoints[i];
	}
}