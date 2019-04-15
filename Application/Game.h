#pragma once

#include <Engine/State.h>
#include <Engine/World.h>

#include "PauseState.h"
#include "DebugState.h"
#include <CommonLib/Timer/Timer.h>
#include <Math/Matrix/Matrix.h>

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

	float m_Time = 0.f;
	float m_AverageFPS = 0.f;
	int m_FrameCount = 0;
	float m_FPSToPrint = 0.f;

	void Reload();
	DebugState m_DebugState;
	CPauseState m_PauseState;
	Base::Timer m_Timer;
	Camera* m_Camera = nullptr;

	World m_World;
	CU::Matrix44f m_Orientation;
};