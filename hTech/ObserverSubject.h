#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

class HTECH_FUNCTION_EXPORT Observer;

class HTECH_FUNCTION_EXPORT ObserverSubject
{
private:
	std::vector<Observer*> m_Observers;

protected:
	ObserverSubject();

public:
	virtual ~ObserverSubject() = 0;
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

	virtual void NotifyAll();
};
