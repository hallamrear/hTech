#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Texture.h"
#include "Transform.h"

class HTECH_FUNCTION_EXPORT AnimationController
{
	float m_TimeBetweenFrames;
	float m_TimeElapsed;
	float m_Duration;
	unsigned int m_CurrentFrame;
	Texture* m_AnimationSheet;
	bool m_IsLooping;
	unsigned int m_CurrentAnimation;
	unsigned int m_TotalFrames;

	//If the animation does not loop, this will set to true when it has finished playing.
	bool m_HasFinished;
	Vector2 m_FrameSize;

public:

	AnimationController(std::string sheetPath, unsigned int numberOfAnimations, unsigned int frameCount, float duration, bool looping);
	~AnimationController();

	bool HasFinished();
	void Start();
	void SetAnimation(unsigned int animation);
	const Vector2 GetFrameSize() const;
	void Update(float DeltaTime);
	void Render(IRenderer& renderer, Transform transform, bool flipped = false);
};