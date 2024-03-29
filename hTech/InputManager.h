#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include <functional>
#include "Vector2.h"

typedef int32_t SDL_Keycode;

enum class HTECH_FUNCTION_EXPORT IM_KEY_STATE : int
{
	IM_KEY_PRESSED,
	IM_KEY_HELD,
	IM_KEY_RELEASED
};

enum class HTECH_FUNCTION_EXPORT IM_SCROLL_DIRECTION : int
{
	IM_SCROLL_UP = 1,
	IM_SCROLL_NONE = 0,
	IM_SCROLL_DOWN = -1
};

enum class HTECH_FUNCTION_EXPORT IM_KEY_CODE : int
{
	/*00*/ IM_KEY_1 = 0,
	/*01*/ IM_KEY_2,
	/*02*/ IM_KEY_3,
	/*03*/ IM_KEY_4,
	/*04*/ IM_KEY_5,
	/*05*/ IM_KEY_6,
	/*06*/ IM_KEY_7,
	/*07*/ IM_KEY_8,
	/*08*/ IM_KEY_9,
	/*09*/ IM_KEY_0,
	/*10*/ IM_KEY_UP_ARROW,
	/*11*/ IM_KEY_DOWN_ARROW,
	/*12*/ IM_KEY_LEFT_ARROW,
	/*13*/ IM_KEY_RIGHT_ARROW,
	/*14*/ IM_KEY_Q,
	/*15*/ IM_KEY_W,
	/*16*/ IM_KEY_E,
	/*17*/ IM_KEY_A,
	/*18*/ IM_KEY_S,
	/*19*/ IM_KEY_D,
	/*20*/ IM_KEY_Z,
	/*21*/ IM_KEY_X,
	/*22*/ IM_KEY_C,
	/*23*/ IM_KEY_SPACE,
	/*24*/ IM_KEY_F1,
	/*25*/ IM_KEY_F2,
	/*26*/ IM_KEY_F3,
	/*27*/ IM_KEY_F4,
	/*28*/ IM_KEY_F5,
	/*29*/ IM_KEY_F6,
	/*30*/ IM_KEY_F7,
	/*31*/ IM_KEY_F8,
	/*32*/ IM_KEY_F9,
	/*33*/ IM_KEY_F10,
	/*34*/ IM_KEY_F11,
	/*35*/ IM_KEY_F12
};

enum class HTECH_FUNCTION_EXPORT IM_MOUSE_CODE : int
{
	/*00*/ IM_MOUSE_SCROLL_UP = 0,
	/*00*/ IM_MOUSE_SCROLL_DOWN,
	/*01*/ IM_MOUSE_LEFT_CLICK,
	/*02*/ IM_MOUSE_RIGHT_CLICK,
	/*03*/ IM_MOUSE_MIDDLE_CLICK
};

class HTECH_FUNCTION_EXPORT Key
{
	std::function<void()> m_OnPressFunction;
	std::function<void()> m_OnHoldFunction;
	std::function<void()> m_OnReleaseFunction;

	IM_KEY_CODE m_KeyCode;
	bool m_PreviousState;
	bool m_KeyState = false;

public:
	Key(IM_KEY_CODE keycode)
	{
		m_KeyCode = keycode;
		m_KeyState = false;
		m_PreviousState = false;
		m_OnPressFunction = nullptr;
		m_OnReleaseFunction = nullptr;
	};

	IM_KEY_CODE	GetKeyCode()				 { return m_KeyCode; }
	const bool	GetState()					 { return m_KeyState; }
	      void	SetState(bool state)		 { m_KeyState = state; }
	      void	SetPreviousState(bool state) { m_PreviousState = state; };
	const bool	GetPreviousState()			 { return m_PreviousState; }

	void Bind(IM_KEY_STATE state, std::function<void()> func)
	{
		switch (state)
		{
		case IM_KEY_STATE::IM_KEY_PRESSED:
			if (func)
			{
				m_OnPressFunction = func;
			}
			break;

		case IM_KEY_STATE::IM_KEY_HELD:
			if(func)
			{
				m_OnHoldFunction = func;
			}
			break;

		case IM_KEY_STATE::IM_KEY_RELEASED:
			if (func)
			{
				m_OnReleaseFunction = func;
			}
			break;
		default:
			throw;
			break;
		}		
	};

	void RunOnPressFunction()
	{
		if (m_OnPressFunction)
			m_OnPressFunction();
	};

	void RunOnHeldFunction()
	{
		if (m_OnHoldFunction)
			m_OnHoldFunction();
	}

	void RunOnReleaseFunction()
	{
		if (m_OnReleaseFunction)
			m_OnReleaseFunction();
	}
};

class HTECH_FUNCTION_EXPORT Mouse
{
	std::function<void()> m_OnPressFunction;
	std::function<void()> m_OnHoldFunction;
	std::function<void()> m_OnReleaseFunction;

	IM_MOUSE_CODE m_KeyCode;
	bool m_PreviousState;
	bool m_KeyState = false;

public:
	Mouse(IM_MOUSE_CODE keycode)
	{
		m_KeyCode = keycode;
		m_KeyState = false;
		m_PreviousState = false;
		m_OnPressFunction = nullptr;
		m_OnReleaseFunction = nullptr;
	};

	IM_MOUSE_CODE	GetMouseCode() { return m_KeyCode; }
	bool			GetState() { return m_KeyState; }
	void			SetState(bool state) { m_KeyState = state; }
	void			SetPreviousState(bool state) { m_PreviousState = state; };
	bool			GetPreviousState() { return m_PreviousState; }

