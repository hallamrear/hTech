#include "pch.h"
#include "Rectangle.h"
#include "Camera.h"

WorldRectangle::WorldRectangle(Vector2 TL, Vector2 BR)
{
	X = TL.X;
	Y = TL.Y;
	W = BR.X - TL.X;
	H = TL.Y - BR.Y;
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
	SDL_Rect rect;
	Vector2 position = Vector2();
	if (drawFromCenter)
	{
		position = Vector2(X - (W / 2), Y + (H / 2));
	}
	else
	{
		position.X = X;
		position.Y = Y;
	}

	position = Camera::WorldToScreen(position);
	rect.x = (int)position.X;
	rect.y = (int)position.Y;
	rect.w = W;
	rect.h = H;
	SDL_RenderDrawRect(&renderer, &rect);
}
