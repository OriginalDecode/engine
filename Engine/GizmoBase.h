#pragma once
#include "GizmoShared.h"

class GizmoBase
{
public:
	GizmoBase() = default;
	virtual void CreateGizmoHandle(GizmoHandle& gizmo_handle, const std::string& model_key, const std::string& texture_path, GizmoHandle::eDirection direction);
	virtual void Render();
	virtual void RenderBoxes();

	virtual GizmoHandle& GetRight() { return m_Right; }
	virtual GizmoHandle& GetUp() { return m_Up; }
	virtual GizmoHandle& GetForward() { return m_Forward; }
protected:
	GizmoHandle m_Right;  // red
	GizmoHandle m_Up; // green
	GizmoHandle m_Forward; //blue
	CU::Matrix44f m_Orientation;

};

