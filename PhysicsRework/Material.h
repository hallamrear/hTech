#pragma once

class Material
{
public:

	float Density = 0.0f;
	float Restitution = 0.0f;
	float StaticFriction = 0.4f;
	float DynamicFriction = 0.2f;

	Material()
	{
	}

	Material(float density, float restitution, float staticFriction = 0.4f, float dynamicFriction = 0.2f)
	{
		Density = density;
		Restitution = restitution;
		StaticFriction = staticFriction;
		DynamicFriction = dynamicFriction;
	}
};

//const Material Rock = Material(0.6f, 0.1f);
//const Material Wood = Material(0.3f, 0.2f);
//const Material Metal = Material(1.2f, 0.05f);
//const Material BouncyBall = Material(0.3f, 0.8f);
//const Material SuperBall = Material(0.3f, 0.95f);
//const Material Pillow = Material(0.1f, 0.2f);
//const Material Static = Material(0.0f, 0.4f);