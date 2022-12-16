#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Vector2.h"

class HTECH_FUNCTION_EXPORT Camera
{
private:
	static Camera* m_Instance;
	Vector2 m_Position;

	Camera();

	void SetCameraPosition_Impl(Vector2 Position);
	Vector2 GetCameraPosition_Impl();
	Vector2 WorldToScreen_Impl(Vector2 entityWorldPosition);
	Vector2 ScreenToWorld_Impl(Vector2 screenPosition);
	void RenderProperties_Impl();

public:
	~Camera();
	static Camera* Get();

	static void SetCameraPosition(Vector2 Position);
	static Vector2 GetCameraPosition();
	static Vector2 WorldToScreen(Vector2 entityWorldPosition);
	static Vector2 ScreenToWorld(Vector2 screenPosition);
	static void RenderProperties();
};