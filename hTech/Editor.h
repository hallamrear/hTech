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

#if _DEBUG


class Entity;
class Texture;

class Editor
{
private:
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

#else

class Editor
{
private:

protected:
	Editor() {};
	static Editor* Get() { return nullptr; };

public:
	~Editor();

	static inline void Update(float deltaTime) {};
	static inline void Render(SDL_Renderer&) {};
	static inline void SetEditorCursorState(EDITOR_STATE state);
};
#endif