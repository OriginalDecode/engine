#pragma once
#include "BaseSystem.h"
class InputSystem : public BaseSystem
{
public:
	InputSystem(CEntityManager& anEntityManager);
	void Update(float delta_time) override;
};

