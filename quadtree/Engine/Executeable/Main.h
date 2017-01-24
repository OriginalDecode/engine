#pragma once
#include <thread>
#include "App\Client.h"
class Synchronizer;
class Renderer;
class HGE;

class Main
{

public:

	Main(HGE& aHGE);
	~Main();

	void Initiate(HINSTANCE aInstance);
	void Run();
	void Render();


private:
	bool myQuit;
	HGE& myHGE;
	Client myClient;
	Renderer myRenderer;
	std::thread* myRenderThread;
	Synchronizer mySynchronizer;


	float myDelta;
	float myAverageFPS;
};