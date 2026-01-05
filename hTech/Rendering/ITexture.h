#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

class Transform;

class HTECH_FUNCTION_EXPORT Vector2;

class HTECH_FUNCTION_EXPORT ITexture
{
private:

protected:
	bool m_Exists;
	std::string m_Name;
	std::string m_Path;
	int m_Width;
	int m_Height;

	virtual bool Create(const unsigned int& width, const unsigned int& height) = 0;
	virtual bool Load(const std::string& texture_path, const std::string& name) = 0;
	virtual bool Destroy() = 0;
	ITexture();

public:
	~ITexture();

	const bool& Exists() const;

	const std::string& GetLocation() const;
	const std::string& GetName() const;

	const int& GetWidth() const;
	const int& GetHeight() const;
};