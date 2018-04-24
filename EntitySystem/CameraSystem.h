#pragma once
#include "BaseSystem.h"
class CameraSystem final : public BaseSystem
{
public:
	CameraSystem(_EntityManager& entitymanager);
	void Update(float delta_time, bool paused) override;


};

