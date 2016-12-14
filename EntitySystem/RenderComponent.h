#pragma once
#include <string>
#include "BaseComponent.h"
#include <Math/Vector/Vector.h>

struct SRenderComponent : public BaseComponent
{
	std::string myModelID;
	CU::Vector4f scale;
	CU::Vector4f m_Rotation;

} typedef RenderComponent;