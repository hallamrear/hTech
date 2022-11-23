#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include <map>
#include "GameStates.h"

enum class HTECH_FUNCTION_EXPORT [[deprecated("Use World instead")]] GameStateIdentifier
{	
	GAME_STATE_UNKNOWN = -1,
	GAME_STATE_PAUSED = 0,
	GAME_STATE_PLAYING,
	GAME_STATE_MAIN_MENU,
	GAME_STATE_1,
	GAME_STATE_2,
	GAME_STATE_3,
	GAME_STATE_4,
	GAME_STATE_5,
	GAME_STATE_6,
	GAME_STATE_7,
	GAME_STATE_8,
	GAME_STATE_9
};

class HTECH_FUNCTION_EXPORT StateDirector
{
private:
	//todo : put singleton back
	//static StateDirector* mInstance;

	GameState* mCurrentState;
	std::map<GameStateIdentifier, GameState*> mDirectorStates;

	
	[[deprecated("Use World instead")]]
	StateDirector();
	[[deprecated("Use World instead")]] 
	~StateDirector();								    
	[[deprecated("Use World instead")]]
	void					SetState_Impl(const GameStateIdentifier state);									    
	[[deprecated("Use World instead")]]
	void					SetupState_Impl(GameStateIdentifier identifier, GameState* state);								    
	[[deprecated("Use World instead")]]
	const GameState*		GetState_Impl();								    
	[[deprecated("Use World instead")]]
	static StateDirector*	Get();								    
	[[deprecated("Use World instead")]]
	void					Update_Impl(float DeltaTime);								    
	[[deprecated("Use World instead")]]
	void					Render_Impl(SDL_Renderer& renderer);

public:
	[[deprecated("Use World instead")]]
	static void				SetState(const GameStateIdentifier state);
	[[deprecated("Use World instead")]]
	static void				SetupState(GameStateIdentifier identifier, GameState* state);
	[[deprecated("Use World instead")]]
	static const GameState*	GetState();
	[[deprecated("Use World instead")]]
	static void				Update(float DeltaTime);
	[[deprecated("Use World instead")]]
	static void				Render(SDL_Renderer& renderer);
};
