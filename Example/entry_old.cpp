//#include "hTech/Game.h"
//#include "hTech\StateDirector.h"
//#include "hTech\GameStates.h"
//#include "hTech/Log.h"
//#include "hTech\TextElement.h"
//#include "hTech\Animation.h"
//
//class TestState : public GameState
//{
//	TextElement* text;
//	AnimationController* idle_animation;
//	AnimationController* run_animation;
//
//	void Start()
//	{
//		text = new TextElement(Transform(Vector2f(0.0f, 64.0f)), "Hello, World!");
//		run_animation = new AnimationController("Assets/Knight_Run.png", 1, 10, 1.0f, true);
//		idle_animation = new AnimationController("Assets/Knight_Idle.png", 1, 10, 1.0f, true);
//	}
//
//	void Update(double dt)
//	{
//		if (run_animation) run_animation->Update(dt);
//		if (idle_animation) idle_animation->Update(dt);
//		if (text) text->Update(dt);
//	}
//
//	void Render(SDL_Renderer& renderer)
//	{
//		if (idle_animation) idle_animation->Render(renderer, Transform(Vector2f(-40.0f, 0.0f)));
//		if (run_animation) run_animation->Render(renderer, Transform(Vector2f(40.0f, 0.0f)));
//		if(text) text->Render();
//	}
//
//	void End()
//	{
//		delete text; text = nullptr;
//		delete idle_animation; idle_animation = nullptr;
//		delete run_animation; run_animation = nullptr;
//	}
//};
//
//
//int main(int argc, char* argv[])
//{
//	WindowDetails details;
//	details.dimensions = Vector2f(1280.0f, 720.0f);
//	details.title = "Hello, World!";
//	details.position = Vector2f(200.0f, 200.0f);
//
//	Log::LogMessage(LogLevel::LOG_ERROR, "Hello, World!");
//
//	Game* game = new Game();
//	game->Initialise(argc, argv, details);
//
//	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_1, new TestState());
//	StateDirector::SetState(GameStateIdentifier::GAME_STATE_1);
//
//	if (game->GetIsInitialised())
//	{
//		game->Start();
//	}
//
//	return 0;
//}