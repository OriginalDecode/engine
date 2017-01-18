#pragma once
#include "BaseComponent.h"
#include <Math/Matrix/Matrix44.h>
#include <Math/Plane/PlaneVolume.h>

struct DebugComponent final : public BaseComponent
{
	~DebugComponent() { }
	CU::PlaneVolume<float> m_OBB;
	CU::Vector4f m_WHD;
	CU::Vector4f m_Rotation;
	CU::Vector4f debugColor = { 255.f, 255.f, 255.f, 255.f };
};