#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

//todo : release functions for all the singletons

class HTECH_FUNCTION_EXPORT Time
{
private:
	static Time* m_Instance;
	float& m_DeltaTime;

	inline Time(float& dt)
		: m_DeltaTime(dt)
	{

	}

	inline ~Time()
	{
		m_DeltaTime = 0.0;
	}

public:
	static Time* Get(float& dt)
	{
		if (!m_Instance)
			m_Instance = new Time(dt);

		return m_Instance;
	}

	static void Release()
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = nullptr;
		}
	}

	static float& DeltaTime()
	{
		float d;
		return Get(d)->m_DeltaTime;
	}
};