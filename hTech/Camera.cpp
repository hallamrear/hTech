#include "pch.h"
#include "Camera.h"
#include "UI.h"
#include "InputManager.h"
#include "Console.h"

Camera* Camera::m_Instance = nullptr;
float Camera::ZoomLevel = 1.0f;

Camera::Camera()
{
	m_Position = Vector2();

#if(ENABLE_ZOOMING)
		InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP, IM_KEY_STATE::IM_KEY_PRESSED,
			[this]()
			{
				Camera::ZoomLevel = std::min(MAX_ZOOM, Camera::ZoomLevel + ZOOM_STEP);
			});


		InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN, IM_KEY_STATE::IM_KEY_PRESSED, 
			[this]()
			{
				Camera::ZoomLevel = std::max(MIN_ZOOM, Camera::ZoomLevel - ZOOM_STEP);
			});
#endif
}

Camera::~Camera()
{
	delete m_Instance;
	m_Instance = nullptr;
}

Camera* Camera::Get()
{	 
	if (!m_Instance)
		m_Instance = new Camera();

	return m_Instance;
}

void Camera::SetCameraPosition_Impl(Vector2 Position)
{
	m_Position = Position;
}

void Camera::SetCameraPosition(Vector2 Position)
{
	Get()->SetCameraPosition_Impl(Position);
}

Vector2 Camera::GetCameraPosition()
{
	return Get()->GetCameraPosition_Impl();
}

Vector2 Camera::GetCameraPosition_Impl()
{
	return m_Position;
}

Vector2 Camera::WorldToScreen(Vector2 worldPosition)
{
	return Get()->WorldToScreen_Impl(worldPosition);
}

Vector2 Camera::WorldToScreen_Impl(Vector2 worldPosition)
{
	Vector2 camPosition = m_Position;
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

void Camera::RenderProperties()
{
	return Get()->RenderProperties_Impl();
}

Vector2 Camera::ScreenToWorld_Impl(Vector2 screenPosition)
{
	Vector2 WindowCentre;
	WindowCentre.X = Console::Query("WindowCentreX");
	WindowCentre.Y = Console::Query("WindowCentreY");

	Vector2 camPosition = m_Position;
	camPosition.Y *= -1;
	Vector2 out = Vector2((screenPosition + camPosition) - WindowCentre);
	out.Y *= -1;
	return out;
}

void Camera::RenderProperties_Impl()
{
	ImGui::Begin("Camera details");
	float Position[2] = { m_Position.X, m_Position.Y };
	ImGui::InputFloat2("Position", Position);
	m_Position.X = Position[0];
	m_Position.Y = Position[1];

	ImGui::End();
}
