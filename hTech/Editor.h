#pragma once

enum class EDITOR_STATE
{
	MOVE = 0,
	INSPECT,
	ROTATE,
	NONE
};

class Entity;
class Texture;

class Editor
{
private:
	Texture* mCursorTextures[3];

	/// <summary>
	/// Store a reference to avoid calling inputmanager 24/7
	/// </summary>
	const Vector2& mMouseWorldPosition;

	/// <summary>
	/// Store a reference to avoid calling inputmanager 24/7
	/// </summary>
	const Vector2& mMouseScreenPosition;

	static Editor* mInstance;
	EDITOR_STATE mCurrentCursorState;
	Entity* mSelectedEntity;

	void Update_Impl(float deltaTime);
	void Render_Impl(SDL_Renderer&);
	void OnMouseClick_Impl();

	void SetEditorCursorState_Impl(EDITOR_STATE state);
	void SetCursorStateToMoveMode();
	void SetCursorStateToRotateMode();
	void SetCursorStateToInspectMode();
	void SetCursorStateToNoMode();


protected:
	Editor();
	static Editor* Get();

public:
	~Editor();

	static void Update(float deltaTime);
	static void Render(SDL_Renderer&);
	static void OnMouseClick();
	static void SetEditorCursorState(EDITOR_STATE state);
};

