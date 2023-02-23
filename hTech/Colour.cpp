#include "pch.h"
#include "Colour.h"

const Colour Colour::Red = Colour(255, 0, 0, 255);
const Colour Colour::Green = Colour(0, 255, 0, 255);
const Colour Colour::Blue = Colour(0, 0, 255, 255);
const Colour Colour::White = Colour(255, 255, 255, 255);

/// <summary>
/// Default instantiation is white
/// </summary>
Colour::Colour()
{
	R = 255;
	G = 255;
	B = 255;
	A = 255;
}

Colour::Colour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	this->R = r;
	this->G = g;
	this->B = b;
	this->A = a;
}