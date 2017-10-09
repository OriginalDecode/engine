#pragma once
#include "BaseComponent.h"
#include <Math/Vector/Vector.h>
#include <Engine/engine_shared.h>
#include <CommonLib/DataStructures/GrowingArray.h>
#include <Math/Matrix/Matrix.h>

struct ModelInstance
{
	u64 m_ModelID;
	u64 m_MaterialKey;
	CU::Matrix44f m_Orientation;
	bool m_RenderWireframe = false;
	bool m_Shadowed = true;
};

struct RenderComponent : public BaseComponent
{
	u64 m_ModelID;
	CU::GrowingArray<ModelInstance> m_Instances;

	CU::Vector4f scale;
	CU::Vector4f m_Rotation;
	CU::Vector3f m_MinPos;
	CU::Vector3f m_MaxPos;
	bool m_RenderWireframe = false;
	bool m_Shadowed = true;
};