#pragma once
#include "BaseComponent.h"

namespace Snowblind
{
	class CCamera;
}

struct CameraComponent final : public BaseComponent
{
	Snowblind::CCamera* m_Camera = nullptr;
};