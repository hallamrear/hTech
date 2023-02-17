#include "pch.h"
#include "InputManager.h"
#include "Camera.h"
#include "Log.h"

InputManager* InputManager::m_Instance = nullptr;

InputManager::InputManager()
{
	m_IsMouseDown = false;
	m_MousePosScreenSpace = Vector2();
	m_MousePosWorldSpace = Vector2();
}

InputManager::~InputManager()
{

}

void InputManager::Bind_Impl(IM_MOUSE_CODE keycode, IM_KEY_STATE mouseState, std::function<void()> func)
{
	if (func)
	{
		for (int i = 0; i < m_MouseCount; i++)
		{
			if (m_MouseStates[i].GetMouseCode() == keycode)
			{
				m_MouseStates[i].Bind(mouseState, func);
				return;
			}
		}
	}
	else
		Log::LogMessage(LogLevel::LOG_ERROR, "FAILED TO BIND FUNCTION TO MOUSECODE REFERENCE");
}

void InputManager::Bind_Impl(IM_KEY_CODE keycode, IM_KEY_STATE keystate, std::function<void()> func)
{
	if (func)
	{
		for (int i = 0; i < m_KeyCount; i++)
		{
			if (m_KeyStates[i].GetKeyCode() == keycode)
			{
				m_KeyStates[i].Bind(keystate, func);
				return;
			}
		}
	}
	else
		Log::LogMessage(LogLevel::LOG_ERROR, "FAILED TO BIND FUNCTION TO KEYCODE REFERENCE");
}

void InputManager::Bind(IM_KEY_CODE mouseCode, IM_KEY_STATE mouseState, std::function<void()> func)
{
	Get()->Bind_Impl(mouseCode, mouseState, func);
}

/*
 * Do not bind anything to mousescrollup/mousescrolldown with keystate held.
 */
void InputManager::Bind(IM_MOUSE_CODE keycode, IM_KEY_STATE keystate, std::function<void()> func)
{
	Get()->Bind_Impl(keycode, keystate, func);
}

void InputManager::Update_Impl()
{
	for (int i = 0; i < m_KeyCount; i++)
	{
		//Key Press
		if (m_KeyStates[i].GetState() == true && m_KeyStates[i].GetPreviousState() == false)
		{
			m_KeyStates[i].RunOnPressFunction();
		}

		if (m_KeyStates[i].GetState() == true)
		{
			m_KeyStates[i].RunOnHeldFunction();
		}

		//Key Release
		else if (m_KeyStates[i].GetState() == false && m_KeyStates[i].GetPreviousState() == true)
		{
			m_KeyStates[i].RunOnReleaseFunction();
		}

		m_KeyStates[i].SetPreviousState(m_KeyStates[i].GetState());
	}

	for (int i = 0; i < m_MouseCount; i++)
	{
		//Mouse Press
		if (m_MouseStates[i].GetState() == true && m_MouseStates[i].GetPreviousState() == false)
		{
			m_MouseStates[i].RunOnPressFunction();
		}

		if (m_MouseStates[i].GetState() == true)
		{
			if (m_MouseStates[i].GetMouseCode() != IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP || m_MouseStates[i].GetMouseCode() != IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN)
			{
				m_MouseStates[i].RunOnHeldFunction();
			}
		}

		//Mouse Release
		else if (m_MouseStates[i].GetState() == false && m_MouseStates[i].GetPreviousState() == true)
		{
			m_MouseStates[i].RunOnReleaseFunction();
		}

		m_MouseStates[i].SetPreviousState(m_MouseStates[i].GetState());
	}
}

void InputManager::Update()
{
	Get()->Update_Impl();
}

int InputManager::FindKey(IM_MOUSE_CODE keycode)
{
	for (int i = 0; i < m_MouseCount; i++)
	{
		if (m_MouseStates[i].GetMouseCode() == keycode)
			return i;
	}

	return NULL;
}

