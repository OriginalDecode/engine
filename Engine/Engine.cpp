#include "stdafx.h"
#include "Engine.h"
#include "DirectX11.h"
#include <Windows.h>
#include "EngineDefines.h"

#include <sstream>
#include <assert.h>
#include <TimeManager.h>
#include <InputWrapper.h>
namespace Snowblind
{
	CEngine* CEngine::myInstance = nullptr;

	CEngine::CEngine(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc)
		: myWindowWidth(aWindowWidth)
		, myWindowHeight(aWindowHeight)
	{
		CreateAppWindow(anInstance, aWndProc);
		CU::Input::InputWrapper::Create(myHWND, anInstance);
		myAPI = new CDirectX11(myHWND, aWindowWidth, aWindowHeight);

//		myFont = new Font("Data/Font/OpenSans-Light.ttf");

		std::stringstream windowText;
		windowText << "API : " << myAPI->GetAPIName() << " | " << "Adapter : " << myAPI->GetActiveAdapterName();
		SetWindowText(myHWND, windowText.str().c_str());
	}

	CEngine::~CEngine()
	{
		SAFE_DELETE(myAPI);
		CU::Input::InputWrapper::Destroy();
	}


	void CEngine::Create(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc)
	{
		assert(myInstance == nullptr && "Instance already created!");
		myInstance = new CEngine(aWindowWidth, aWindowHeight, anInstance, aWndProc);
	}

	void CEngine::Destroy()
	{
		assert(myInstance != nullptr && "Can't destroy the instance before it's created. Did you call Destroy twice?");
		SAFE_DELETE(myInstance);
	}

	CEngine* CEngine::GetInstance()
	{
		return myInstance;
	}

	void CEngine::Present()
	{
		myInstance->myAPI->Present();
	}

	void CEngine::Clear()
	{
		myInstance->myAPI->Clear();
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
		float width = (GetSystemMetrics(SM_CXSCREEN)* 0.5f) - (myWindowWidth * 0.5f);
		float height = (GetSystemMetrics(SM_CYSCREEN)* 0.5f) - (myWindowHeight * 0.5f);
		myHWND = CreateWindow(
			"WindowsClass",
			NULL, WS_OVERLAPPEDWINDOW, //Windowed
			static_cast<int>(width),
			static_cast<int>(height),
			static_cast<int>(myWindowWidth),
			static_cast<int>(myWindowHeight),
			NULL, NULL,
			GetModuleHandle(NULL), NULL);

		ShowWindow(myHWND, true);
	}
};