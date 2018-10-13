

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
	m_Synchronizer = myEngine->GetSynchronizer();
	m_States.PushState(&m_Game, StateStack::MAIN);

	//Keep at the end of initiate...

	myLogicThread = new std::thread([&] { Application::Update(); });
	cl::SetThreadName(myLogicThread->get_id(), "Logic Thread");
#if defined (_WIN32) || (_WIN64)
	CoInitialize(0);
#endif

	m_Initialized = true;
	return m_Initialized;
}

void Application::Update()
{


#ifdef _PROFILE
	EASY_THREAD_SCOPE("LogicThread");
#endif
	while (!m_Synchronizer->HasQuit())
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
			m_Synchronizer->LogicIsDone();
			m_Synchronizer->WaitForRender();
			break;
		}

		m_Synchronizer->LogicIsDone();
		m_Synchronizer->WaitForRender();
	}
	myQuitFlag = true;
}

void Application::OnPause()
{
	if (!m_Initialized)
		return;

	myEngine->OnPause();
}

void Application::OnResume()
{
	if (!m_Initialized)
		return;

	myEngine->OnResume();
}

void Application::OnInactive()
{
	if (!m_Initialized)
		return;

	myEngine->OnInactive();
}

void Application::OnActive()
{
	if (!m_Initialized)
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
	if (!m_Initialized)
		return;

	myEngine->OnAltEnter();
}

void Application::OnResize()
{
	if (!m_Initialized)
		return;

	myEngine->OnResize();
}

bool Application::HasQuit()
{
	return myQuitFlag;
}

bool Application::CleanUp()
{
	if (!myLogicThread)
		return true;

	myLogicThread->join();

	SAFE_DELETE(myLogicThread);
	if (myLogicThread)
		return false;

	return true;
}
