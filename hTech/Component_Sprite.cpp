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
	if (mTexture != nullptr)
	{
		ImGui::Text("Texture Location: %s", mTexture->GetLocation().c_str());
		ImGui::Text("Height: %i", mTexture->Height);
		ImGui::Text("Width: %i", mTexture->Width);
		ImGui::Checkbox("Flip texture", &mIsFlipped);
	}
	else
	{
		ImGui::Text("No texture loaded at the moment.");
		ImGui::Text("Write texture location in this box and click the arrow.");
	}
}

SpriteComponent::SpriteComponent(Entity& entity) : Component("Sprite Component", entity)
{
	mTexture = nullptr;
	mIsFlipped = false;
}

SpriteComponent::~SpriteComponent()
{
	mTexture = nullptr;
}

void SpriteComponent::SetIsFlipped(bool state)
{
	mIsFlipped = state;
}

bool SpriteComponent::IsFlipped()
{
	return mIsFlipped;
}

void SpriteComponent::LoadTexture(std::string texture_path)
{
	mTexture = TextureCache::GetTexture(texture_path);
}

void SpriteComponent::UnloadTexture()
{
	if (mTexture)
	{
		mTexture = nullptr;
	}
}

void SpriteComponent::Update(float DeltaTime)
{

}

void SpriteComponent::Render(SDL_Renderer& renderer)
{
	if (mTexture)
	{
		mTexture->Render(renderer, Parent.GetTransform().Position, Parent.GetTransform().Rotation, mIsFlipped);
	}
}

void SpriteComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);

	writer.String("Texture");  
	if (mTexture != nullptr)
	{
		writer.String(mTexture->GetLocation().c_str());
	}
	else
	{
		writer.Null();
	}

	writer.String("IsFlipped"); writer.Bool(mIsFlipped);
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
		mTexture = nullptr;
	}

	if (value["IsFlipped"].IsBool())
	{
		mIsFlipped = value["IsFlipped"].GetBool();
	}
}