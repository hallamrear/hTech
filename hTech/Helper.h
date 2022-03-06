#pragma once
#include <random>
#include "Vector2.h"
#include "Vector3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//MYAPI HandVal StdDeck_StdRules_EVAL_N(StdDeck_CardMask cards, int n_cards)
//
//__declspec(dllexport) int Public_StdDeck_StdRules_EVAL_N(StdDeck_CardMask cards, int n_cards)
//{
//    return StdDeck_StdRules_EVAL_N(cards, n_cards);
//}

namespace
{
	////Works equally each direction from 0
	////e.g. GetRandomIntExcludingCentre(256, 128)
	////		can return between -256 -> -128 & 128 -> 256
	static inline int GetRandomIntExcludingCentre(int upperBound, int lowerBound)
	{
		return 0;
	};

	static inline float ConvertToRadians(float degrees)
	{
	return 0;
	};

	static inline float ConvertToDegrees(float radians)
	{
	return 0;
	};

	////todo : make reference to store in point rather than returning
	static inline Vector2f RotatePointAroundOriginRadians(Vector2f point, float rotation, Vector2f origin)
	{
		return Vector2f();
	};

	static inline Vector2f RotatePointAroundOriginDegrees(Vector2f point, float rotation, Vector2f origin)
	{
		return Vector2f();
	};

	static inline Vector2f LerpPoint(Vector2f start, Vector2f end, float time)
	{
		return Vector2f();
	};
	////Returns -1 if val < 0, 1 if val > 0, 0 if val == 0
	//
	static inline int Sign(int val)
	{
		return 0;
	};

	static inline float Dot(Vector2f a, Vector2f b)
	{
		return 0.0f;
	};
	//#endif
}