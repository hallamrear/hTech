#pragma once
#include "Component.h"

class Entity;

class TextComponent:
    public Component
{
private:
	Vector2 m_Offset;
	SDL_Texture* m_TextTexture;
	int m_WrapWidth = 0;
	bool m_IsDirty;
	Vector2 m_Size;
	int m_FontSize;
	std::string m_Data;
	Colour m_Colour;
	void CreateTextTexture();

public:
	TextComponent(Entity& entity);
	~TextComponent();

	void Update(float DeltaTime);
	void Render(IRenderer& renderer);

	void Serialize(Serializer& writer) const override;
	void Deserialize(SerializedValue& value) override;
	void RenderProperties() override;

	/*
	/// <summary>
	/// Sets the name of the font used for the string element.
	/// </summary>
	/// <param name="fontName"></param>
	void SetFontAssetName(const std::string& fontName);
	*/

	inline void SetFontSize(const int& size) { if (m_FontSize != size) m_FontSize = size; m_IsDirty = true; };

	/// <summary>
	/// Text is wrapped on multiple lines on line endings and word boundaries if it extends past WrapWidth.
	/// </summary>
	/// <param name="width">If wrapLength is 0, this function will only wrap on newline characters.</param>
	inline void SetWrapWidthInPixels(const int& width) { if (m_WrapWidth != width) m_WrapWidth = width; m_IsDirty = true; };

	/// <summary>
	/// Set offset from entity transform.
	/// </summary>
	/// <param name="screenSpacePosition"></param>
	inline void SetOffset(const Vector2& offset) { if (m_Offset != offset) m_Offset = offset; m_IsDirty = true;	};

	/// <summary>
	/// Set string value of text element.
	/// </summary>
	/// <param name="str"></param>
	inline void SetString(const char* str) { if (m_Data != str) m_Data = str; m_IsDirty = true; };

	/// <summary>
	/// Set string value of text element.
	/// </summary>
	/// <param name="str"></param>
	inline void SetString(const std::string& str) { if (m_Data != str) m_Data = str; m_IsDirty = true; };

	/// <summary>
	/// Set string colour of text element.
	/// </summary>
	/// <param name="colour"></param>
	void SetColour(const Colour& colour) { if(m_Colour != colour) m_Colour = colour; m_IsDirty = true;	};

	inline const Vector2& GetOffset()	const { return m_Offset; };
	inline const Colour& GetColour()	const { return m_Colour; };
	inline const std::string& GetText() const { return m_Data; };
	inline const int& GetFontSize()		const { return m_FontSize; };
	inline const int& GetTextWidth()	const { return (int)m_Size.X; };
	inline const int& GetTextHeight()	const { return (int)m_Size.Y; };
};

