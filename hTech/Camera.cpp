#include "pch.h"
#include "Camera.h"


//Camera* Camera::mInstance = nullptr;

Camera::Camera()
{
	mPosition = Vector2f();
}

Camera::~Camera()
{
	//delete mInstance;
	//mInstance = nullptr;
}

Camera* Camera::Get()
{
	static Camera mInstance;
	//todo : fix : Was static Camera* mInstance;
	 
	//if (!mInstance)
	//	mInstance = new Camera();

	return &mInstance;
}

void Camera::SetCameraPosition_Impl(Vector2f position)
{
	mPosition = position;
}

void Camera::SetCameraPosition(Vector2f position)
{
	Get()->SetCameraPosition_Impl(position);
}

Vector2f Camera::GetCameraPosition()
{
	return Get()->GetCameraPosition_Impl();
}

Vector2f Camera::GetCameraPosition_Impl()
{
	return mPosition;
}

Vector2f Camera::WorldToScreen(Vector2f worldPosition)
{
	return Get()->WorldToScreen_Impl(worldPosition);
}

Vector2f Camera::WorldToScreen_Impl(Vector2f worldPosition)
{
	Vector2f camPosition = mPosition;
	camPosition.Y *= -1;
	Vector2f wp = worldPosition;
	wp.Y *= -1;
	return Vector2f((wp - camPosition) + Settings::Get()->GetWindowCentre());
}

Vector2f Camera::ScreenToWorld(Vector2f screenPosition)
{
	return Get()->ScreenToWorld_Impl(screenPosition);
}

Vector2f Camera::ScreenToWorld_Impl(Vector2f screenPosition)
{
	Vector2f camPosition = mPosition;
	camPosition.Y *= -1;
	Vector2f out = Vector2f((screenPosition + camPosition) - Settings::Get()->GetWindowCentre());
	out.Y *= -1;
	return out;
}