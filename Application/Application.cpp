#include "Application.h"
#include <thread>

#include <Engine.h>
#include <Synchronizer.h>
#include <RenderCommand.h>

#include <sstream>
#include "Game.h"
#include "../Input/InputHandle.h"
#include "../Input/InputWrapper.h"
#include "../CommonLib/Utilities.h"


#ifdef _PROFILE
#include <easy/profiler.h>
#endif
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
#ifdef _DEBUG
	cl::SetThreadName(myLogicThread->get_id(), "Logic Thread");
#endif


	return true;
}

void Application::Update()
{
#if defined (_WIN32) || (_WIN64)
	CoInitialize(0);
#endif

#ifdef _PROFILE
	EASY_THREAD_SCOPE("LogicThread");
#endif
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
			break;
		}

		mySynchronizer->LogicIsDone();
		mySynchronizer->WaitForRender();
	}
	myQuitFlag = true;
}

void Application::OnPause()
{
	if (myEngine)
		myEngine->OnPause();
}

void Application::OnResume()
{
	if (myEngine)
		myEngine->OnResume();
}

void Application::OnInactive()
{
	if(myEngine)
		myEngine->OnInactive();
}

void Application::OnActive()
{
	if (!myEngine)
		return;
	myEngine->OnActive();
}

void Application::OnExit()
{
	m_States.Clear();
	myEngine->OnExit();
	CleanUp();
}

void Application::OnAltEnter()
{
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
	if ( !myLogicThread )
		return true;

	myLogicThread->join();

	SAFE_DELETE(myLogicThread);
	if (myLogicThread)
		return false;

	return true;
}
