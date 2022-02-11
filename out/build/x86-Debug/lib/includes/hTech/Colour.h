#pragma once
typedef uint8_t Uint8;

class HTECH_API Colour
{
public:
	Uint8 R;
	Uint8 G;
	Uint8 B;
	Uint8 A;

	Colour();
	Colour(Uint8 R, Uint8 G, Uint8 B, Uint8 A);
};

