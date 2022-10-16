#pragma once
#include "Vector2.h"

class Camera
{
private:
	Vector2 mPosition;

	Camera();

	void SetCameraPosition_Impl(Vector2 position);
	Vector2 GetCameraPosition_Impl();
	Vector2 WorldToScreen_Impl(Vector2 entityWorldPosition);
	Vector2 ScreenToWorld_Impl(Vector2 screenPosition);

public:
	~Camera();
	static Camera* Get();

	static void SetCameraPosition(Vector2 position);
	static Vector2 GetCameraPosition();
	static Vector2 WorldToScreen(Vector2 entityWorldPosition);
	static Vector2 ScreenToWorld(Vector2 screenPosition);
};

//todo : figure out a zoom thing with SDL_RenderSetScale();