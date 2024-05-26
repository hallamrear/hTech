#include "pch.h"
#include "Rendering/IWindow.h"
#include "System/WindowDetails.h"

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
