#include "pch.h"
#include "Rectangle.h"
#include "Camera.h"

WorldRectangle::WorldRectangle(Vector2 TL, Vector2 BR)
{
	X = (int) TL.X;
	Y = (int) TL.Y;
	W = (int)(BR.X - TL.X);
	H = (int)(TL.Y - BR.Y);
}

WorldRectangle::WorldRectangle(int x, int y, int w, int h)
{
	X = x;
	Y = y;
	W = w;
	H = h;
}
