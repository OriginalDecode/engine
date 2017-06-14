#pragma once
#include "BaseSystem.h"
class CameraSystem final : public BaseSystem
{
public:
	CameraSystem(NodeEntityManager& entitymanager);
	void Update(float delta_time) override;


};