int InputManager::FindKey(IM_KEY_CODE keycode)
{
	for (int i = 0; i < m_KeyCount; i++)
	{
		if (m_KeyStates[i].GetKeyCode() == keycode)
			return i;
	}

	return NULL;
}

void InputManager::KeyPressUpdate(SDL_Keycode key, bool state)
{
	switch (key)
	{
	case SDLK_0:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_0)].SetState(state);
		break;

	case SDLK_1:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_1)].SetState(state);
		break;

	case SDLK_2:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_2)].SetState(state);
		break;

	case SDLK_3:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_3)].SetState(state);
		break;

	case SDLK_4:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_4)].SetState(state);
		break;

	case SDLK_5:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_5)].SetState(state);
		break;

	case SDLK_6:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_6)].SetState(state);
		break;

	case SDLK_7:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_7)].SetState(state);
		break;

	case SDLK_8:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_8)].SetState(state);
		break;

	case SDLK_9:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_9)].SetState(state);
		break;

	case SDLK_UP:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_UP_ARROW)].SetState(state);
		break;

	case SDLK_DOWN:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_DOWN_ARROW)].SetState(state);
		break;

	case SDLK_LEFT:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_LEFT_ARROW)].SetState(state);
		break;

	case SDLK_RIGHT:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_RIGHT_ARROW)].SetState(state);
		break;

	case SDLK_w:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_W)].SetState(state);
		break;

	case SDLK_a:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_A)].SetState(state);
		break;

	case SDLK_s:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_S)].SetState(state);
		break;

	case SDLK_d:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_D)].SetState(state);
		break;

	case SDLK_SPACE:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_SPACE)].SetState(state);
		break;

	case SDLK_q:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_Q)].SetState(state);
		break;

	case SDLK_e:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_E)].SetState(state);
		break;

	case SDLK_z:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_Z)].SetState(state);
		break;

	case SDLK_x:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_X)].SetState(state);
		break;

	case SDLK_c:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_C)].SetState(state);
		break;

	case SDLK_F1:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_F1)].SetState(state);
		break;

	case SDLK_F2:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_F2)].SetState(state);
		break;

	case SDLK_F3:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_F3)].SetState(state);
		break;

	case SDLK_F4:
		m_KeyStates[FindKey(IM_KEY_CODE::IM_KEY_F4)].SetState(state);
		break;
	}
}

void InputManager::MousePressUpdate(SDL_Keycode key, bool state)
{
	switch (key)
	{
	case SDL_BUTTON_LEFT:
		m_MouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK)].SetState(state);
		break;

	case SDL_BUTTON_MIDDLE:
		m_MouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_MIDDLE_CLICK)].SetState(state);
		break;

	case SDL_BUTTON_RIGHT:
		m_MouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_RIGHT_CLICK)].SetState(state);
		break;
	}
}

InputManager* InputManager::Get()
{
	if (!m_Instance)
		m_Instance = new InputManager();

	return m_Instance;
}

void InputManager::MousePositionUpdate(int x, int y)
{
	m_MousePosScreenSpace.X = (float)x;
	m_MousePosScreenSpace.Y = (float)y;
}

const Vector2& InputManager::GetMouseScreenPosition()
{
	return m_MousePosScreenSpace;
}

const Vector2& InputManager::GetMouseWorldPosition()
{
	m_MousePosWorldSpace = Camera::ScreenToWorld(m_MousePosScreenSpace);
	return m_MousePosWorldSpace;
}

void InputManager::MouseScrollUpdate(IM_SCROLL_DIRECTION direction)
{
	switch (direction)
	{
	case IM_SCROLL_DIRECTION::IM_SCROLL_UP:
		m_MouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP)].RunOnPressFunction();
		break;
	case IM_SCROLL_DIRECTION::IM_SCROLL_DOWN:
		m_MouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN)].RunOnPressFunction();
		break;
	default:
		break;
	}
}
