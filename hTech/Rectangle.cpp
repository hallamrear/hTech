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

void WorldRectangle::Render(SDL_Renderer& renderer, bool drawFromCenter)
{
	SDL_Rect rect{};
	Vector2 Position = Vector2();
	if (drawFromCenter)
	{
		Position = Vector2((float)(X - (W / 2)), (float)(Y + (H / 2)));
	}
	else
	{
		Position.X = (float)X;
		Position.Y = (float)Y;
	}

	Position = Camera::WorldToScreen(Position);
	rect.x = (int)Position.X;
	rect.y = (int)Position.Y;
	rect.w = W;
	rect.h = H;
	SDL_RenderDrawRect(&renderer, &rect);
}
