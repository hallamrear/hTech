#pragma once
#include "Colour.h"

class PanelRect
{
public:
	unsigned short X = 0;
	unsigned short Y = 0;
	unsigned short W = 1;
	unsigned short H = 1;
	Colour BackgroundColour = Colour(255, 255, 255, 255);
	Colour TextColour = Colour(0, 0, 0, 255);

	PanelRect();
	PanelRect(unsigned short posX, unsigned short posY, unsigned short width, unsigned short height, Colour textColour = Colour(0, 0, 0, 255), Colour bgColour = Colour(255, 255, 255, 255));
};
