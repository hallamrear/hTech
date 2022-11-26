#pragma once
#include "Rectangle.h"

enum class EDITOR_STATE
{
	SELECT = 0,
	MOVE,
	INSPECT,
	ROTATE,
	NONE
};

class Entity;
class Texture;

class Editor
{
private:
	Texture* mCursorTextures[4];
	std::vector<Entity*> mSelectedEntities;
	Vector2 mDragStartWS, mDragCurrentWS, mDragEndWS;
	Vector2 mDragDifferenceThisFrame, mMovementMousePosLastFrame;
	bool mIsDraggingRect = false;
	WorldRectangle selectionRect = WorldRectangle(0, 0, 0, 0);

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

	void Update_Impl(float deltaTime);
	void Render_Impl(SDL_Renderer&);

	void SetEditorCursorState_Impl(EDITOR_STATE state);
	void SetCursorStateToMoveMode();
	void SetCursorStateToRotateMode();
	void SetCursorStateToInspectMode();
	void SetCursorStateToNoMode();
	void SetCursorStateToSelectMode();

	void MousePress();
	void MouseRelease();
	void MouseHold();


protected:
	Editor();
	static Editor* Get();

public:
	~Editor();

	static void Update(float deltaTime);
	static void Render(SDL_Renderer&);
	static void SetEditorCursorState(EDITOR_STATE state);
};
