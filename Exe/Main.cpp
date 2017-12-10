
#include <Engine/Engine.h>
#if !defined(_PROFILE) && !defined(_FINAL)
#include <Engine/imgui_impl_dx11.h>
#endif
#include <Engine/VirtualFileSystem.h>

#include <DL_Debug/DL_Debug.h>

#include <Application/Application.h>
#include <string>
#include <PostMaster/EventManager.h>
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

Application* application = nullptr;
static bool s_WindowActive = false;
Engine* engine = nullptr;
int WINAPI WinMain(HINSTANCE anInstance, HINSTANCE, LPSTR someCommandLines, int)
{
#ifdef _PROFILE
	profiler::startListen();
#endif/* 1,777777777777777777777777777777778*/
	DL_Debug::Debug::Create();
	//double res16x9 = 1.777777777777777777777777777777778; best
	const char* inputString = someCommandLines;
	std::string input(inputString);

	DL_Debug::Debug::GetInstance()->ActivateFilters(Update_Filter | Render_Filter | Physics_Filter | Resource_Filter | Engine_Filter | Font_Filter | Model_Filter);

	float w = 1920;
	float h = 1080;
	application = new Application;

	Engine::Create();
	
	Engine* engine = Engine::GetInstance();


	engine->GetVFS().Register("Data/Shaders", "Shaders");
	engine->GetVFS().Register("Data/Textures", "Textures");
	engine->GetVFS().Register("Data/Model", "Models");
	engine->GetVFS().Register("Data/Texture", "Texture");

	engine->Initiate(w, h, anInstance, WindowProc);
	DL_ASSERT_EXP(application->Initiate(), "Failed to initiate game");

	s_WindowActive = true;
	//ShowCursor(false);
	MSG msg;
	bool applicationIsRunning = true;
	do
	{
#if !defined(_PROFILE) && !defined(_FINAL)
		ImGui_ImplDX11_NewFrame();
#endif
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

		if (application->HasQuit() == true)
		{
			applicationIsRunning = false;
			break;
		}
		engine->Update();
		/*if (g_windowactive)
		{
			SetCursorPos(w / 2.f, h / 2.f);
		}*/


	} while (applicationIsRunning == true);

	application->OnExit();
	delete application;
	application = nullptr;

	DL_Debug::Debug::Destroy();
	Engine::Destroy();
#ifdef _PROFILE
	profiler::stopListen();
#endif

	return 0;
}

#if !defined(_PROFILE) && !defined(_FINAL)
extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#if !defined(_PROFILE) && !defined(_FINAL)
	if (ImGui_ImplDX11_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
#endif

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
			if (EventManager::GetInstance())
				EventManager::GetInstance()->SendMessage(EngineEvents_OnInactive);
			//g_windowactive = false;
			application->OnInactive();
			//ShowCursor(!g_windowactive);
		}
		else
		{
			if(EventManager::GetInstance())
				EventManager::GetInstance()->SendMessage(EngineEvents_OnActive);
			//g_windowactive = true;
			application->OnActive();
			//ShowCursor(!g_windowactive);
		}
	}break;
	case WM_ENTERSIZEMOVE:
		//application->OnPause();
		break;
	case WM_EXITSIZEMOVE:

		application->OnResume();
		break;
	case WM_CLOSE:
		application->OnExit();
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
				application->OnAltEnter();
		}break;
	case WM_INPUT:
	{
		int apa;
		apa = 5;
	} break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}