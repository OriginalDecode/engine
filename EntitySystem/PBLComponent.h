#pragma once
#include "BaseComponent.h"


struct PBLComponent : public BaseComponent
{
	struct
	{
		float m_Roughness = 0.f;
		float m_Metalness = 0.f;
		float dummy0;
		float dummy1;
	} m_PBLValues;
	void* m_Buffer = nullptr;
};