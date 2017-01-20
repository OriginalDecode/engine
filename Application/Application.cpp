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
#include <imgui.h>
bool Application::Initiate()
{
	myEngine = Hex::Engine::GetInstance();
	myCamera = myEngine->GetCamera();

	mySynchronizer = myEngine->GetSynchronizer();
	myGame = new Game;
	if (!myGame->Initiate())
		return false;


	//Keep at the end of initiate...
	myLogicThread = new std::thread([&] { Application::Update(); });
	return true;
}

void Application::Update()
{
	while (mySynchronizer->HasQuit() == false)
	{
		float deltaTime = myEngine->GetDeltaTime();

		myEngine->UpdateInput();
		myGame->Update(deltaTime);

		/*std::stringstream ss;
		ss << "m_camera_position\n" <<
		"X : " << myCamera->GetPosition().x << "\n" <<
		"Y : " << myCamera->GetPosition().y << "\n" <<
		"Z : " << myCamera->GetPosition().z;

		myEngine->AddDebugText(ss.str());*/
		//myEngine->Render();
		
		mySynchronizer->LogicIsDone();
		mySynchronizer->WaitForRender();
	}
	myQuitFlag = true;
}

void Application::OnPause()
{
	myEngine->OnPause();
}

void Application::OnResume()
{
	myEngine->OnResume();
}

void Application::OnInactive()
{
	myEngine->OnInactive();
}

void Application::OnActive()
{
	myEngine->OnActive();
}

void Application::OnExit()
{
	myGame->CleanUp();
	myEngine->OnExit();
	CleanUp();
}

void Application::OnAltEnter()
{
	myEngine->OnAltEnter();
}

bool Application::HasQuit()
{
	return myQuitFlag;
}

bool Application::CleanUp()
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
