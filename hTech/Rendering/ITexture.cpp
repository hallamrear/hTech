#include "pch.h"
#include "Rendering/ITexture.h"
#include "Rendering/Camera.h"
#include "System/Engine.h"
#include "System/Console.h"
#include "System/ProjectLoader.h"
#include "DataTypes/Transform.h"
#include "Rendering/OriginalRenderer.h"

bool ITexture::Create(const std::string& texture_path, const std::string& name)
{
	return false;
}

bool ITexture::Destroy()
{
	return false;
}

ITexture::ITexture(const std::string& texture_path, const std::string& name)
{
	m_Width = NULL;
	m_Height = NULL;
	m_Name = "";
	m_Path = "";
	Create(texture_path, name);
}

ITexture::~ITexture()
{
	m_Width = NULL;
	m_Height = NULL;
	Destroy();
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
