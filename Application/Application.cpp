#include "Application.h"
#include <thread>

#include <Engine.h>
#include <Camera.h>

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
	myEngine = Snowblind::Engine::GetInstance();
	myCamera = myEngine->GetCamera();

	mySynchronizer = myEngine->GetSynchronizer();
	myGame = new CGame;
	if (!myGame->Initiate(mySynchronizer))
		return false;

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
		ss << "m_camera_position\n" <<
		"X : " << myCamera->GetPosition().x << "\n" <<
		"Y : " << myCamera->GetPosition().y << "\n" <<
		"Z : " << myCamera->GetPosition().z;

		myEngine->AddDebugText(ss.str());
//		mySynchronizer->AddRenderCommand(RenderCommand(eType::SKYSPHERE, myCamera->GetPosition()));

		myGame->Update(deltaTime);
		Snowblind::Engine::GetInstance()->Render();

		mySynchronizer->LogicIsDone();
		mySynchronizer->WaitForRender();
	}
	myQuitFlag = true;
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
	myEngine->OnInactive();
}

void CApplication::OnActive()
{
	myEngine->OnActive();
}

void CApplication::OnExit()
{
	myEngine->OnExit();
	CleanUp();
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

	if (!myGame->CleanUp())
		return false;

	SAFE_DELETE(myGame);
	if (myGame)
		return false;

	return true;
}
