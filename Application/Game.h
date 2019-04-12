#pragma once

#include <Engine/State.h>
#include <Engine/World.h>

#include "PauseState.h"
#include "DebugState.h"

class Camera;
class Game : public State
{
public:
	Game() = default;
	~Game() = default;

	void InitState(StateStack* state_stack) override;
	void EndState() override;
	void Render(bool render_through) override;
	void Update(float aDeltaTime) override;


	void Initiate(const std::string& level);

	void OldUpdate(float dt);


private:
	void Reload();
	DebugState m_DebugState;
	CPauseState m_PauseState;

	Camera* m_Camera = nullptr;

	World m_World;
	CU::Matrix44f m_Orientation;
};