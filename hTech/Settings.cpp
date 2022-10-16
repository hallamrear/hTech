#include "pch.h"
#include "Settings.h"

//todo : put singleton back
//Settings* Settings::mInstance = nullptr;

Settings::Settings()
{
	mMaxLogMessages = 20;
	mDrawLog = false;
	mWindowDimensions = Vector2(1280.0f, 720.0f);
	mWindowCentre = Vector2(mWindowDimensions.X / 2.0f, mWindowDimensions.Y / 2.0f);
	mDrawColliders = false;
	mGravityDirection = Vector2(0.0f, -9.81f);
}

Settings::~Settings()
{
	mDrawColliders = false;
}

Settings* Settings::Get()
{
	//todo : put singleton back
	//if (!mInstance)
	//	mInstance = new Settings();

	static Settings mInstance;
	return &mInstance;
}

bool Settings::GetDrawLog() const
{
	return mDrawLog;
}

void Settings::SetDrawLog(bool state)
{
	mDrawLog = state;
}

Vector2 Settings::GetWindowCentre() const
{
	return mWindowCentre;
}

Vector2 Settings::GetWindowDimensions() const
{
	return mWindowDimensions;
}

bool Settings::GetDrawColliders() const
{
	return mDrawColliders;
}

void Settings::SetDrawColliders(bool state)
{
	mDrawColliders = state;
}

Vector2 Settings::GetGravityDirection() const
{
	return mGravityDirection;
}

void Settings::SetGravityDirection(Vector2 direction)
{
	mGravityDirection = direction;
}

void Settings::SetMaxLogMessages(int count)
{
	mMaxLogMessages = count;
}

int Settings::GetMaxLogMessages() const
{
	return mMaxLogMessages;
}

void Settings::SetWindowDimensions(Vector2 dimensions)
{
	mWindowDimensions = dimensions;
	mWindowCentre = Vector2(mWindowDimensions.X / 2.0f, mWindowDimensions.Y / 2.0f);
}
