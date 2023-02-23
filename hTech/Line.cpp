#include "pch.h"
#include "Line.h"

template<class P>
Vector2 TwoPointLine<P>::GetNormal() const
{
	float dx = B.X - A.X;
	float dy = B.Y - A.Y;
	return Vector2(-dy, dx).GetNormalized();
}

template<class P>
Vector2 TwoPointLine<P>::GetCentrePoint() const
{
	return Vector2((A.X + B.X) / 2.0f, (A.Y + B.Y) / 2.0f);
}

template<class P>
TwoPointLine<P>& TwoPointLine<P>::operator=(const TwoPointLine<P>& line)
{
	A = line.A;
	B = line.B;
	return *this;
};

template<class P>
void TwoPointLine<P>::Render(IRenderer& renderer)
{
	SDL_RenderDrawLine(&renderer, A.X, A.Y, B.X, B.Y);
}