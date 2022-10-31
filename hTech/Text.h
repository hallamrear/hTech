#pragma once
#include "Colour.h"

class Text
{
private:
	static TTF_Font* mFont;
	static bool CreateTTFFontAsset(const int& fontSize = 16.0f, const std::string& fontLocation = "Assets/arial.ttf");
	static bool DestroyTTFFontAsset();

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

	void SetPosition(const Vector2& screenSpacePosition);
	void SetString(const std::string& str);
	void SetColour(const Colour& colour);
	void SetString(const char* str);

	const Vector2 GetTextureSize() const;


};

