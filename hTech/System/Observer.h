#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

class HTECH_FUNCTION_EXPORT Observer
{
protected:
	Observer();

public:
	virtual ~Observer() = 0;
	virtual void OnNotify() = 0;
};