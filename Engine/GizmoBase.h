#pragma once
#include "GizmoShared.h"
#include <Math/Vector/Vector.h>
class GizmoBase
{
public:
	GizmoBase() = default;
	void CreateGizmoHandle(GizmoHandle& gizmo_handle, const std::string& model_key, const std::string& texture_path, GizmoHandle::eDirection direction);
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
	void OffsetGizmoHandle(GizmoHandle& gizmo_handle);
	float m_GizmoHandleOffset;
	GizmoHandle m_Right;  // red
	GizmoHandle m_Up; // green
	GizmoHandle m_Forward; //blue
	CU::Matrix44f m_Orientation;
	bool m_Active = false;
};

typedef GizmoBase PositionGizmo;
typedef GizmoBase RotationGizmo;
typedef GizmoBase ScaleGizmo;