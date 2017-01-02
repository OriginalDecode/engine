#include <Windows.h>
#include <string>
#include "../Application/Application.h"
#include "../DL_Debug/DL_Debug.h"
#include "../Engine/Engine.h"
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Application* newApplication = nullptr;
int WINAPI WinMain(HINSTANCE anInstance, HINSTANCE, LPSTR someCommandLines, int)
{
	DL_Debug::Debug::Create();
	//double res16x9 = 1.777777777777777777777777777777778; best
	const char* inputString = someCommandLines;
	std::string input(inputString);

	DL_Debug::Debug::GetInstance()->ActivateFilters(Update_Filter | Render_Filter | Physics_Filter | Resource_Filter | Engine_Filter | Font_Filter | Model_Filter);

	float w = 1920;
	float h = 1080;
	newApplication = new Application();

	Hex::Engine::Create();
	Hex::Engine::GetInstance()->Initiate(w, h, anInstance, WindowProc);
	DL_ASSERT_EXP(newApplication->Initiate(), "Failed to initiate game");

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
		Hex::Engine::GetInstance()->Update();

	} while (applicationIsRunning == true);

	delete newApplication;
	newApplication = nullptr;
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
		newApplication->OnExit();
		DL_Debug::Debug::Destroy();
		Hex::Engine::Destroy();
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}