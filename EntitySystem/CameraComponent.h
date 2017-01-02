#pragma once
#include "BaseComponent.h"

namespace Hex
{
	class Camera;
}

struct CameraComponent final : public BaseComponent
{
	Hex::Camera* m_Camera = nullptr;
};