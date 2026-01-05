#include "pch.h"
#include "IShader.h"

IShader::~IShader()
{

}

const unsigned int& IShader::GetID() const
{
	return m_ID;
}