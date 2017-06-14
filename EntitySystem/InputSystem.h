#pragma once
#include "BaseSystem.h"
#include "../Input/InputCommand.h"
class RigidBody;
//class JumpCommand final : public InputCommand
//{
//	~JumpCommand();
//};

class InputSystem : public BaseSystem
{
public:
	InputSystem(NodeEntityManager& anEntityManager);
	void Update(float delta_time) override;

};

