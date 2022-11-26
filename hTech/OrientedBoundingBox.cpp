#include "pch.h"
#include "OrientedBoundingBox.h"
#include "Camera.h"
#include "Transform.h"

OrientedBoundingBox::OrientedBoundingBox(Transform& transform, float size_x, float size_y)
	: BoundingBox(transform, size_x, size_y)
{
	mType = COLLIDER_TYPE::COLLIDER_OBB;
	CalculateRotations();
}

OrientedBoundingBox::~OrientedBoundingBox()
{

}

void OrientedBoundingBox::CalculateRotations()
{
	if (mTransform.Rotation != 0.0f || mTransform.Rotation != 360.0f)
	{
		float rotation = 360.0f - mTransform.Rotation;
			 TopLeft = MathsHelp::RotatePointAroundOriginDegrees(Vector2(mTransform.Position.X - (Size.X / 2), mTransform.Position.Y + (Size.Y / 2)), rotation, mTransform.Position);
		  BottomLeft = MathsHelp::RotatePointAroundOriginDegrees(Vector2(mTransform.Position.X - (Size.X / 2), mTransform.Position.Y - (Size.Y / 2)), rotation, mTransform.Position);
			TopRight = MathsHelp::RotatePointAroundOriginDegrees(Vector2(mTransform.Position.X + (Size.X / 2), mTransform.Position.Y + (Size.Y / 2)), rotation, mTransform.Position);
		 BottomRight = MathsHelp::RotatePointAroundOriginDegrees(Vector2(mTransform.Position.X + (Size.X / 2), mTransform.Position.Y - (Size.Y / 2)), rotation, mTransform.Position);
	}
}

void OrientedBoundingBox::Update(float DeltaTime)
{
	CalculateRotations();
}

void OrientedBoundingBox::Render(SDL_Renderer& renderer)
{
	if (Console::Query("DrawColliders") != 0)
	{
		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);

		const Vector2 TopLeft_ScreenSpace = Camera::WorldToScreen(TopLeft);
		const Vector2 TopRight_ScreenSpace = Camera::WorldToScreen(TopRight);
		const Vector2 BottomLeft_ScreenSpace = Camera::WorldToScreen(BottomLeft);
		const Vector2 BottomRight_ScreenSpace = Camera::WorldToScreen(BottomRight);

		//Top
		SDL_RenderDrawLine(&renderer, (int)TopLeft_ScreenSpace.X, (int)TopLeft_ScreenSpace.Y, (int)TopRight_ScreenSpace.X, (int)TopRight_ScreenSpace.Y);
		//Bot
		SDL_RenderDrawLine(&renderer, (int)BottomLeft_ScreenSpace.X, (int)BottomLeft_ScreenSpace.Y, (int)BottomRight_ScreenSpace.X, (int)BottomRight_ScreenSpace.Y);
		//Left
		SDL_RenderDrawLine(&renderer, (int)TopLeft_ScreenSpace.X, (int)TopLeft_ScreenSpace.Y, (int)BottomLeft_ScreenSpace.X, (int)BottomLeft_ScreenSpace.Y);
		//Right
		SDL_RenderDrawLine(&renderer, (int)TopRight_ScreenSpace.X, (int)TopRight_ScreenSpace.Y, (int)BottomRight_ScreenSpace.X, (int)BottomRight_ScreenSpace.Y);
	}
}

//Fills array with points of the box.
void OrientedBoundingBox::GetColliderAsPoints(Vector2 points[]) const
{
	points[0] = BottomLeft;
	points[1] = BottomRight;
	points[2] = TopRight;
	points[3] = TopLeft;
}