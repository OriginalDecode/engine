#pragma once
#include "BaseComponent.h"

namespace Snowblind
{
	class Camera;
}

struct CameraComponent final : public BaseComponent
{
	Snowblind::Camera* m_Camera = nullptr;
};