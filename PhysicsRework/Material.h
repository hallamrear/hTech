#pragma once

class Material
{
public:
	float Density;
	float Restitution;

	Material(float density, float restitution)
	{
		Density = density;
		Restitution = restitution;
	}

	static const Material Rock;
	static const Material Wood;
	static const Material Metal;
	static const Material BouncyBall;
	static const Material SuperBall;
	static const Material Pillow;
	static const Material Static;
};