#include "pch.h"
#include "OrientedBoundingBox.h"
#include "Camera.h"

OrientedBoundingBox::OrientedBoundingBox(Vector2& position, float& rotation, float size_x, float size_y)
	: BoundingBox(position, size_x, size_y), Rotation(rotation)
{
	mType = COLLIDER_TYPE::COLLIDER_OBB;
	CalculateRotations();
}

OrientedBoundingBox::~OrientedBoundingBox()
{
	Rotation = 0.0f;
}

void OrientedBoundingBox::CalculateRotations()
{
	if (Rotation != 0.0f && Rotation != 360.0f)
	{
		float rotation = 360.0f - Rotation;
			 TopLeft = HelperFunctions::RotatePointAroundOriginDegrees(Vector2(mOrigin.X - (Size.X / 2), mOrigin.Y + (Size.Y / 2)), rotation, mOrigin);
		  BottomLeft = HelperFunctions::RotatePointAroundOriginDegrees(Vector2(mOrigin.X - (Size.X / 2), mOrigin.Y - (Size.Y / 2)), rotation, mOrigin);
			TopRight = HelperFunctions::RotatePointAroundOriginDegrees(Vector2(mOrigin.X + (Size.X / 2), mOrigin.Y + (Size.Y / 2)), rotation, mOrigin);
		 BottomRight = HelperFunctions::RotatePointAroundOriginDegrees(Vector2(mOrigin.X + (Size.X / 2), mOrigin.Y - (Size.Y / 2)), rotation, mOrigin);
	}
}

void OrientedBoundingBox::Update(double deltaTime)
{
	CalculateRotations();
}

void OrientedBoundingBox::Render(SDL_Renderer& renderer)
{
	if (Settings::Get()->GetDrawColliders())
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