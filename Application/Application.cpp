#include "Application.h"
#include <thread>

#include <Engine.h>
#include <Synchronizer.h>
#include <RenderCommand.h>

#include <sstream>
#include "Game.h"
#include <imgui.h>
#include "../Input/InputHandle.h"
#include "../Input/InputWrapper.h"
#include "../CommonLib/Utilities.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
bool Application::Initiate()
{
	myEngine = Engine::GetInstance();
	mySynchronizer = myEngine->GetSynchronizer();

	/*myGame = new Game;
	if (!myGame->Initiate())
		return false;*/

	m_States.PushState(&m_Game, StateStack::MAIN);
	//Keep at the end of initiate...

	myLogicThread = new std::thread([&] { Application::Update(); });
	CL::SetThreadName(myLogicThread->get_id(), "Logic Thread");

	return true;
}

void Application::Update()
{
	while (mySynchronizer->HasQuit() == false)
	{
		float deltaTime = myEngine->GetDeltaTime();

		myEngine->UpdateInput();
		InputWrapper* input_wrapper = myEngine->GetInputHandle()->GetInputWrapper();
		if (input_wrapper->OnDown(KButton::P))
		{
			m_States.PauseCurrentState();
		}

		if (input_wrapper->OnDown(KButton::O))
		{
			m_States.ResumeCurrentState();
		}

		if (!m_States.UpdateCurrentState(deltaTime))
		{
			mySynchronizer->Quit();
		}

		//myGame->Update(deltaTime);

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
	//myGame->CleanUp();
	m_States.Clear();
	myEngine->OnExit();
	CleanUp();
}

void Application::OnAltEnter()
{
	//myEngine->OnResize();
	myEngine->OnAltEnter();
}

void Application::OnResize()
{
	myEngine->OnResize();
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

	return true;
}
