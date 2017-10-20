
#include <Engine/Engine.h>
#if !defined(_PROFILE) && !defined(_FINAL)
#include <Engine/imgui_impl_dx11.h>
#endif
#include <Engine/VirtualFileSystem.h>

#include <DL_Debug/DL_Debug.h>

#include <Application/Application.h>
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
#include <csignal>
void(*signal(int sig, void(*func)(int)))(int);


void signalHandler(int signum)
{
	MessageBox(NULL, "WAOH", "crash!", MB_ICONWARNING);
	exit(signum);
}


int WINAPI WinMain(HINSTANCE anInstance, HINSTANCE, LPSTR someCommandLines, int)
{


	//std::set_terminate([]() {   MessageBox(NULL, "WAOH", "crash!", MB_ICONWARNING); std::abort(); });
	signal(SIGINT, signalHandler);

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
	newApplication = new Application;

	Engine::Create();
	
	Engine* engine = Engine::GetInstance();


	engine->GetVFS().Register("Data/Shaders", "Shaders");
	engine->GetVFS().Register("Data/Textures", "Textures");
	engine->GetVFS().Register("Data/Model", "Models");
	engine->GetVFS().Register("Data/Texture", "Texture");

	engine->Initiate(w, h, anInstance, WindowProc);
	DL_ASSERT_EXP(newApplication->Initiate(), "Failed to initiate game");

// 	PRAWINPUTDEVICELIST device_list = 0;
// 	UINT device_count;
// 
// 	if (GetRawInputDeviceList(NULL, &device_count, sizeof(RAWINPUTDEVICELIST)) != 0)
// 	{
// 		//failed
// 	}
// 	device_list = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * device_count);
// 	if (GetRawInputDeviceList(device_list, &device_count, sizeof(RAWINPUTDEVICELIST)) == ((UINT)-1))
// 	{
// 		//failed
// 	}
// 
// 
// 	for (u32 i = 0; i < device_count; i++)
// 	{
// 		UINT size = 256;
// 		TCHAR tbuffer[256];
// 		if ( GetRawInputDeviceInfo(device_list[i].hDevice, RIDI_DEVICENAME, tbuffer, &size) < 0 )
// 		{
// 			// Error in reading device name
// 		}
// 
// 
// 		std::stringstream ss;
// 		std::string str(tbuffer);
// 
// 		if ( str.find("VID") != str.npos )
// 		{
// 			if ( str.find("1532") != str.npos )
// 			{
// 				ss << "RAZER | ";
// 			}
// 			else if ( str.find("045E") != str.npos )
// 			{
// 				ss << "MICROSOFT |";
// 			}
// 			else if ( str.find("0D8C") != str.npos )
// 			{
// 				ss << "C-Media Electronics | ";
// 			}
// 			else if ( str.find("046D") != str.npos )
// 			{
// 				ss << "Logitech | ";
// 			}
// 			else if ( str.find("8087") != str.npos )
// 			{
// 				ss << "Intel |  ";
// 			}
// 			else if ( str.find("9886") != str.npos )
// 			{
// 				ss << "Audio Device | ";
// 			}
// 			else if ( str.find("054C") != str.npos )
// 			{
// 				ss << "SONY | ";
// 			}
// 		}
// 
// 		ss << tbuffer << "\n";
// 
// 		OutputDebugString(ss.str().c_str());
// 
// 		RAWINPUTDEVICE rid[1];
// 		rid[0].usUsagePage = ( USHORT ) EUsagePage::DEFAULT;
// 		rid[0].usUsage = GAMEPAD;
// 		rid[0].dwFlags = 0;
// 		rid[0].hwndTarget = engine->GetHWND();
// 		if ( RegisterRawInputDevices(rid, 1, sizeof(rid[0])) == FALSE )
// 		{
// 			DL_MESSAGE("Failed to Register input device!");
// 		}
// 
// 
// 	}

	//if ( RegisterRawInputDevices(rid, 1, sizeof(rid[0])) == FALSE )
	//{
	//	DL_MESSAGE("Failed to Register input device!");
	//}

	//free(device_list);
	/*
		raw input can block other windows messages. Is this something I want?>
	
	*/

	g_windowactive = true;
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

		if (newApplication->HasQuit() == true)
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

	newApplication->OnExit();
	delete newApplication;
	newApplication = nullptr;

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