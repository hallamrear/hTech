#pragma once

struct Rectangle
{
public:
	int X = 0;
	int Y = 0;
	int W = 1;
	int H = 1;

	void Render(SDL_Renderer& renderer);
};

