#include "pch.h"
#include "BoundingPolygon.h"
#include "Transform.h"
#include "Camera.h"
#include "Console.h"

BoundingPolygon::BoundingPolygon(Transform& origin, float& rotation, const std::vector<Vector2> points, const int& pointCount) : Collider(origin), mRotation(rotation)
{
	mType = COLLIDER_TYPE::COLLIDER_POLYGON;
	PointCount = pointCount;
	mOriginalPoints = std::vector<Vector2>();
	mOriginalPoints = points;

	for (size_t i = 0; i < PointCount; i++)
	{
		mTransformedPoints.push_back(mTransform.Position + mOriginalPoints[i]);
	}
}

void BoundingPolygon::Update(float DeltaTime)
{
	for (size_t i = 0; i < PointCount; i++)
	{
		mTransformedPoints[i] = MathsHelp::RotatePointAroundOriginDegrees(mTransform.Position + mOriginalPoints[i], mRotation, mTransform.Position);
	}
}

void BoundingPolygon::Render(SDL_Renderer& renderer)
{
	if (Console::Query("DrawColliders") != 0)
	{
		PointCount = mOriginalPoints.size();

		switch (PointCount)
		{
		case 0:
			break;
		case 1:
			SDL_RenderDrawPoint(&renderer, (int)mTransformedPoints[0].X, (int)mTransformedPoints[0].Y);
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
				SDL_RenderDrawLine(&renderer, (int)screenSpaceOne.X, (int)screenSpaceOne.Y, (int)screenSpaceTwo.X, (int)screenSpaceTwo.Y);
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
	for (size_t i = 0; i < PointCount; i++)
	{
		points[i] = mTransformedPoints[i];
	}
}