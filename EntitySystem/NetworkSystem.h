#pragma once
#include "BaseSystem.h"
class NetworkSystem :
	public BaseSystem
{
public:
	NetworkSystem(CEntityManager& anEntityManager);
	void Update(float delta_time) override;
};

