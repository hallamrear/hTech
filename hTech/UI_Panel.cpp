#include "pch.h"
#include "UI_Panel.h"

UI_Panel::UI_Panel()
{
}

UI_Panel::UI_Panel(unsigned short posX, unsigned short posY, unsigned short width, unsigned short height, Colour textColour, Colour bgColour)
{
	this->X = posX;
	this->Y = posY;
	this->W = width;
	this->H = height;
	this->BackgroundColour = bgColour;
	this->TextColour = textColour;
}
