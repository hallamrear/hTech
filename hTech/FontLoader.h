#pragma once

class FontLoader
{
private:
	std::string m_DefaultFontLocation = "Assets\\Arial.ttf";

	FontLoader();

	static FontLoader* m_Instance;
	std::unordered_map<int, TTF_Font*> m_FontMap;

	TTF_Font* GetFont_Impl(const int& fontSize);
	void UnloadAll_Impl();
	void RenderProperties_Impl();
	static FontLoader* Get();

public:
	~FontLoader();
	static inline TTF_Font* GetFont(const int& fontSize) { return Get()->GetFont_Impl(fontSize); };
	static inline void UnloadAll() { return Get()->UnloadAll_Impl(); };
	static inline void RenderProperties() { return Get()->RenderProperties_Impl(); };
};

