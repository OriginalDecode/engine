#pragma once
#include <string>
#include "BaseComponent.h"
#include <Math/Vector/Vector.h>
#include <Engine/ModelInstance.h>
struct RenderComponent : public BaseComponent
{
	ModelInstance m_ModelInstance;
	CU::Vector4f m_Scale;
	CU::Vector4f m_Rotation;
	CU::Vector3f m_MinPos;
	CU::Vector3f m_MaxPos;
	bool m_RenderWireframe = false;

};