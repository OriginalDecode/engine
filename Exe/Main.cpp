#include <Windows.h>
#include <string>
#include "../Application/Application.h"
#include "../DL_Debug/DL_Debug.h"
#include "../Engine/Engine.h"
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

enum ERawInputType
{
	POINTER = 0x01,
	MOUSE = 0x02,
	JOYSTICK = 0x04,
	GAMEPAD = 0x05,
	KEYBOARD = 0x06,
	KEYPAD = 0x07,
	SYSTEM_CONTROL = 0x80,
	CONSUMER_AUDIO_CONTROL = 0x01
};

enum class EUsagePage
{
	DEFAULT = 0x01,
	CONSUMER_AUDIO_CONTROL = 0x0C
};

Application* newApplication = nullptr;
static bool g_windowactive = false;

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

	PRAWINPUTDEVICELIST device_list = 0;
	UINT device_count;

	if (GetRawInputDeviceList(NULL, &device_count, sizeof(RAWINPUTDEVICELIST)) != 0)
	{
		//failed
	}
	device_list = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * device_count);
	int dev = 0;
	if (dev = GetRawInputDeviceList(device_list, &device_count, sizeof(RAWINPUTDEVICELIST)) == ((UINT)-1))
	{
		//failed
	}
	//
	//RAWINPUTDEVICE rid[1];
	//rid[0].usUsagePage = (USHORT)EUsagePage::DEFAULT;
	//rid[0].usUsage = MOUSE;
	//rid[0].dwFlags = 0;
	//rid[0].hwndTarget = Hex::Engine::GetInstance()->GetHWND();

	//if (RegisterRawInputDevices(rid, 1, sizeof(rid[0])) == FALSE)
	//{
	//	DL_MESSAGE("Failed to Register input device!");
	//}
	//
	/*
		raw input can block other windows messages. Is this something I want?>
	
	*/

	g_windowactive = true;
	ShowCursor(false);
	MSG msg;
	bool applicationIsRunning = true;
	do
	{
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			applicationIsRunning = false;
			break;
		}
		else if (msg.message == WM_INPUT)
		{
			int apa = 5;
		}


		if (newApplication->HasQuit() == true)
		{
			applicationIsRunning = false;
			break;
		}
		Hex::Engine::GetInstance()->Update();
		if (g_windowactive)
		{
			SetCursorPos(w / 2.f, h / 2.f);
		}

	} while (applicationIsRunning == true);

	delete newApplication;
	newApplication = nullptr;
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_SIZE:
		newApplication->OnAltEnter();
		break;
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			g_windowactive = false;
			newApplication->OnInactive();
			ShowCursor(!g_windowactive);
		}
		else
		{
			g_windowactive = true;
			newApplication->OnActive();
			ShowCursor(!g_windowactive);
		}
	}break;
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