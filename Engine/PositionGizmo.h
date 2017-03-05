#pragma once
#include "GizmoBase.h"

class PositionGizmo : public GizmoBase
{
public:
	PositionGizmo() = default;
	void Initiate();
	void Update();
	void SetPosition(const CU::Vector3f& pos);
private:
	void OffsetGizmoHandle(GizmoHandle& gizmo_handle);
};

