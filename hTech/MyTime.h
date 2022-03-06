#pragma once

//todo : release functions for all the singletons

class Time
{
private:
	//todo : put singleton back
	//static Time* mInstance;
	double& mDeltaTime;

	inline Time(double& dt) 
		: mDeltaTime(dt)
	{

	}

	inline ~Time()
	{
		mDeltaTime = 0.0;
	}

public:
	static Time* Get(double& dt)
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

	static const double DeltaTime()
	{
		double d;
		return Get(d)->mDeltaTime;
	}
};

