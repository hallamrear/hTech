#include "pch.h"
#include "UI_PanelRect.h"

PanelRect::PanelRect()
{
}

PanelRect::PanelRect(unsigned short posX, unsigned short posY, unsigned short width, unsigned short height, Colour textColour, Colour bgColour)
{
	this->X = posX;
	this->Y = posY;
	this->W = width;
	this->H = height;
	this->BackgroundColour = bgColour;
	this->TextColour = textColour;
}
