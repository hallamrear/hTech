#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

class WorldRectangle
{
public:
	int X = 0;
	int Y = 0;
	int W = 1;
	int H = 1;

	WorldRectangle(Vector2 TL, Vector2 BR);
	WorldRectangle(int x, int y, int w, int h);
};

typedef WorldRectangle ScreenRectangle;