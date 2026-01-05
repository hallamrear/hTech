#include "pch.h"
#include "Rendering/ITexture.h"
#include "Rendering/Camera.h"
#include "System/Engine.h"
#include "System/Console.h"
#include "System/ProjectLoader.h"
#include "DataTypes/Transform.h"

ITexture::ITexture()
{
	m_Exists = false;
	m_Width = NULL;
	m_Height = NULL;
	m_Name = "";
	m_Path = "";
}

ITexture::~ITexture()
{
	
}

const bool& ITexture::Exists() const
{
	return m_Exists;
}

const std::string& ITexture::GetLocation() const
{
	return m_Path;
}

const std::string& ITexture::GetName() const
{
	return m_Name;
}

const int& ITexture::GetWidth() const
{
	return m_Width;
}

const int& ITexture::GetHeight() const
{
	return m_Height;
}
