#include "pch.h"
#include "Camera.h"
#include "UI.h"
#include "InputManager.h"
#include "Console.h"

Camera* Camera::mInstance = nullptr;

Camera::Camera()
{
	mPosition = Vector2();

	InputManager::Bind(IM_KEY_CODE::IM_KEY_UP_ARROW, IM_KEY_STATE::IM_KEY_HELD,
		[this]
		{
			Vector2 pos = Camera::GetCameraPosition();
			pos.Y += 2.5f;
			Camera::SetCameraPosition(pos);
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_DOWN_ARROW, IM_KEY_STATE::IM_KEY_HELD,
		[this]
		{
			Vector2 pos = Camera::GetCameraPosition();
			pos.Y -= 2.5f;
			Camera::SetCameraPosition(pos);
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_LEFT_ARROW, IM_KEY_STATE::IM_KEY_HELD,
		[this]
		{
			Vector2 pos = Camera::GetCameraPosition();
			pos.X -= 2.5f;
			Camera::SetCameraPosition(pos);
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_RIGHT_ARROW, IM_KEY_STATE::IM_KEY_HELD,
		[this]
		{
			Vector2 pos = Camera::GetCameraPosition();
			pos.X += 2.5f;
			Camera::SetCameraPosition(pos);
		});
}

Camera::~Camera()
{
	delete mInstance;
	mInstance = nullptr;
}

Camera* Camera::Get()
{	 
	if (!mInstance)
		mInstance = new Camera();

	return mInstance;
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

	Vector2 WindowCentre;
	WindowCentre.X = Console::Query("WindowCentreX");
	WindowCentre.Y = Console::Query("WindowCentreY");

	return Vector2((wp - camPosition) + WindowCentre);
}

Vector2 Camera::ScreenToWorld(Vector2 screenPosition)
{
	return Get()->ScreenToWorld_Impl(screenPosition);
}

Vector2 Camera::ScreenToWorld_Impl(Vector2 screenPosition)
{
	Vector2 WindowCentre;
	WindowCentre.X = Console::Query("WindowCentreX");
	WindowCentre.Y = Console::Query("WindowCentreY");

	Vector2 camPosition = mPosition;
	camPosition.Y *= -1;
	Vector2 out = Vector2((screenPosition + camPosition) - WindowCentre);
	out.Y *= -1;
	return out;
}