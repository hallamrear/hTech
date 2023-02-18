#include "pch.h"
#include "Entity.h"
#include "Component_Sprite.h"
#include "TextureCache.h"

void SpriteComponent::RenderProperties()
{
	ImGui::Text("Texture Details:\n");
	static std::string mUINewSpriteString;
	ImGui::InputText("Sprite Name:", &mUINewSpriteString);
	ImGui::SameLine();
	if (ImGui::ArrowButton("Load Texture", ImGuiDir_::ImGuiDir_Left))
	{
		LoadTexture(mUINewSpriteString);
		mUINewSpriteString = "";
	}
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	if (m_SDLTexture != nullptr)
	{
		ImGui::Text("Texture Location: %s", m_SDLTexture->GetLocation().c_str());
		ImGui::Text("Texture Name: %s", m_SDLTexture->GetName().c_str());
		ImGui::Text("Height: %i", m_SDLTexture->Height);
		ImGui::Text("Width: %i", m_SDLTexture->Width);
		ImGui::Checkbox("Flip texture", &m_IsFlipped);
	}
	else
	{
		ImGui::Text("No texture loaded at the moment.");
		ImGui::Text("Write texture location in this box and click the arrow.");
	}
}

SpriteComponent::SpriteComponent(Entity& entity) : Component("Sprite Component", entity)
{
	m_SDLTexture = nullptr;
	m_IsFlipped = false;
}

SpriteComponent::~SpriteComponent()
{
	m_SDLTexture = nullptr;
}

void SpriteComponent::SetIsFlipped(bool state)
{
	m_IsFlipped = state;
}

bool SpriteComponent::IsFlipped()
{
	return m_IsFlipped;
}

void SpriteComponent::LoadTexture(std::string texture_path)
{
	m_SDLTexture = TextureCache::GetTexture(texture_path);
}

void SpriteComponent::UnloadTexture()
{
	if (m_SDLTexture)
	{
		m_SDLTexture = nullptr;
	}
}

void SpriteComponent::Update(float DeltaTime)
{

}

void SpriteComponent::Render(SDL_Renderer& renderer)
{
	if (m_SDLTexture)
	{
		m_SDLTexture->Render(renderer, m_ParentEntity.GetTransform(), m_IsFlipped);
	}
}

void SpriteComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);

	writer.String("Texture");  
	if (m_SDLTexture != nullptr)
	{
		writer.String(m_SDLTexture->GetName().c_str());
	}
	else
	{
		writer.Null();
	}

	writer.String("IsFlipped"); writer.Bool(m_IsFlipped);
}

void SpriteComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);

	if (value["Texture"].IsString())
	{
		std::string texturePath = value["Texture"].GetString();
		LoadTexture(texturePath);
	}
	else
	{
		m_SDLTexture = nullptr;
	}

	if (value["IsFlipped"].IsBool())
	{
		m_IsFlipped = value["IsFlipped"].GetBool();
	}
}