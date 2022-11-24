#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



class HTECH_FUNCTION_EXPORT Vector3
{
public:
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;

	Vector3(float X = 0.0f, float Y = 0.0f, float Z = 0.0f)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	~Vector3()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}
};



