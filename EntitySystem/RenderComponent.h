#pragma once
#include <string>
#include "BaseComponent.h"
#include <Math/Vector/Vector.h>

struct RenderComponent : public BaseComponent
{
	std::string myModelID;
	CU::Vector4f scale;
	CU::Vector4f m_Rotation;
	CU::Vector3f m_MinPos;
	CU::Vector3f m_MaxPos;

};