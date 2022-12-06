#include "pch.h"
#include "Component_Animation.h"
#include "TextureCache.h"
#include "Texture.h"
#include "Entity.h"
#include "Game.h"

AnimationComponent::AnimationComponent(Entity& entity) : Component("Animation Controller", entity)
{
	mAnimationSheet = nullptr;
	mTimeElapsed = 0.0f;
	IsLooping = false;
	mDuration = 0.0f;
	mTotalFrames = 0;
	mTimeBetweenFrames = 0.0f;
	mAnimationCount = 0;
	mFrameSize.X = 0.0f;
	mFrameSize.Y = 0.0f;
	mHasFinished = false;
}

AnimationComponent::~AnimationComponent()
{

}

void AnimationComponent::Update(float DeltaTime)
{
	if (mAnimationSheet)
	{
		if (mHasFinished == false)
		{
			mTimeElapsed += DeltaTime;

			if (mTimeElapsed > mDuration)
			{
				if (IsLooping)
				{
					mTimeElapsed = 0.0f;
					mCurrentFrame = 0;
				}
				else
				{
					mTimeElapsed = 0.0f;
					mCurrentFrame = 0;
					mHasFinished = true;
				}
			}
			else
			{
				mCurrentFrame = (unsigned int)(trunc(mTimeElapsed / mTimeBetweenFrames));
			}
		}
	}
}

void AnimationComponent::Render(SDL_Renderer& renderer)
{
	if (mAnimationSheet)
	{
		Transform& transform = Parent.GetTransform();
		Vector2 srcPos = Vector2((mFrameSize.X * mCurrentFrame) + (mFrameSize.X / 2.0f), (mFrameSize.Y * mCurrentAnimation) + mFrameSize.Y / 2.0f);
		mAnimationSheet->Render(*Game::Renderer, transform.Position, transform.Rotation, srcPos, mFrameSize, IsFlipped);
	}
}

void AnimationComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);

	writer.String("Duration"			 ); writer.Double((double)mDuration);
	writer.String("Animation Frame Count"); writer.Int(mTotalFrames);
	writer.String("Animation Count"		 ); writer.Int(mAnimationCount);

	writer.String("Texture");
	if (mAnimationSheet != nullptr)
	{
		writer.String(mAnimationSheet->GetLocation().c_str());
	}
	else
	{
		writer.Null();
	}

	writer.String("IsFlipped"); writer.Bool(IsFlipped);
	writer.String("IsLooping"); writer.Bool(IsLooping);
}

void AnimationComponent::LoadAnimationSheet(std::string sheet_path)
{
	if (mAnimationSheet)
	{
		mAnimationSheet = nullptr;
	}

	mAnimationSheet = TextureCache::GetTexture(sheet_path);
	
	RecalculateFrameAndAnimationData();
}

void AnimationComponent::RecalculateFrameAndAnimationData()
{
	if (mAnimationSheet)
	{
		mTimeBetweenFrames = mDuration / (float)(mTotalFrames);
		mFrameSize = Vector2(mAnimationSheet->Width / (float)mTotalFrames, mAnimationSheet->Height / (float)mAnimationCount);
	}
}

void AnimationComponent::RenderProperties()
{
	ImGui::DragFloat("Animation duration", &mDuration, 0.25f, 1, 60);
	ImGui::DragInt("Total Frames per Animation", &mTotalFrames, 1.0f, 1, 200);
	ImGui::DragInt("Total Animations in sheet", &mAnimationCount, 1.0f, 0, 200);
	ImGui::Text("Current frame: %f", mTimeBetweenFrames);
	ImGui::Text("Frame time elapsed: %f", mTimeElapsed);
	ImGui::Text("Current frame: %i", (int)mCurrentFrame);
	ImGui::Text("Current animation: %i", (int)mCurrentAnimation);
	ImGui::Text("Frame Size: X = %i, Y = %i", mFrameSize.X, mFrameSize.Y);
	ImGui::Text("Has Finished: %i", (int)&mHasFinished);
	ImGui::Checkbox("Loop animation: ", &IsLooping);
	ImGui::Checkbox("Flip animation: ", &IsFlipped);

	ImGui::InputText("Sprite Name:##Animation", &mUINewAnimationSheetString);
	ImGui::SameLine();
	if (ImGui::ArrowButton("Load Texture##Animation", ImGuiDir_::ImGuiDir_Left))
	{
		LoadAnimationSheet(mUINewAnimationSheetString);
		mUINewAnimationSheetString = "";
	}

	RecalculateFrameAndAnimationData();
}

void AnimationComponent::SetAnimationCount(unsigned int animationCount)
{
	mAnimationCount = animationCount;
	RecalculateFrameAndAnimationData();
}

void AnimationComponent::SetAnimationFrameCount(unsigned int frameCountPerAnimation)
{
	mTotalFrames = frameCountPerAnimation;
	RecalculateFrameAndAnimationData();
}

void AnimationComponent::SetDuration(float value)
{
	mDuration = value;
	RecalculateFrameAndAnimationData();
}

float AnimationComponent::GetTimeElapsed() const
{
	return mTimeElapsed;
}

bool AnimationComponent::HasFinished() const
{
	return mHasFinished;
}

void AnimationComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);

}