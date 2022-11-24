#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

//todo : release functions for all the singletons

class HTECH_FUNCTION_EXPORT Time
{
private:
	//todo : put singleton back
	//static Time* mInstance;
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
		//todo : put singleton back
		//if (!mInstance)
		//	mInstance = new Time(dt);

		static Time mInstance(dt);
		return &mInstance;
	}

	static void Release()
	{
		//todo : put singleton back
		//if (mInstance)
		//{
		//	delete mInstance;
		//	mInstance = nullptr;
		//}
	}

	static float& DeltaTime()
	{
		float d;
		return Get(d)->mDeltaTime;
	}
};