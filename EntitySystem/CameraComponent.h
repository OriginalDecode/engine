#pragma once
#include "BaseComponent.h"


class Camera;
struct CameraComponent final : public BaseComponent
{
	Camera* m_Camera = nullptr;
};