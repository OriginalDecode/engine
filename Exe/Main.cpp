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
	DL_DEBUG("%i Active Filters!", DL_Debug::Debug::_COUNT);

	Snowblind::eAPIFlag apiFlag;
	float w = 1280.f;
	float h = 720.f;
	if (input == "DX11")
	{
		newApplication = new CApplication();
		apiFlag = Snowblind::eAPIFlag::DIRECTX11;
	}
	else if (input == "DX12")
	{
		newApplication = new CApplication();
		apiFlag = Snowblind::eAPIFlag::DIRECTX12;
	}
	else if (input == "GL")
	{
		newApplication = new CApplication();
		apiFlag = Snowblind::eAPIFlag::OPENGL;
	}
	else if (input == "Vulkan")
	{
		newApplication = new CApplication();
		apiFlag = Snowblind::eAPIFlag::VULKAN;
	}
	Snowblind::CEngine::Create(apiFlag, w, h, anInstance, WindowProc);
	newApplication->Initiate(w, h);

	MSG msg;
	bool applicationIsRunning = true;
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (newApplication->Update() == false)
		{
			break;
		}

	} while (applicationIsRunning == true);

	delete newApplication;
	newApplication = nullptr;
	DL_Debug::Debug::Destroy();

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
		break;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
		}
		else
		{
		}
		break;
	case WM_ENTERSIZEMOVE:
			newApplication->OnPause();
		break;
	case WM_EXITSIZEMOVE:
		newApplication->OnResume();
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}