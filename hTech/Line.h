#pragma once
#include "Vector2.h"

template<class P>
struct TwoPointLine
{
	P A;
	P B;

	TwoPointLine();
	TwoPointLine(P a, P b);

	Vector2 GetNormal() const
	{
		float dx = B.X - A.X;
		float dy = B.Y - A.Y;
		return Vector2(-dy, dx).GetNormalized();
	}

	Vector2 GetCentrePoint() const
	{
		return Vector2((A.X + B.X) / 2.0f, (A.Y + B.Y) / 2.0f);
	}

	TwoPointLine& operator=(const TwoPointLine& line)
	{
		A = line.A;
		B = line.B;
		return *this;
	};

	void Render(SDL_Renderer& renderer)
	{
		SDL_RenderDrawLine(&renderer, A.X, A.Y, B.X, B.Y);
	}
};

typedef TwoPointLine<Point&> Edge;
typedef TwoPointLine<Point> Line;

template<class P>
inline TwoPointLine<P>::TwoPointLine(P a, P b) : A(a), B(b)
{

}