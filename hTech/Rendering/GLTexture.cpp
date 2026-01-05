#include "pch.h"
#include "Rendering/GLTexture.h"
#include <External/GLEW/include/GL/glew.h>
#include <External/STB/include/stb_image.h>
#include <System/Console.h>

GLTexture::GLTexture()
	: ITexture()
{
	m_TextureID = GL_INVALID_VALUE;
}

GLTexture::~GLTexture()
{
	Destroy();
}

const unsigned int& GLTexture::GetID() const
{
	return m_TextureID;
}

bool GLTexture::Create(const unsigned int& width, const unsigned int& height)
{
	//todo : implement
	assert_func_not_implemented;
	return false;
}

bool GLTexture::Load(const std::string& texture_path, const std::string& name)
{
	if (texture_path == "")
	{
		return false;
	}

	//Generate a texture ID.
	glGenTextures(1, &m_TextureID);

	//Bind it to edit.
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	int nrChannels;
	unsigned char* data = stbi_load(texture_path.c_str(), &m_Width, &m_Height, &nrChannels, 0);

	if (data)
	{
		//Create image object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// set wrapping/filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		Console::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load image <" + texture_path + "> \n"));
		stbi_image_free(data);
		return false;
	}

	stbi_image_free(data);

	m_Path = texture_path;
	m_Name = name;
	m_Exists = true;
	return true;
}

bool GLTexture::Destroy()
{
	glDeleteTextures(1, &m_TextureID);

	m_TextureID = GL_INVALID_VALUE;
	m_Width = NULL;
	m_Height = NULL;
	m_Name = "";
	m_Path = "";
	m_Exists = false;

	return true;
}