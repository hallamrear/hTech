#pragma once
#include "Colour.h"

class Text
{
private:
	static TTF_Font* mFont;
	static bool CreateTTFFontAsset(const int& fontSize = 16.0f, const std::string& fontLocation = "Assets/arial.ttf");
	static bool DestroyTTFFontAsset();

	int mWrapWidth = 0;
	SDL_Texture* mTextTexture;
	Vector2 mPosition;
	std::string mData;
	bool mIsDirty;
	int mTextWidth;
	int mTextHeight;
	Colour mColour;

	void CreateTextTexture();
	void DestroyTextTexture();


public:
	Text(Vector2 position = Vector2(), std::string text = "Empty.", Colour colour = Colour());
	~Text();

	void Update(float DeltaTime);
	void Render(SDL_Renderer& renderer);

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

