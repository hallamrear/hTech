#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



typedef uint8_t Uint8;

class HTECH_FUNCTION_EXPORT Colour
{
public:
	Uint8 R;
	Uint8 G;
	Uint8 B;
	Uint8 A;

	Colour();
	Colour(Uint8 R, Uint8 G, Uint8 B, Uint8 A);
};

