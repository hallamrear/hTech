#pragma once
#include "Vector2.h"

struct IRenderer;

template<class P>
struct TwoPointLine
{
	P A;
	P B;

	TwoPointLine();
	TwoPointLine(P a, P b);

	Vector2 GetNormal() const;
	Vector2 GetCentrePoint() const;
	TwoPointLine& operator=(const TwoPointLine& line);
	void Render(IRenderer& renderer);
};

typedef TwoPointLine<Point&> Edge;
typedef TwoPointLine<Point> Line;

template<class P>
inline TwoPointLine<P>::TwoPointLine(P a, P b) : A(a), B(b)
{

}