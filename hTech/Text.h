#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Colour.h"
#include "Transform.h"

class HTECH_FUNCTION_EXPORT Text
{
private:
	static bool CreateTTFFontAsset(const int& fontSize = 14.0f, const std::string& fontLocation = "Assets/arial.ttf");
	static bool DestroyTTFFontAsset();

	SDL_Texture* mTextTexture;
	static TTF_Font* m_Font;
	int m_WrapWidth = 0;
	Transform m_Transform;
	std::string m_Data;
	Colour m_Colour;
	bool m_IsDirty;
	int m_Height;
	int m_Width;

	void CreateTextTexture();
	void DestroyTextTexture();


public:
	Text(Transform transform = Transform(), std::string text = "Empty.", Colour colour = Colour());
	~Text();

	void Update(float DeltaTime);
	void Render(IRenderer& renderer);

	/// <summary>
	/// Text is wrapped on multiple lines on line endings and word boundaries if it extends past WrapWidth.
	/// </summary>
	/// <param name="width">If wrapLength is 0, this function will only wrap on newline characters.</param>
	void SetWrapWidthInPixels(int width);
	void SetPosition(const Vector2& screenSpacePosition);
	void SetString(const std::string& str);
	void SetColour(const Colour& colour);
	void SetString(const char* str);

	const Vector2 GetTextureSize() const;

};

