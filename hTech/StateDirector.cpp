#pragma region deprecated
#include "pch.h"

/*

#include "StateDirector.h"

//todo : put singleton back
//StateDirector* StateDirector::mInstance = nullptr;

[[deprecated("Use World instead")]]
StateDirector::StateDirector()
{
	mCurrentState = nullptr;
}

[[deprecated("Use World instead")]]
StateDirector::~StateDirector()
{
	mCurrentState = nullptr;
}

[[deprecated("Use World instead")]]
void StateDirector::SetupState(GameStateIdentifier identifier, GameState* state)
{
	Get()->SetupState_Impl(identifier, state);
}

[[deprecated("Use World instead")]]
void StateDirector::SetupState_Impl(GameStateIdentifier identifier, GameState* state)
{
	mDirectorStates.emplace(identifier, state);
}

[[deprecated("Use World instead")]]
void StateDirector::SetState_Impl(const GameStateIdentifier state)
{
	auto itr = mDirectorStates.find(state);

	if (mCurrentState)
		mCurrentState->End();

	if (itr == mDirectorStates.end())
	{
		mCurrentState = nullptr;
		//todo : fix
		//assert(itr != mDirectorStates.end());
		return;
	}

	mCurrentState = itr->second;
	mCurrentState->Start();
}

[[deprecated("Use World instead")]]
void StateDirector::SetState(const GameStateIdentifier state)
{
	Get()->SetState_Impl(state);
}

[[deprecated("Use World instead")]]
StateDirector* StateDirector::Get()
{
	//todo : put singleton back
	//if (!mInstance)
	//	mInstance = new StateDirector();

	static StateDirector mInstance;
	return &mInstance;
}

[[deprecated("Use World instead")]]
const GameState* StateDirector::GetState_Impl()
{
	return mCurrentState;
}

[[deprecated("Use World instead")]]
const GameState* StateDirector::GetState()
{
	return Get()->GetState_Impl();
}

[[deprecated("Use World instead")]]
void StateDirector::Update(double DeltaTime)
{
	Get()->Update_Impl(DeltaTime);
}

[[deprecated("Use World instead")]]
void StateDirector::Render(SDL_Renderer& renderer)
{
	Get()->Render_Impl(renderer);
}

[[deprecated("Use World instead")]]
void StateDirector::Update_Impl(double DeltaTime)
{
	if(mCurrentState)
		mCurrentState->Update(DeltaTime);
}
[[deprecated("Use World instead")]]
void StateDirector::Render_Impl(SDL_Renderer& renderer)
{
	if (mCurrentState)
		mCurrentState->Render(renderer);
}
*/

#pragma endregion