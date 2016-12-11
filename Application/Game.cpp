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

bool Game::Initiate()
{
	myEngine = Snowblind::Engine::GetInstance();
	mySynchronizer = myEngine->GetSynchronizer();

	myEngine->LoadLevel("Data/Levels/level_01.json");

	myPicker = new Snowblind::CMousePicker;

	myEngine->ToggleVsync(); //settings
	m_Camera = myEngine->GetCamera();
	return true;
}

bool Game::CleanUp()
{
	SAFE_DELETE(myPicker);
	if (myPicker)
		return false;

	myTerrain.RemoveAll();
	if (myTerrain.Size() > 0)
		return false;

	return true;
}

void Game::Update(float dt)
{
	myFrameCount++;
	myAverageFPS += myEngine->GetFPS();
	myTime -= dt;
	if (myTime <= 0.f)
	{
		myFPSToPrint = static_cast<u32>(myAverageFPS / myFrameCount);
		myFrameCount = 0;
		myAverageFPS = 0.f;
		myTime = 1.f;
	}


	m_Camera->Update(myEngine->GetInputHandle()->GetDeltaCursorPos());

	InputWrapper* input_wrapper = myEngine->GetInputHandle()->GetInputWrapper();

	if (input_wrapper->IsDown(KButton::W))
		m_Camera->Move(Snowblind::eDirection::FORWARD, 50.f * dt);
	if (input_wrapper->IsDown(KButton::S))
		m_Camera->Move(Snowblind::eDirection::BACK, -50.f * dt);
	if (input_wrapper->IsDown(KButton::A))
		m_Camera->Move(Snowblind::eDirection::LEFT, -50.f * dt);
	if (input_wrapper->IsDown(KButton::D))
		m_Camera->Move(Snowblind::eDirection::RIGHT, 50.f * dt);
	if (input_wrapper->IsDown(KButton::SPACE))
		m_Camera->Move(Snowblind::eDirection::UP, 50.f * dt);
	if (input_wrapper->IsDown(KButton::X))
		m_Camera->Move(Snowblind::eDirection::DOWN, -50.f * dt);




	CU::Vector2f cur_pos = myEngine->GetInputHandle()->GetDeltaCursorPos();
	std::stringstream ss;
	ss << "FPS : " << myEngine->GetFPS() << "\n" << "Average FPS : " << myFPSToPrint << "\nDeltaTime:" << dt << "\n" << Snowblind::Engine::GetInstance()->GetLocalTimeAsString() << "\ncursor_pos : \n" << cur_pos.x << "\n" << cur_pos.y;
	myEngine->AddDebugText(ss.str());


	mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, "Data/Model/cube.fbx", pointHit));

	//mySynchronizer->AddRenderCommand(RenderCommand(eType::TERRAIN)); This shouldn't even be a command. This should just be rendered

	myEngine->GetEntityManager()->Update(dt);
}