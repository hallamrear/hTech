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
	bool  m_AutosaveEnabled;
	float m_AutosaveTimer;
	float m_AutosaveCooldown;

	std::vector<Entity*> m_SelectedEntities;
	Entity* m_Selected = nullptr;
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
	
	inline void ClearSelected_Impl()						  { m_Selected = nullptr; m_SelectedEntities.clear(); };
	inline void SetEditorCursorState_Impl(EDITOR_STATE state) { m_CurrentCursorState = state; };
	inline void SetCursorStateToMoveMode()					  { m_CurrentCursorState = EDITOR_STATE::MOVE; };
	inline void SetCursorStateToRotateMode()				  { m_CurrentCursorState = EDITOR_STATE::ROTATE; };
	inline void SetCursorStateToInspectMode()				  { m_CurrentCursorState = EDITOR_STATE::INSPECT; };
	inline void SetCursorStateToNoMode()					  { m_CurrentCursorState = EDITOR_STATE::NONE; };
	inline void SetCursorStateToSelectMode()				  { m_CurrentCursorState = EDITOR_STATE::SELECT; m_Selected = nullptr; };
	inline void SetSelectedEntity_Impl(Entity* entity)		  { ClearSelected_Impl(); m_Selected = entity; };
	inline Entity* GetSelectedEntity_Impl()					  { return m_Selected; };
	inline std::vector<Entity*> GetSelectedEntities_Impl()	  { return m_SelectedEntities; };

	void MousePress();
	void MouseRelease();
	void MouseHold();
	bool OpenProject(std::string& path);

protected:
	Editor();
	static Editor* Get();

public:
	~Editor();

	static void Update(float deltaTime);
	static void Render(IRenderer& renderer);
	inline static void SetEditorCursorState(EDITOR_STATE state) { return Get()->SetEditorCursorState_Impl(state); };
	inline static std::vector<Entity*> GetSelectedEntities()	{ return Get()->GetSelectedEntities_Impl(); }
	inline static void SetSelectedEntity(Entity* entity)		{ return Get()->SetSelectedEntity_Impl(entity); };
	inline static EDITOR_STATE GetEditorCursorState()			{ return Get()->m_CurrentCursorState; };
	inline static Entity* GetSelectedEntity()					{ return Get()->GetSelectedEntity_Impl(); };
	inline static void ClearSelected()							{ return Get()->ClearSelected_Impl(); };
};