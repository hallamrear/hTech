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
	Vector2 position = Vector2();
	if (drawFromCenter)
	{
		position = Vector2((float)(X - (W / 2)), (float)(Y + (H / 2)));
	}
	else
	{
		position.X = (float)X;
		position.Y = (float)Y;
	}

	position = Camera::WorldToScreen(position);
	rect.x = (int)position.X;
	rect.y = (int)position.Y;
	rect.w = W;
	rect.h = H;
	SDL_RenderDrawRect(&renderer, &rect);
}
