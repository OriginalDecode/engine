#include <Windows.h>
#include <hge.h>
#include "DL_Debug.h"
#include "Main.h"
#include "Renderer\Synchronizer.h"
#include "Renderer\Renderer.h"
#include "Renderer\RenderCommand.h"
#include "Misc\Megaton.h"
float x = 0;
float y = 0;

Main::Main(HGE& aHGE) :
myRenderer(aHGE, mySynchronizer),
myClient(aHGE, mySynchronizer),
myHGE(aHGE)
{
}

Main::~Main()
{
	delete myRenderThread;
	myRenderThread = nullptr;
	CU::Megaton::Destroy();

	DL_Debug::Debug::Destroy(); //This should always be destroyed last because of Debug messages
}



void HGEShutdown(HGE* aHGEInstance)
{
	aHGEInstance->System_Shutdown();
	aHGEInstance->Release();
}

void Main::Initiate(HINSTANCE aInstance)
{
	myClient.Initiate(aInstance, GetActiveWindow(), x, y);
	myRenderer.Initiate();
	myDelta = 1.f;
	myRenderThread = new std::thread(&Renderer::Render, myRenderer);
}



void Main::Run()
{
	MSG msg;
	while (myQuit == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				myQuit = true;
			}
			else
			{
				DispatchMessage(&msg);
			}
		}
		else
		{
			myQuit = myClient.Update();

			std::stringstream title;
			title << "FPS : " << TIME_HANDLER->GetFPS() << " | " "AssignmentNameHere";
			myHGE.System_SetState(HGE_TITLE, title.str().c_str());

		
		}
	}
	mySynchronizer.Quit();
	myRenderThread->join();
}


int WINAPI WinMain(HINSTANCE aInstance, HINSTANCE, LPSTR, int)
{
	HGE* hgeEngine = nullptr;
	DL_Debug::Debug::Create();
	CU::Megaton::Create();
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	x = float(desktop.right);
	y = float(desktop.bottom);


	hgeEngine = hgeCreate(HGE_VERSION);
	hgeEngine->System_SetState(HGE_WINDOWED, true);
	hgeEngine->System_SetState(HGE_USESOUND, false);

	hgeEngine->System_SetState(HGE_TITLE, "Basic Window");
	hgeEngine->System_SetState(HGE_HIDEMOUSE, false);

	hgeEngine->System_SetState(HGE_SCREENWIDTH, 1200);
	hgeEngine->System_SetState(HGE_SCREENHEIGHT, 900);

	hgeEngine->System_SetState(HGE_SHOWSPLASH, false);


	DL_PRINT("HGE Initiated.");

	srand(static_cast<unsigned int>(GetTickCount()));

	if (hgeEngine->System_Initiate())
	{
		Main game(*hgeEngine);
		game.Initiate(aInstance);
		game.Run();

	}
	else
	{
		MessageBox(NULL, hgeEngine->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	HGEShutdown(hgeEngine);


	return 0;
}
