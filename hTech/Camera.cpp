#include "pch.h"
#include "Camera.h"


//Camera* Camera::mInstance = nullptr;

Camera::Camera()
{
	mPosition = Vector2();
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

void Camera::SetCameraPosition_Impl(Vector2 position)
{
	mPosition = position;
}

void Camera::SetCameraPosition(Vector2 position)
{
	Get()->SetCameraPosition_Impl(position);
}

Vector2 Camera::GetCameraPosition()
{
	return Get()->GetCameraPosition_Impl();
}

Vector2 Camera::GetCameraPosition_Impl()
{
	return mPosition;
}

Vector2 Camera::WorldToScreen(Vector2 worldPosition)
{
	return Get()->WorldToScreen_Impl(worldPosition);
}

Vector2 Camera::WorldToScreen_Impl(Vector2 worldPosition)
{
	Vector2 camPosition = mPosition;
	camPosition.Y *= -1;
	Vector2 wp = worldPosition;
	wp.Y *= -1;
	return Vector2((wp - camPosition) + Settings::Get()->GetWindowCentre());
}

Vector2 Camera::ScreenToWorld(Vector2 screenPosition)
{
	return Get()->ScreenToWorld_Impl(screenPosition);
}

Vector2 Camera::ScreenToWorld_Impl(Vector2 screenPosition)
{
	Vector2 camPosition = mPosition;
	camPosition.Y *= -1;
	Vector2 out = Vector2((screenPosition + camPosition) - Settings::Get()->GetWindowCentre());
	out.Y *= -1;
	return out;
}