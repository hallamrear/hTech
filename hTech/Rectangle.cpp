#include "pch.h"
#include "Rectangle.h"
#include "Camera.h"

void Rectangle_::Render(SDL_Renderer& renderer)
{
	SDL_Rect rect;
	Vector2 position = Camera::WorldToScreen(Vector2(X - (W / 2), Y + (H / 2)));
	rect.x = (int)position.X;
	rect.y = (int)position.Y;
	rect.w = W;
	rect.h = H;
}
