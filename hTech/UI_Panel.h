#pragma once
#ifdef DEFINE_OLD_UI
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)


#include "Rectangle.h"
#include "Colour.h"

class HTECH_FUNCTION_EXPORT UI_Panel
{
public:
	int X = 0;
	int Y = 0;
	int W = 1;
	int H = 1;
	Colour BackgroundColour = Colour(255, 255, 255, 255);
	Colour TextColour = Colour(0, 0, 0, 255);

	UI_Panel();
	UI_Panel(int posX, int posY, int width, int height, Colour textColour = Colour(0, 0, 0, 255), Colour bgColour = Colour(255, 255, 255, 255));
};

#endif