#include "pch.h"
#include "Component_Animation.h"
#include "TextureCache.h"
#include "Texture.h"
#include "Entity.h"
#include "Game.h"
#include "IRenderer.h"

AnimationComponent::AnimationComponent(Entity& entity) : Component("Animation Component", entity)
{
	m_AnimationSheet = nullptr;
	m_TimeElapsed = 0.0f;
	m_IsLooping = false;
	m_Duration = 0.0f;
	m_TotalFrames = 0;
	m_TimeBetweenFrames = 0.0f;
	m_AnimationCount = 0;
	m_FrameSize.X = 0.0f;
	m_FrameSize.Y = 0.0f;
	m_HasFinished = false;
}

AnimationComponent::~AnimationComponent()
{

}

void AnimationComponent::Update(float DeltaTime)
{
	if (m_IsLooping && m_HasFinished)
	{
		m_HasFinished = false;
	}

	if (m_AnimationSheet)
	{
		if (m_HasFinished == false)
		{
			m_TimeElapsed += DeltaTime;

			if (m_TimeElapsed > m_Duration)
			{
				if (m_IsLooping)
				{
					m_TimeElapsed = 0.0f;
					m_CurrentFrame = 0;
				}
				else
				{
					m_TimeElapsed = 0.0f;
					m_CurrentFrame = 0;
					m_HasFinished = true;
				}
			}
			else
			{
				m_CurrentFrame = (unsigned int)(trunc(m_TimeElapsed / m_TimeBetweenFrames));
			}
		}
	}
}

void AnimationComponent::Render(IRenderer& renderer)
{
	if (m_AnimationSheet)
	{
		Transform& transform = m_ParentEntity.GetTransform();
		WorldRectangle srcRect = WorldRectangle(
			(m_FrameSize.X * m_CurrentFrame),
			(m_FrameSize.Y * m_CurrentAnimation),
			m_FrameSize.X,
			m_FrameSize.Y
		);

		WorldRectangle dstRect = WorldRectangle(
			transform.Position.X,
			transform.Position.Y,
			m_FrameSize.X,
			m_FrameSize.Y
		);

		renderer.Render_Texture(*m_AnimationSheet, transform, RENDER_LAYER::LAYER_TO_BE_REMOVED_WHEN_I_HOOK_UP_LAYERS_TO_COMPONENT, nullptr, &srcRect, &dstRect, m_IsFlipped);
	}
}

void AnimationComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);

	writer.String("Duration"); writer.Double((double)m_Duration);
	writer.String("Animation Frame Count"); writer.Int(m_TotalFrames);
	writer.String("Animation Count"); writer.Int(m_AnimationCount);
	writer.String("Playing Animation"); writer.Uint(m_CurrentAnimation);

	writer.String("Texture");
	if (m_AnimationSheet != nullptr)
	{
		writer.String(m_AnimationSheet->GetName().c_str());
	}
	else
	{
		writer.Null();
	}

	writer.String("IsFlipped"); writer.Bool(m_IsFlipped);
	writer.String("IsLooping"); writer.Bool(m_IsLooping);
}

void AnimationComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);

	if (value.HasMember("Duration"))
		m_Duration = (float)value.FindMember("Duration")->value.GetDouble();

	if (value.HasMember("Animation Frame Count"))
		m_TotalFrames = value.FindMember("Animation Frame Count")->value.GetInt();

	if (value.HasMember("Animation Count"))
		m_AnimationCount = value.FindMember("Animation Count")->value.GetInt();

	if (value.HasMember("IsFlipped"))
		m_IsFlipped = value.FindMember("IsFlipped")->value.GetBool();

	if (value.HasMember("IsLooping"))
		m_IsLooping = value.FindMember("IsLooping")->value.GetBool();
	
	if(value.HasMember("Playing Animation"))
		m_CurrentAnimation = value.FindMember("Playing Animation")->value.GetUint();
	
	if (value.HasMember("Texture"))
	{
		auto texture = value.FindMember("Texture");
		if (texture->value.IsNull() == false)
		{
			std::string textureSheet = texture->value.GetString();
			LoadAnimationSheet(textureSheet);
		}
	}
}

void AnimationComponent::LoadAnimationSheet(std::string sheet_path)
{
	if (m_AnimationSheet)
	{
		m_AnimationSheet = nullptr;
	}

	m_AnimationSheet = TextureCache::GetTexture(sheet_path);
	
	RecalculateFrameAndAnimationData();
}

void AnimationComponent::RecalculateFrameAndAnimationData()
{
	if (m_AnimationSheet)
	{
		m_TimeBetweenFrames = m_Duration / (float)(m_TotalFrames);
		m_FrameSize = Vector2(m_AnimationSheet->Width / (float)m_TotalFrames, m_AnimationSheet->Height / (float)m_AnimationCount);
	}
}

void AnimationComponent::RenderProperties()
{
	ImGui::DragFloat("Animation duration", &m_Duration, 0.25f, 1, 60);
	ImGui::DragInt("Total Frames per Animation", &m_TotalFrames, 1.0f, 1, 200);
	ImGui::DragInt("Total Animations in sheet", &m_AnimationCount, 1.0f, 0, 200);
	ImGui::DragInt("Current animation: ", (int*)&m_CurrentAnimation, 1.0f, 0, m_AnimationCount);;
	ImGui::Text("Current frame: %f", m_TimeBetweenFrames);
	ImGui::Text("Frame time elapsed: %f", m_TimeElapsed);
	ImGui::Text("Current frame: %i", (int)m_CurrentFrame);
	ImGui::Text("Frame Size: X = %i, Y = %i", (int)m_FrameSize.X, (int)m_FrameSize.Y);
	ImGui::Text("Has Finished: %i", (int)&m_HasFinished);
	ImGui::Checkbox("Loop animation: ", &m_IsLooping);
	ImGui::Checkbox("Flip animation: ", &m_IsFlipped);

	ImGui::InputText("Sprite Name:##Animation", &m_UINewAnimationSheetString);
	ImGui::SameLine();
	if (ImGui::ArrowButton("Load Texture##Animation", ImGuiDir_::ImGuiDir_Left))
	{
		LoadAnimationSheet(m_UINewAnimationSheetString);
		m_UINewAnimationSheetString = "";
	}

	RecalculateFrameAndAnimationData();
}

void AnimationComponent::SetAnimationCount(unsigned int animationCount)
{
	m_AnimationCount = animationCount;
	RecalculateFrameAndAnimationData();
}

void AnimationComponent::SetAnimationFrameCount(unsigned int frameCountPerAnimation)
{
	m_TotalFrames = frameCountPerAnimation;
	RecalculateFrameAndAnimationData();
}

const bool AnimationComponent::IsLooping() const
{
	return m_IsLooping;
}

const bool AnimationComponent::IsFlipped() const
{
	return m_IsFlipped;
}

const unsigned int AnimationComponent::GetPlayingAnimation()
{
	return m_CurrentAnimation;
}

void AnimationComponent::SetPlayingAnimation(unsigned int animation)
{
	m_CurrentAnimation = animation;
}

void AnimationComponent::SetDuration(float value)
{
	m_Duration = value;
	RecalculateFrameAndAnimationData();
}

float AnimationComponent::GetTimeElapsed() const
{
	return m_TimeElapsed;
}

const bool AnimationComponent::HasFinished() const
{
	return m_HasFinished;
}