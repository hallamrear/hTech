#include "pch.h"
#include "Component_TextElement.h"
#include "Engine.h"
#include "Entity.h"
#include "FontLoader.h"
#include "Console.h"
#include <SDL_render.h>
#include "OriginalRenderer.h"
#include "Camera.h"

void TextComponent::CreateTextTexture()
{
	if (m_IsDirty)
	{
		if (m_TextTexture)
		{
			SDL_DestroyTexture(m_TextTexture);
		}

		TTF_Font* font = FontLoader::GetFont(m_FontSize);

		if (font == nullptr || m_Data == "")
			return;

		OriginalRenderer& ref = (OriginalRenderer&)Engine::GetRenderer();
		SDL_Renderer* renderer = ref.GetAPIRenderer();
		if (!renderer)
			return;

		SDL_Surface* textSurface = nullptr;
		SDL_Color color = { m_Colour.R, m_Colour.G, m_Colour.B, m_Colour.A };

		textSurface = TTF_RenderUTF8_Blended_Wrapped(font, m_Data.c_str(), color, m_WrapWidth);

		if (!textSurface)
		{
			Console::LogMessage(LogLevel::LOG_ERROR, "FAILED TO RENDER TEXT");
			Console::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
			return;
		}

		m_TextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		int w, h;
		SDL_QueryTexture(m_TextTexture, NULL, NULL, &w, &h);
		m_Size.X = w;
		m_Size.Y = h;

		SDL_FreeSurface(textSurface);
		textSurface = nullptr;
		m_IsDirty = false;
	}
}

TextComponent::TextComponent(Entity& entity) : Component("Text Component", entity)
{
	m_IsDirty = true;
	m_FontSize = 14;
	m_Data = "Placeholder Text";
	m_WrapWidth = 32;
	CreateTextTexture();
}

TextComponent::~TextComponent()
{
	SDL_DestroyTexture(m_TextTexture);
}

void TextComponent::Update(float DeltaTime)
{
	if (m_IsDirty)
	{
		CreateTextTexture();

		if (m_TextTexture)
		{
			m_IsDirty = false;
		}
	}
}

void TextComponent::Render(IRenderer& renderer)
{
	if (m_TextTexture)
	{
		Vector2 position = Vector2(m_ParentEntity.GetTransform().Position.X + m_Offset.X, m_ParentEntity.GetTransform().Position.Y + m_Offset.Y);
		position = Camera::WorldToScreen(position);
		SDL_Rect destRect =
		{
			position.X,
			position.Y,
			m_Size.X,
			m_Size.Y
		};

		OriginalRenderer& ref = (OriginalRenderer&)Engine::GetRenderer();
		SDL_Renderer* r = ref.GetAPIRenderer();
		SDL_RenderCopy(r, m_TextTexture, nullptr, &destRect);
	}
}

void TextComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);

	writer.String("Data");	    writer.String(m_Data.c_str());
	writer.String("FontSize");  writer.Int(m_FontSize);
	writer.String("ColourR");   writer.Int(m_Colour.R);
	writer.String("ColourG");   writer.Int(m_Colour.G);
	writer.String("ColourB");   writer.Int(m_Colour.B);
	writer.String("WrapWidth"); writer.Int(m_WrapWidth);
	writer.String("OffsetX");	writer.Double(m_Offset.X);
	writer.String("OffsetY");	writer.Double(m_Offset.Y);
}

void TextComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);

	if (value.HasMember("Data"))
		m_Data = value.FindMember("Data")->value.GetString();

	if (value.HasMember("FontSize"))
		m_FontSize = value.FindMember("FontSize")->value.GetInt();

	if (value.HasMember("ColourR"))
		m_Colour.R = value.FindMember("ColourR")->value.GetInt();
	if (value.HasMember("ColourG-"))
		m_Colour.G = value.FindMember("ColourG")->value.GetInt();
	if (value.HasMember("ColourB"))
		m_Colour.B = value.FindMember("ColourB")->value.GetInt();

	if (value.HasMember("WrapWidth"))
		m_WrapWidth = value.FindMember("WrapWidth")->value.GetInt();

	if (value.HasMember("OffsetX"))
		m_Offset.X = (float)value.FindMember("OffsetX")->value.GetDouble();

	if (value.HasMember("OffsetY"))
		m_Offset.Y = (float)value.FindMember("OffsetY")->value.GetDouble();
}

void TextComponent::RenderProperties()
{
	std::string data = m_Data;
	ImGui::InputText("Text: ", &data);
	SetString(data);

	int fontSize = m_FontSize;
	ImGui::InputInt("Font Size: ", &fontSize, 1, 2);
	SetFontSize(fontSize);

	float c[3] = { m_Colour.R, m_Colour.G, m_Colour.B };
	ImGui::DragFloat3("Text colour: ", c);
	SetColour(Colour(c[0], c[1], c[2], 255));

	int ww = m_WrapWidth;
	ImGui::InputInt("Font wrap width: ", &ww);
	SetWrapWidthInPixels(ww);

	float offset[2] = { m_Offset.X, m_Offset.Y };
	ImGui::InputFloat2("Text Offset: ", offset);
	SetOffset(Vector2(offset[0], offset[1]));
}
