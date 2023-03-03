#include "pch.h"
#include "ObserverSubject.h"
#include "Observer.h"

ObserverSubject::ObserverSubject()
{
}

ObserverSubject::~ObserverSubject()
{
	for (size_t i = 0; i < m_Observers.size(); i++)
	{
		if (m_Observers[i])
		{
			delete m_Observers[i];
			m_Observers[i] = nullptr;
		}
	}

	m_Observers.clear();
}

void ObserverSubject::AddObserver(Observer* observer)
{
	if (observer != nullptr)
	{
		m_Observers.push_back(observer);
	}
}

void ObserverSubject::RemoveObserver(Observer* observer)
{
	if (observer != nullptr)
	{
		auto itr =  std::find(m_Observers.begin(), m_Observers.end(), observer);

		if (itr != m_Observers.end())
		{
			m_Observers.erase(itr);
		}
	}
}

void ObserverSubject::NotifyAll()
{
	for (size_t i = 0; i < m_Observers.size(); i++)
	{
		m_Observers[i]->OnNotify();
	}
}
