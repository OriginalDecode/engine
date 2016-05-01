#include "stdafx.h"

namespace Snowblind
{
	CEngine* CEngine::myInstance = nullptr;
	CDirectX11* CEngine::myAPI = nullptr;

	CEngine::CEngine(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc)
		: myWindowSize(aWindowWidth, aWindowHeight)
	{
		CreateAppWindow(anInstance, aWndProc);
		CU::Input::InputWrapper::Create(myHWND, anInstance);
		myAPI = new CDirectX11(myHWND, aWindowWidth, aWindowHeight);

		myTimeManager = new CU::TimeManager();

		myFontManager = new CFontManager();
		myFontManager->Initiate();
	}

	CEngine::~CEngine()
	{

		SAFE_DELETE(myAPI);
		SAFE_DELETE(myFontManager);
		SAFE_DELETE(myTimeManager);
		CU::Input::InputWrapper::Destroy();
	}


	void CEngine::Create(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc)
	{
		DL_ASSERT_EXP(myInstance == nullptr, "Instance already created!");
		myInstance = new CEngine(aWindowWidth, aWindowHeight, anInstance, aWndProc);
	}

	void CEngine::Destroy()
	{
		DL_ASSERT_EXP(myInstance != nullptr,"Can't destroy the instance before it's created. Did you call Destroy twice?");
		SAFE_DELETE(myInstance);
	}

	CEngine* CEngine::GetInstance()
	{
		return myInstance;
	}

	CDirectX11* CEngine::GetDirectX()
	{
		return myAPI;
	}


	void CEngine::Update()
	{
#ifdef _DEBUG
		CEffectContainer::GetInstance()->Update();
#endif
		myInstance->myTimeManager->Update();
	}

	void CEngine::Present()
	{
		myInstance->myAPI->Present();
	}

	void CEngine::Clear()
	{
		myInstance->myAPI->Clear();
	}

	const Snowblind::SWindowSize& CEngine::GetWindowSize() const
	{
		return myWindowSize;
	}

	CFont* CEngine::LoadFont(const char* aFilepath, short aFontWidth)
	{
		return myFontManager->LoadFont(aFilepath, aFontWidth);
	}

	const float CEngine::GetDeltaTime()
	{
		return myTimeManager->GetDeltaTime();
	}

	const float CEngine::GetFPS()
	{
		return myTimeManager->GetFPS();
	}

	const char* CEngine::GetAPIName()
	{
		return myAPI->GetAPIName();
	}

	void CEngine::OnPause()
	{
		myTimeManager->Pause();
	}

	void CEngine::OnResume()
	{
		myTimeManager->Start();
	}

	void CEngine::CreateAppWindow(HINSTANCE anInstance, WNDPROC aWndProc)
	{
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = aWndProc;
		wc.hInstance = anInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = "WindowsClass";

		RegisterClassEx(&wc);
		float width = (GetSystemMetrics(SM_CXSCREEN)* 0.5f) - (myWindowSize.myWidth * 0.5f);
		float height = (GetSystemMetrics(SM_CYSCREEN)* 0.5f) - (myWindowSize.myHeight * 0.5f);
		myHWND = CreateWindow(
			"WindowsClass",
			NULL, WS_OVERLAPPEDWINDOW, //Windowed
			static_cast<int>(width),
			static_cast<int>(height),
			static_cast<int>(myWindowSize.myWidth),
			static_cast<int>(myWindowSize.myHeight),
			NULL, NULL,
			GetModuleHandle(NULL), NULL);

		ShowWindow(myHWND, true);
	}
};