#include "pch.h"
#include "Animation.h"
#include "Game.h"
#include "TextureCache.h"

AnimationController::AnimationController(std::string sheetPath, unsigned int animationCount, unsigned int frameCountPerAnimation, float duration, bool looping)
{
	m_AnimationSheet = TextureCache::GetTexture(sheetPath);
	m_TimeElapsed = 0.0f;
	m_IsLooping = looping;
	m_Duration = duration;
	m_TotalFrames = frameCountPerAnimation;
	m_TimeBetweenFrames = m_Duration / (float)(m_TotalFrames);
	m_FrameSize = Vector2(m_AnimationSheet->Width / (float)m_TotalFrames, m_AnimationSheet->Height / (float)animationCount);
	m_HasFinished = false;
}

AnimationController::~AnimationController()
{
	m_AnimationSheet = nullptr;
}

bool AnimationController::HasFinished()
{
	return m_HasFinished;
}

void AnimationController::Start()
{
	m_HasFinished = false;
	m_CurrentFrame = 0;
	m_TimeElapsed = 0.0f;
}

void AnimationController::SetAnimation(unsigned int animation)
{
	m_CurrentAnimation = animation;
}

const Vector2 AnimationController::GetFrameSize() const
{
	return m_FrameSize;
}

void AnimationController::Update(float DeltaTime)
{
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


void AnimationController::Render(SDL_Renderer& renderer, Transform transform)
{
	if (m_AnimationSheet)
	{
		Vector2 srcPos = Vector2((m_FrameSize.X * m_CurrentFrame) + (m_FrameSize.X / 2.0f), (m_FrameSize.Y * m_CurrentAnimation) + (m_FrameSize.Y / 2.0f));
		m_AnimationSheet->Render(*Game::Renderer, transform.Position, transform.Rotation, srcPos, m_FrameSize);
	}
}

void AnimationController::Render(SDL_Renderer& renderer, Transform transform, bool flipped)
{
	if (m_AnimationSheet)
	{
		Vector2 srcPos = Vector2((m_FrameSize.X * m_CurrentFrame) + (m_FrameSize.X / 2.0f), (m_FrameSize.Y * m_CurrentAnimation) + m_FrameSize.Y / 2.0f);
		m_AnimationSheet->Render(*Game::Renderer, transform.Position, transform.Rotation, srcPos, m_FrameSize, flipped);
	}
}
