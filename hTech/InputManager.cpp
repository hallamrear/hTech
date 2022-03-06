#include "pch.h"
#include "InputManager.h"
#include "Camera.h"
#include "Log.h"

//todo : put singleton back
//InputManager* InputManager::mInstance = nullptr;

InputManager::InputManager()
{
	mIsMouseDown = false;
	mMousePositionX = -1;
	mMousePositionY = -1;
}

InputManager::~InputManager()
{
	//todo : fin
}

void InputManager::Bind_Impl(IM_MOUSE_CODE keycode, IM_KEY_STATE mouseState, std::function<void()> func)
{
	if (func)
	{
		for (int i = 0; i < mMouseCount; i++)
		{
			if (mMouseStates[i].GetMouseCode() == keycode)
			{
				mMouseStates[i].Bind(mouseState, func);
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
		for (int i = 0; i < mKeyCount; i++)
		{
			if (mKeyStates[i].GetKeyCode() == keycode)
			{
				mKeyStates[i].Bind(keystate, func);
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
	for (int i = 0; i < mKeyCount; i++)
	{
		//Key Press
		if (mKeyStates[i].GetState() == true && mKeyStates[i].GetPreviousState() == false)
		{
			mKeyStates[i].RunOnPressFunction();
		}

		if (mKeyStates[i].GetState() == true)
		{
			mKeyStates[i].RunOnHeldFunction();
		}

		//Key Release
		else if (mKeyStates[i].GetState() == false && mKeyStates[i].GetPreviousState() == true)
		{
			mKeyStates[i].RunOnReleaseFunction();
		}

		mKeyStates[i].SetPreviousState(mKeyStates[i].GetState());
	}

	for (int i = 0; i < mMouseCount; i++)
	{
		//Mouse Press
		if (mMouseStates[i].GetState() == true && mMouseStates[i].GetPreviousState() == false)
		{
			mMouseStates[i].RunOnPressFunction();
		}

		if (mMouseStates[i].GetState() == true)
		{
			if (mMouseStates[i].GetMouseCode() != IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP || mMouseStates[i].GetMouseCode() != IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN)
			{
				mMouseStates[i].RunOnHeldFunction();
			}
		}

		//Mouse Release
		else if (mMouseStates[i].GetState() == false && mMouseStates[i].GetPreviousState() == true)
		{
			mMouseStates[i].RunOnReleaseFunction();
		}

		mMouseStates[i].SetPreviousState(mMouseStates[i].GetState());
	}
}

void InputManager::Update()
{
	Get()->Update_Impl();
}

int InputManager::FindKey(IM_MOUSE_CODE keycode)
{
	for (int i = 0; i < mMouseCount; i++)
	{
		if (mMouseStates[i].GetMouseCode() == keycode)
			return i;
	}

	return NULL;
}

int InputManager::FindKey(IM_KEY_CODE keycode)
{
	for (int i = 0; i < mKeyCount; i++)
	{
		if (mKeyStates[i].GetKeyCode() == keycode)
			return i;
	}

	return NULL;
}

void InputManager::KeyPressUpdate(SDL_Keycode key, bool state)
{
	switch (key)
	{
	case SDLK_0:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_0)].SetState(state);
		break;

	case SDLK_1:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_1)].SetState(state);
		break;

	case SDLK_2:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_2)].SetState(state);
		break;

	case SDLK_3:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_3)].SetState(state);
		break;

	case SDLK_4:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_4)].SetState(state);
		break;

	case SDLK_5:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_5)].SetState(state);
		break;

	case SDLK_6:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_6)].SetState(state);
		break;

	case SDLK_7:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_7)].SetState(state);
		break;

	case SDLK_8:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_8)].SetState(state);
		break;

	case SDLK_9:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_9)].SetState(state);
		break;

	case SDLK_UP:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_UP_ARROW)].SetState(state);
		break;

	case SDLK_DOWN:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_DOWN_ARROW)].SetState(state);
		break;

	case SDLK_LEFT:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_LEFT_ARROW)].SetState(state);
		break;

	case SDLK_RIGHT:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_RIGHT_ARROW)].SetState(state);
		break;

	case SDLK_w:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_W)].SetState(state);
		break;

	case SDLK_a:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_A)].SetState(state);
		break;

	case SDLK_s:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_S)].SetState(state);
		break;

	case SDLK_d:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_D)].SetState(state);
		break;

	case SDLK_SPACE:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_SPACE)].SetState(state);
		break;

	case SDLK_q:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_Q)].SetState(state);
		break;

	case SDLK_e:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_E)].SetState(state);
		break;

	case SDLK_z:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_Z)].SetState(state);
		break;

	case SDLK_x:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_X)].SetState(state);
		break;

	case SDLK_c:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_C)].SetState(state);
		break;

	case SDLK_F1:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_F1)].SetState(state);
		break;

	case SDLK_F2:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_F2)].SetState(state);
		break;

	case SDLK_F3:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_F3)].SetState(state);
		break;

	case SDLK_F4:
		mKeyStates[FindKey(IM_KEY_CODE::IM_KEY_F4)].SetState(state);
		break;
	}
}

void InputManager::MousePressUpdate(SDL_Keycode key, bool state)
{
	switch (key)
	{
	case SDL_BUTTON_LEFT:
		mMouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK)].SetState(state);
		break;

	case SDL_BUTTON_MIDDLE:
		mMouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_MIDDLE_CLICK)].SetState(state);
		break;

	case SDL_BUTTON_RIGHT:
		mMouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_RIGHT_CLICK)].SetState(state);
		break;
	}
}

InputManager* InputManager::Get()
{
	//todo : put singleton back
	//if (!mInstance)
	//	mInstance = new InputManager();

	static InputManager mInstance;
	return &mInstance;
}

void InputManager::MousePositionUpdate(int x, int y)
{
	mMousePositionX = x;
	mMousePositionY = y;
}

Vector2f InputManager::GetMouseScreenPosition()
{
	return Vector2f((float)mMousePositionX, (float)mMousePositionY);
}

Vector2f InputManager::GetMouseWorldPosition()
{
	return Camera::ScreenToWorld(GetMouseScreenPosition());
}

void InputManager::MouseScrollUpdate(IM_SCROLL_DIRECTION direction)
{
	switch (direction)
	{
	case IM_SCROLL_DIRECTION::IM_SCROLL_UP:
		mMouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP)].RunOnPressFunction();
		break;
	case IM_SCROLL_DIRECTION::IM_SCROLL_DOWN:
		mMouseStates[FindKey(IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN)].RunOnPressFunction();
		break;
	default:
		break;
	}
}
