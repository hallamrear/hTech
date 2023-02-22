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
	void Render_Impl(IRenderer&);

	void SetEditorCursorState_Impl(EDITOR_STATE state);
	void SetCursorStateToMoveMode();
	void SetCursorStateToRotateMode();
	void SetCursorStateToInspectMode();
	void SetCursorStateToNoMode();
	void SetCursorStateToSelectMode();

	void SetSelectedEntity_Impl(Entity* entity);

	void MousePress();
	void MouseRelease();
	void MouseHold();
	void ClearSelected_Impl();
	Entity* GetSelectedEntity_Impl();
	std::vector<Entity*> GetSelectedEntities_Impl();

protected:
	Editor();
	static Editor* Get();

public:
	~Editor();

	static void Update(float deltaTime);
	static void Render(IRenderer& renderer);
	static void SetEditorCursorState(EDITOR_STATE state);
	static void SetSelectedEntity(Entity* entity);
	static Entity* GetSelectedEntity();
	static std::vector<Entity*> GetSelectedEntities();
	static EDITOR_STATE GetEditorCursorState();
	static void ClearSelected();
};