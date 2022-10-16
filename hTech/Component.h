#pragma once
class Component
{
protected:
	bool mIsEnabled = true;
	std::string mComponentName = "Unnamed Component";
	Component(std::string componentName) : mComponentName(componentName) {};

public:
	virtual ~Component() = 0;

	virtual void SetIsEnabled(bool state);
	bool GetIsEnabled() const;
	std::string GetComponentName() const;
};

