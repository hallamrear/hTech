#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Component.h"

struct SDL_Renderer;
class HTECH_FUNCTION_EXPORT Texture;

class HTECH_FUNCTION_EXPORT AnimationComponent
	: public Component
{
private:
	std::string		m_UINewAnimationSheetString;
	float			m_TimeBetweenFrames;
	float			m_TimeElapsed;
	float			m_Duration;
	int m_CurrentFrame;
	Texture* m_AnimationSheet;
	int m_CurrentAnimation;
	int m_TotalFrames;
	Vector2 m_FrameSize;
	int m_AnimationCount;
	bool			m_IsLooping;
	bool			m_IsFlipped;

	//If the animation does not loop, this will set to true when it has finished playing.
	bool m_HasFinished;
	void RecalculateFrameAndAnimationData();

protected:
	void RenderProperties();

public:

	void SetDuration(float value);
	float GetTimeElapsed() const;

	//If the animation does not loop, this will set to true when it has finished playing.
	const bool HasFinished() const;
	void LoadAnimationSheet(std::string sheet_path);
	void SetAnimationCount(unsigned int animationCount);
	void SetAnimationFrameCount(unsigned int frameCountPerAnimation);
	const bool IsLooping() const;
	const bool IsFlipped() const;

	const unsigned int GetPlayingAnimation();
	void SetPlayingAnimation(unsigned int animation);

	AnimationComponent(Entity& entity);
	~AnimationComponent();

	void Update(float DeltaTime);
	void Render(IRenderer& renderer);

	void Serialize(Serializer& writer) const override;
	void Deserialize(SerializedValue& value) override;
};

