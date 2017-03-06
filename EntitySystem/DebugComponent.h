#pragma once
#include <Math/Matrix/Matrix44.h>
#include "BaseComponent.h"
#include <Math/Plane/PlaneVolume.h>
#include "../Engine/EditObject.h"
//#include "PositionGizmo.h"
//#include "ScaleGizmo.h"
//#include "RotationGizmo.h"
#include "GizmoBase.h"
struct DebugComponent final : public BaseComponent
{
	~DebugComponent() { }
	CU::PlaneVolume<float> m_OBB;
	CU::Vector4f m_WHD;
	CU::Vector4f m_Rotation;
	CU::Vector4f debugColor = { 255.f, 255.f, 255.f, 255.f };
	EditObject m_EditObject;

	PositionGizmo m_PositionGizmo;
	RotationGizmo m_RotationGizmo;
	ScaleGizmo m_ScaleGizmo;

	bool m_DirtyFlag = false;

};