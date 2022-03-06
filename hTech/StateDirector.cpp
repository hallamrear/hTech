#include "pch.h"
#include "StateDirector.h"

//todo : put singleton back
//StateDirector* StateDirector::mInstance = nullptr;

StateDirector::StateDirector()
{
	mCurrentState = nullptr;
}

StateDirector::~StateDirector()
{
	mCurrentState = nullptr;
}

void StateDirector::SetupState(GameStateIdentifier identifier, GameState* state)
{
	Get()->SetupState_Impl(identifier, state);
}

void StateDirector::SetupState_Impl(GameStateIdentifier identifier, GameState* state)
{
	//Todo : error check for uniques
	//todo : check if identifier exists and if so, throw;
	mDirectorStates.emplace(identifier, state);
}

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

void StateDirector::SetState(const GameStateIdentifier state)
{
	Get()->SetState_Impl(state);
}

StateDirector* StateDirector::Get()
{
	//todo : put singleton back
	//if (!mInstance)
	//	mInstance = new StateDirector();

	static StateDirector mInstance;
	return &mInstance;
}

const GameState* StateDirector::GetState_Impl()
{
	return mCurrentState;
}

const GameState* StateDirector::GetState()
{
	return Get()->GetState_Impl();
}

void StateDirector::Update(double DeltaTime)
{
	Get()->Update_Impl(DeltaTime);
}

void StateDirector::Render(SDL_Renderer& renderer)
{
	Get()->Render_Impl(renderer);
}

void StateDirector::Update_Impl(double DeltaTime)
{
	if(mCurrentState)
		mCurrentState->Update(DeltaTime);
}
void StateDirector::Render_Impl(SDL_Renderer& renderer)
{
	if (mCurrentState)
		mCurrentState->Render(renderer);
}
