#pragma once

class IShader
{
private:
	

protected:
	enum class SHADER_TYPE : int
	{
		VERTEX = 0,
		PIXEL = 1
	};

	unsigned int m_ID;

public:
	virtual ~IShader() = 0;

	const unsigned int& GetID() const;

	virtual bool Create(const char* pixelShaderSource, const char* vertexShaderSource) = 0;
	virtual void Destroy() = 0;
	virtual void Bind() const = 0;
	virtual void SetBool(const std::string& name, bool value) const = 0;
	virtual void SetInt(const std::string& name, int value) const = 0;
	virtual void SetFloat(const std::string& name, float value) const = 0;
};

