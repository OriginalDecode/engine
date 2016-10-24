#pragma once
#include "BaseSystem.h"
#include "../Input/InputCommand.h"
class CRigidBody;
//class JumpCommand final : public InputCommand
//{
//	~JumpCommand();
//};

class InputSystem : public BaseSystem
{
public:
	InputSystem(CEntityManager& anEntityManager);
	void Update(float delta_time) override;

};