	void Bind(IM_KEY_STATE state, std::function<void()> func)
	{
		switch (state)
		{
		case IM_KEY_STATE::IM_KEY_PRESSED:
			if (func)
			{
				m_OnPressFunction = func;
			}
			break;

		case IM_KEY_STATE::IM_KEY_HELD:
			if (func)
			{
				m_OnHoldFunction = func;
			}
			break;

		case IM_KEY_STATE::IM_KEY_RELEASED:
			if (func)
			{
				m_OnReleaseFunction = func;
			}
			break;
		default:
			throw;
			break;
		}
	};

	void RunOnPressFunction()
	{
		if (m_OnPressFunction)
			m_OnPressFunction();
	};

	void RunOnHeldFunction()
	{
		if (m_OnHoldFunction)
			m_OnHoldFunction();
	}

	void RunOnReleaseFunction()
	{
		if (m_OnReleaseFunction)
			m_OnReleaseFunction();
	}
};

class HTECH_FUNCTION_EXPORT InputManager
{
private:
	static InputManager* m_Instance;

	const int m_KeyCount = 36;
	const int m_MouseCount = 5;
	bool m_IsMouseDown;
	Vector2 m_MousePosScreenSpace;
	Vector2 m_MousePosWorldSpace;

	Key m_KeyStates[36] =
	{
		/*00*/ IM_KEY_CODE::IM_KEY_1,
		/*01*/ IM_KEY_CODE::IM_KEY_2,
		/*02*/ IM_KEY_CODE::IM_KEY_3,
		/*03*/ IM_KEY_CODE::IM_KEY_4,
		/*04*/ IM_KEY_CODE::IM_KEY_5,
		/*05*/ IM_KEY_CODE::IM_KEY_6,
		/*06*/ IM_KEY_CODE::IM_KEY_7,
		/*07*/ IM_KEY_CODE::IM_KEY_8,
		/*08*/ IM_KEY_CODE::IM_KEY_9,
		/*09*/ IM_KEY_CODE::IM_KEY_0,
		/*10*/ IM_KEY_CODE::IM_KEY_UP_ARROW,
		/*11*/ IM_KEY_CODE::IM_KEY_DOWN_ARROW,
		/*12*/ IM_KEY_CODE::IM_KEY_LEFT_ARROW,
		/*13*/ IM_KEY_CODE::IM_KEY_RIGHT_ARROW,
		/*14*/ IM_KEY_CODE::IM_KEY_Q,
		/*15*/ IM_KEY_CODE::IM_KEY_W,
		/*16*/ IM_KEY_CODE::IM_KEY_E,
		/*17*/ IM_KEY_CODE::IM_KEY_A,
		/*18*/ IM_KEY_CODE::IM_KEY_S,
		/*19*/ IM_KEY_CODE::IM_KEY_D,
		/*20*/ IM_KEY_CODE::IM_KEY_Z,
		/*21*/ IM_KEY_CODE::IM_KEY_X,
		/*22*/ IM_KEY_CODE::IM_KEY_C,
		/*23*/ IM_KEY_CODE::IM_KEY_SPACE,
		/*24*/ IM_KEY_CODE::IM_KEY_F1,
		/*25*/ IM_KEY_CODE::IM_KEY_F2,
		/*26*/ IM_KEY_CODE::IM_KEY_F3,
		/*27*/ IM_KEY_CODE::IM_KEY_F4,
		/*28*/ IM_KEY_CODE::IM_KEY_F5,
		/*29*/ IM_KEY_CODE::IM_KEY_F6,
		/*30*/ IM_KEY_CODE::IM_KEY_F7,
		/*31*/ IM_KEY_CODE::IM_KEY_F8,
		/*32*/ IM_KEY_CODE::IM_KEY_F9,
		/*33*/ IM_KEY_CODE::IM_KEY_F10,
		/*34*/ IM_KEY_CODE::IM_KEY_F11,
		/*35*/ IM_KEY_CODE::IM_KEY_F12
	};

	Mouse m_MouseStates[5] =
	{
		/*00*/ IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP,
		/*01*/ IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN,
		/*02*/ IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK,
		/*03*/ IM_MOUSE_CODE::IM_MOUSE_RIGHT_CLICK,
		/*04*/ IM_MOUSE_CODE::IM_MOUSE_MIDDLE_CLICK
	};

	void Update_Impl();
	void Bind_Impl(IM_KEY_CODE keycode, IM_KEY_STATE keystate, std::function<void()> func);
	void Bind_Impl(IM_MOUSE_CODE keycode, IM_KEY_STATE keystate, std::function<void()> func);
	int FindKey(IM_MOUSE_CODE keycode);
	int FindKey(IM_KEY_CODE keycode);

	InputManager();
	~InputManager();
public:
	static InputManager* Get();
	static void Update();

	static void Bind(IM_KEY_CODE keycode, IM_KEY_STATE keystate, std::function<void()> func);
	static void Bind(IM_MOUSE_CODE keycode, IM_KEY_STATE keystate, std::function<void()> func);
	void KeyPressUpdate(SDL_Keycode key, bool state);
	void MousePressUpdate(SDL_Keycode key, bool state);

	void MousePositionUpdate(int x, int y);
	const Vector2& GetMouseScreenPosition();
	const Vector2& GetMouseWorldPosition();
	void MouseScrollUpdate(IM_SCROLL_DIRECTION direction);
};

