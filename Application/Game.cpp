#include "Game.h"

#include <EntityManager.h>

#include <Synchronizer.h>
#include <RenderCommand.h>
#include <Line3D.h>

#include <DL_Debug.h>
#include <Engine.h>
#include <Utilities.h>
#include <MousePicker.h>

#include <EngineDefines.h>

#include <Camera.h>
#include "../Input/InputHandle.h"

#include <RenderCommand_Shared.h>
#include <PostMaster.h>
#include <OnLeftClick.h>
#include <StateStack.h>
#include <Math/Vector/Vector.h>
#include "../Physics/PhysicsManager.h"
#include "../EntitySystem/EntityManager.h"
#include <imgui.h>
#include "CameraHandle.h"

void Game::InitState(StateStack* state_stack)
{
	m_StateStack = state_stack;

	m_Engine = Engine::GetInstance();
	m_Synchronizer = m_Engine->GetSynchronizer();

	//m_World.Initiate(CU::Vector3f(256, 256, 256)); //Might be a v2 instead and a set y pos 

	CU::GrowingArray<TreeDweller*> dwellers = m_Engine->LoadLevel("Data/Levels/level_01.level");

	//m_World.AddDwellers(dwellers);


	m_Picker = new CMousePicker;

	//m_Engine->ToggleVsync(); //settings
	m_Camera = m_Engine->GetCamera();

	CameraHandle::Create();
	CameraHandle::GetInstance()->Initiate();
	m_PauseState.InitState(m_StateStack);
	//component = &m_Engine->GetEntityManager().GetComponent<TranslationComponent>(0);
}

void Game::EndState()
{
	CameraHandle::Destroy();
	SAFE_DELETE(m_Picker);
}

void Game::Render(bool render_through)
{
}

void Game::Update(float dt)
{
	Render(true);
	if (m_Paused)
		return;
	CameraHandle::GetInstance()->Update();
	m_FrameCount++;
	m_AverageFPS += m_Engine->GetFPS();
	m_Time -= dt;
	if (m_Time <= 0.f)
	{
		m_FPSToPrint = u32(m_AverageFPS / m_FrameCount);
		m_FrameCount = 0;
		m_AverageFPS = 0.f;
		m_Time = 1.f;
	}

	InputWrapper* input_wrapper = m_Engine->GetInputHandle()->GetInputWrapper();
	if (input_wrapper->OnClick(MouseInput::LEFT))
	{
		CU::Vector3f ray_dir = m_Picker->GetCurrentRay(input_wrapper->GetCursorPos());
		PostMaster::GetInstance()->SendMessage(OnLeftClick(ray_dir.x, ray_dir.y, ray_dir.z, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z));
	}
	Engine::GetInstance()->GetEntityManager().Update(dt);
	if (input_wrapper->OnDown(KButton::ESCAPE))
	{
		m_StateStack->PopCurrentMainState();
	}

	if (input_wrapper->IsDown(MouseInput::RIGHT))
	{
		m_Camera->Update(m_Engine->GetInputHandle()->GetDeltaCursorPos());
	}

	{
		if (input_wrapper->IsDown(KButton::UP_ARROW))
			m_Camera->RotateAroundX(0.5f * dt);
		if (input_wrapper->IsDown(KButton::DOWN_ARROW))
			m_Camera->RotateAroundX(-0.5f * dt);
		if (input_wrapper->IsDown(KButton::RIGHT_ARROW))
			m_Camera->RotateAroundY(0.5f * dt);
		if (input_wrapper->IsDown(KButton::LEFT_ARROW))
			m_Camera->RotateAroundY(-0.5f * dt);
	}

	float cam_speed = 5.f;

	if (input_wrapper->IsDown(KButton::W))
		m_Camera->Move(eDirection::FORWARD, cam_speed * dt);
	if (input_wrapper->IsDown(KButton::S))
		m_Camera->Move(eDirection::BACK, -cam_speed * dt);
	if (input_wrapper->IsDown(KButton::A))
		m_Camera->Move(eDirection::LEFT, -cam_speed * dt);
	if (input_wrapper->IsDown(KButton::D))
		m_Camera->Move(eDirection::RIGHT, cam_speed * dt);
	if (input_wrapper->IsDown(KButton::SPACE))
		m_Camera->Move(eDirection::UP, cam_speed * dt);
	if (input_wrapper->IsDown(KButton::X))
		m_Camera->Move(eDirection::DOWN, -cam_speed * dt);



	//m_World.Update(dt);
}