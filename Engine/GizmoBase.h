#pragma once
#include <Engine/engine_shared.h>
#include "GizmoShared.h"
#include <Math/Vector/Vector.h>
#include <Subscriber.h>

static const std::string w_down = ("w_down");
static const u64 w_down_HASH = Hash("w_down");
static const std::string e_down = ("e_down");
static const u64 e_down_HASH = Hash("e_down");
static const std::string r_down = ("r_down");
static const u64 r_down_HASH = Hash("r_down");




class GizmoBase : public Subscriber
{
public:
	GizmoBase() = default;

	enum eGizmoType
	{
		NONE,
		SCALING,
		ROTATE,
		POSITION,
	};

	void Initiate(std::string event, eGizmoType type);


	void CreateGizmoHandle(GizmoHandle& gizmo_handle, std::string model_key, const std::string& texture_path, GizmoHandle::eDirection direction);
	void Render();
	void RenderBoxes();

	void Update();

	GizmoHandle& GetRight() { return m_Right; }
	GizmoHandle& GetUp() { return m_Up; }
	GizmoHandle& GetForward() { return m_Forward; }
	void SetPosition(const CU::Vector3f& position);
	bool IsActive() { return m_Active; }
	void ToggleActive() { m_Active = !m_Active; }

	bool Inside(const CU::Vector3f& position, GizmoHandle* result);

private:
	
	virtual void HandleEvent(u64 event, void* data);



	void OffsetGizmoHandle(GizmoHandle& gizmo_handle);
	float m_GizmoHandleOffset;
	GizmoHandle m_Right;  // red
	GizmoHandle m_Up; // green
	GizmoHandle m_Forward; //blue
	CU::Matrix44f m_Orientation;
	eGizmoType m_Type = NONE;
	bool m_Active = false;
};

typedef GizmoBase PositionGizmo;
typedef GizmoBase RotationGizmo;
typedef GizmoBase ScaleGizmo;