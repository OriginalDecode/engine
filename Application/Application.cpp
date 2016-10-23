#include "Application.h"
#include <thread>

#include <Engine.h>
#include <Camera.h>

#include "../Input/InputWrapper.h"
#include <Sprite.h>

#include <PointLight.h>

#include <Synchronizer.h>
#include <RenderCommand.h>
#include <SystemMonitor.h>
#include <EngineDefines.h>
#include <sstream>

#include "Game.h"

bool CApplication::Initiate()
{
	myEngine = Snowblind::CEngine::GetInstance();
	myCamera = myEngine->GetCamera();

	mySynchronizer = myEngine->GetSynchronizer();
	myGame = new CGame(mySynchronizer);

	//Keep at the end of initiate...
	myLogicThread = new std::thread([&] { CApplication::Update(); });
	return true;
}

void CApplication::Update()
{
	while (mySynchronizer->HasQuit() == false)
	{
		float deltaTime = myEngine->GetDeltaTime();

		std::stringstream ss;
		ss << "X : " << myCamera->GetPosition().x << "\n" <<
		"Y : " << myCamera->GetPosition().y << "\n" <<
		"Z : " << myCamera->GetPosition().z << "\n";

		mySynchronizer->AddRenderCommand(RenderCommand(ss.str(), CU::Vector2f(500, 0)));
		mySynchronizer->AddRenderCommand(RenderCommand(eType::SKYSPHERE, myCamera->GetPosition()));

		UpdateInput(deltaTime);

		myGame->Update(deltaTime);
		mySynchronizer->LogicIsDone();
		mySynchronizer->WaitForRender();
	}
	myQuitFlag = true;
}

void CApplication::UpdateInput(float /*aDeltaTime*/)
{
	CU::Input::InputWrapper::GetInstance()->Update();
	if (myWindowIsActive)
	{
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(ESCAPE))
		{
			myEngine->OnExit();
			myQuitFlag = true;
		}
	}
}

void CApplication::OnPause()
{
	myEngine->OnPause();
}

void CApplication::OnResume()
{
	myEngine->OnResume();
}

void CApplication::OnInactive()
{
	CU::Input::InputWrapper::SetActiveWindow(false);
	myWindowIsActive = false;
}

void CApplication::OnActive()
{
	CU::Input::InputWrapper::SetActiveWindow(true);
	myWindowIsActive = true;
}

void CApplication::OnExit()
{

}

void CApplication::OnAltEnter()
{
	myEngine->OnAltEnter();
}

bool CApplication::HasQuit()
{
	return myQuitFlag;
}

bool CApplication::CleanUp()
{
	myLogicThread->join();

	SAFE_DELETE(myLogicThread);
	if (myLogicThread)
		return false;

	SAFE_DELETE(myGame);
	if (myGame)
		return false;

	return true;
}
