#pragma once
#include "BaseSystem.h"
class CameraSystem final : public BaseSystem
{
public:
	CameraSystem(EntityManager& entitymanager);
	void Update(float delta_time) override;


};

