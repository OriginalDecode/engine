#pragma once
#include "State.h"
#include "PauseState.h"
namespace Hex
{
	class Synchronizer;
	class Engine;
	class CMousePicker;
	class Camera;
};


class Game : public State
{
public:
	Game() = default;

	void InitState(StateStack* state_stack) override;
	void Update(float aDeltaTime) override;

	void EndState() override;
	void Render(bool render_through) override;

private:
	CPauseState m_PauseState;

	Hex::Camera* m_Camera = nullptr;
	Hex::Engine* m_Engine = nullptr;
	Hex::CMousePicker* m_Picker = nullptr;
	Hex::Synchronizer* m_Synchronizer = nullptr;
	int m_FrameCount = 0;
	int m_FPSToPrint = 0;
	float m_AverageFPS = 0;
	float m_Time = 0.f;

	const char* m_ModelKey;
};