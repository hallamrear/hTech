#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

//todo : release functions for all the singletons

class HTECH_FUNCTION_EXPORT Time
{
private:
	static Time* mInstance;
	float& mDeltaTime;

	inline Time(float& dt)
		: mDeltaTime(dt)
	{

	}

	inline ~Time()
	{
		mDeltaTime = 0.0;
	}

public:
	static Time* Get(float& dt)
	{
		if (!mInstance)
			mInstance = new Time(dt);

		return mInstance;
	}

	static void Release()
	{
		if (mInstance)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	static float& DeltaTime()
	{
		float d;
		return Get(d)->mDeltaTime;
	}
};