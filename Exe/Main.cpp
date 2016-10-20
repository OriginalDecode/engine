#include <Windows.h>
#include <string>
#include "../Application/Application.h"
#include "../DL_Debug/DL_Debug.h"
#include "../Engine/Engine.h"
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CApplication* newApplication = nullptr;
int WINAPI WinMain(HINSTANCE anInstance, HINSTANCE, LPSTR someCommandLines, int)
{
	DL_Debug::Debug::Create();
	//double res16x9 = 1.777777777777777777777777777777778; best
	const char* inputString = someCommandLines;
	std::string input(inputString);

	DL_Debug::Debug::GetInstance()->ActivateFilter(DL_Debug::Debug::Engine);
	DL_Debug::Debug::GetInstance()->ActivateFilter(DL_Debug::Debug::Update);
	DL_Debug::Debug::GetInstance()->ActivateFilter(DL_Debug::Debug::Render);
	DL_Debug::Debug::GetInstance()->ActivateFilter(DL_Debug::Debug::Resource);
	DL_Debug::Debug::GetInstance()->ActivateFilter(DL_Debug::Debug::Physics);
	DL_Debug::Debug::GetInstance()->ActivateFilter(DL_Debug::Debug::Font);
	DL_Debug::Debug::GetInstance()->ActivateFilter(DL_Debug::Debug::Model);

	DL_MESSAGE("%i Active Filters!", DL_Debug::Debug::GetInstance()->GetActiveLogCount());

	float w = 1920;
	float h = 1080;
	newApplication = new CApplication();

	Snowblind::CEngine::Create();
	Snowblind::CEngine::GetInstance()->Initiate(w, h, anInstance, WindowProc);
	newApplication->Initiate(w, h);

	MSG msg;
	bool applicationIsRunning = true;
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				applicationIsRunning = false;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (newApplication->HasQuit() == true)
		{
			applicationIsRunning = false;
			break;
		}
		Snowblind::CEngine::Update();

	} while (applicationIsRunning == true);

	delete newApplication;
	newApplication = nullptr;
	Snowblind::CEngine::Destroy();
	DL_Debug::Debug::Destroy();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_SIZE:
		newApplication->OnAltEnter();
		break;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
			newApplication->OnInactive();
		else
			newApplication->OnActive();
		break;
	case WM_ENTERSIZEMOVE:
		newApplication->OnPause();
		break;
	case WM_EXITSIZEMOVE:
		newApplication->OnResume();
		break;
	case WM_CLOSE:
		Snowblind::CEngine::GetInstance()->OnExit();
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}