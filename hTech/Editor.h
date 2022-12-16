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
	std::vector<Entity*> m_SelectedEntities;
	Vector2 m_DragStartWS, m_DragCurrentWS, m_DragEndWS;
	Vector2 m_DragDeltaThisFrame;
	Vector2 m_MousePosPreviousFrame;
	bool m_IsDraggingRect = false;
	WorldRectangle m_SelectionRect = WorldRectangle(0, 0, 0, 0);

	/// <summary>
	/// Store a reference to avoid calling inputmanager 24/7
	/// </summary>
	const Vector2& m_MouseWorldPositionRef;

	/// <summary>
	/// Store a reference to avoid calling inputmanager 24/7
	/// </summary>
	const Vector2& m_MouseScreenPositionRef;

	static Editor* m_Instance;
	EDITOR_STATE m_CurrentCursorState;

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
	void ClearSelected_Impl();	
	void SetSelectedEntity_Impl(Entity* entity);

protected:
	Editor();
	static Editor* Get();

public:
	~Editor();

	static void Update(float deltaTime);
	static void Render(SDL_Renderer&);
	static void SetEditorCursorState(EDITOR_STATE state);
	static EDITOR_STATE GetEditorCursorState();
	static void ClearSelected();
	static void SetSelectedEntity(Entity* entity);
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