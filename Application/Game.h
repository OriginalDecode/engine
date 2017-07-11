#pragma once

#include <Frustum.h>
#include <State.h>
#include <World.h>
#include "../EntitySystem/TranslationComponent.h"
#include "PauseState.h"

class Synchronizer;
class Engine;
class CMousePicker;
class Camera;
class Game : public State
{
public:
	Game() = default;

	void InitState(StateStack* state_stack) override;

	void Initiate(const std::string& level);

	void Update(float aDeltaTime) override;

	void AddRenderCommand(const struct ModelCommand& command);

	void EndState() override;
	void Render(bool render_through) override;



private:
	World m_World;
	CPauseState m_PauseState;
	CU::Vector3f pointHit;
	TranslationComponent* component;
	TreeDweller* m_Player = nullptr;
	Camera* m_Camera = nullptr;
	Frustum m_Frustum;

	CU::Matrix44f m_Orientation;


	Engine* m_Engine = nullptr;
	CMousePicker* m_Picker = nullptr;
	Synchronizer* m_Synchronizer = nullptr;
	int m_FrameCount = 0;
	int m_FPSToPrint = 0;
	float m_AverageFPS = 0;
	float m_Time = 0.f;

	const char* m_ModelKey;
};