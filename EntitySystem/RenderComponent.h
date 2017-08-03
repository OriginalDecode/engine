#pragma once
#include <string>
#include "BaseComponent.h"
#include <Math/Vector/Vector.h>
#include <Engine/engine_shared.h>
#include <CommonLib/DataStructures/GrowingArray.h>
#include <Math/Matrix/Matrix.h>
struct ModelInstance
{
	std::string m_ModelID;

	CU::Matrix44f m_Orientation;
	bool m_RenderWireframe = false;
	bool m_Shadowed = true;

	/*
	CU::Vector4f m_Scale;
		CU::Vector4f m_Rotation;
		CU::Vector4f m_Position;*/
};


struct RenderComponent : public BaseComponent
{
	std::string myModelID;

	CU::GrowingArray<ModelInstance> m_Instances;

	CU::Vector4f scale;
	CU::Vector4f m_Rotation;
	CU::Vector3f m_MinPos;
	CU::Vector3f m_MaxPos;
	bool m_RenderWireframe = false;
	bool m_Shadowed = true;
};