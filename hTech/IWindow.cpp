#include "pch.h"
#include "IWindow.h"

IWindow::IWindow()
{
	m_IsInitialised = false;
}

IWindow::~IWindow()
{

}

const bool IWindow::IsInitialised() const
{
	return m_IsInitialised;
}
