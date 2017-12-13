#pragma once
#include <Math/Matrix/Matrix44.h>
#include "BaseComponent.h"
#include <Math/Plane/PlaneVolume.h>
#include "../Engine/EditObject.h"
#include "GizmoBase.h"
struct DebugComponent final : public BaseComponent
{

	~DebugComponent() { }
	CU::PlaneVolume<float> m_OBB;
	CU::Vector4f m_WHD;
	void* m_Dweller = nullptr;
	CU::Vector4f m_MinPoint;
	CU::Vector4f m_MaxPoint;
	CU::Vector4f debugColor = { 255.f, 255.f, 255.f, 255.f };
	int m_ComponentFlags = 0;
	bool m_DirtyFlag = false;

};