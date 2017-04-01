
#include <Engine/Engine.h>
#include <Engine/imgui_impl_dx11.h>
#include <Engine/VirtualFileSystem.h>

#include <DL_Debug/DL_Debug.h>

#include <Application/Application.h>

#include <Windows.h>
#include <string>

#ifdef _PROFILE
#include <easy/profiler.h>
#endif

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

//#define _REMOTE_PROFILE

int WINAPI WinMain(HINSTANCE anInstance, HINSTANCE, LPSTR someCommandLines, int)
{
#ifdef _PROFILE
	EASY_PROFILER_ENABLE;
#ifdef _REMOTE_PROFILE
	profiler::startListen();
#endif
#endif
	DL_Debug::Debug::Create();
	//do/uble res16x9 = 1.777777777777777777777777777777778; best
	const char* inputString = someCommandLines;
	std::string input(inputString);

	DL_Debug::Debug::GetInstance()->ActivateFilters(Update_Filter | Render_Filter | Physics_Filter | Resource_Filter | Engine_Filter | Font_Filter | Model_Filter);

	float w = 1920;
	float h = 1080;
	newApplication = new Application;

	Engine::Create();
	
	Engine::GetInstance()->GetVFS().Register("Data/Shaders", "Shaders");
	Engine::GetInstance()->GetVFS().Register("Data/Textures", "Textures");
	Engine::GetInstance()->GetVFS().Register("Data/Model", "Models");

	Engine::GetInstance()->Initiate(w, h, anInstance, WindowProc);
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
	//rid[0].hwndTarget = Engine::GetInstance()->GetHWND();

	//if (RegisterRawInputDevices(rid, 1, sizeof(rid[0])) == FALSE)
	//{
	//	DL_MESSAGE("Failed to Register input device!");
	//}
	//
	/*
		raw input can block other windows messages. Is this something I want?>
	
	*/

	g_windowactive = true;
	//ShowCursor(false);
	MSG msg;
	bool applicationIsRunning = true;
	do
	{
		ImGui_ImplDX11_NewFrame();
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT || msg.message == WM_CLOSE)
		{
			applicationIsRunning = false;
			break;
		}

		if (newApplication->HasQuit() == true)
		{
			applicationIsRunning = false;
			break;
		}
		Engine::GetInstance()->Update();
		/*if (g_windowactive)
		{
			SetCursorPos(w / 2.f, h / 2.f);
		}*/


	} while (applicationIsRunning == true);

	newApplication->OnExit();
	delete newApplication;
	newApplication = nullptr;

	DL_Debug::Debug::Destroy();
	Engine::Destroy();

#ifdef _PROFILE
#ifdef _REMOTE_PROFILE
	profiler::stopListen();
#endif
	profiler::dumpBlocksToFile("file.prof");
	EASY_PROFILER_DISABLE;
#endif
	return 0;
}

extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplDX11_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_SIZE:
		//newApplication->OnAltEnter();
		//newApplication->OnResize();
		break;
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			//g_windowactive = false;
			newApplication->OnInactive();
			//ShowCursor(!g_windowactive);
		}
		else
		{
			//g_windowactive = true;
			newApplication->OnActive();
			//ShowCursor(!g_windowactive);
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
		/* Unsure that this is needed since I can cleanup when the loop cancel */
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN)
		{
			if ((HIWORD(lParam) & KF_ALTDOWN))
				newApplication->OnAltEnter();
		}
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}