#pragma once
#include "BaseComponent.h"
#include <Math/Matrix/Matrix44.h>
#include <Math/Plane/PlaneVolume.h>

struct DebugComponent final : public BaseComponent
{
	~DebugComponent() { }
	CU::PlaneVolume<float> m_OBB;
	CU::StaticArray<CU::Vector3f, 8> m_BoxPointList;
	CU::Vector3f m_WHD;
};