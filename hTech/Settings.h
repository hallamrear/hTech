#pragma once
#include "Vector2.h"

class Settings
{
private:
	//todo : put singleton back
	//static Settings* mInstance;

	Settings();
	~Settings();

	int		 mMaxLogMessages;
	bool	 mDrawColliders;
	bool	 mDrawLog;
	Vector2 mWindowCentre;
	Vector2 mWindowDimensions;
	Vector2 mGravityDirection;

public:
	static Settings* Get();

	bool GetDrawLog() const;
	void SetDrawLog(bool state);
	bool GetDrawColliders() const;
	void SetDrawColliders(bool state);
	Vector2 GetWindowCentre() const;
	Vector2 GetWindowDimensions() const;
	Vector2 GetGravityDirection() const;
	void SetGravityDirection(Vector2 direction);
	void SetWindowDimensions(Vector2 dimensions);
	void SetMaxLogMessages(int count);
	int GetMaxLogMessages() const;
};

