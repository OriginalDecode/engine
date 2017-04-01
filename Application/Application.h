#pragma once
#include <StateStack.h>
#include "Game.h"

class Engine;
class Synchronizer;

namespace std
{
	class thread;
};
class Application
{
public:
	Application() = default;

	bool Initiate();

	void Update();

	void OnPause();
	void OnResume();
	void OnInactive();
	void OnActive();
	void OnExit();
	void OnAltEnter();
	void OnResize();
	bool HasQuit();
	bool CleanUp();
private:
	/* Threading */
	std::thread* myLogicThread;
	volatile bool myQuitFlag = false;

	Game m_Game;
	Engine* myEngine;
	Synchronizer* mySynchronizer;

	StateStack m_States;

};

