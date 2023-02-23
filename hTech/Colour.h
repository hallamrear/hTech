#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

typedef uint8_t Uint8;

class HTECH_FUNCTION_EXPORT Colour
{
public:
	static const Colour Red;
	static const Colour Green;
	static const Colour Blue;
	static const Colour White;


	Uint8 R;
	Uint8 G;
	Uint8 B;
	Uint8 A;

	Colour();
	Colour(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};