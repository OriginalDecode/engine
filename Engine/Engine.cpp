#include "Engine.h"
#if defined(_WIN32) || defined(_WIN64)
#include "DirectX11.h"
#include "DirectX12.h"
#include <Windows.h>
#endif
#include "OpenGL.h"
#include "Vulkan.h"
#include "EngineDefines.h"

#include <sstream>
#include <assert.h>
#include <TimeManager/TimeManager.h>
namespace Snowblind
{
	CEngine* CEngine::myInstance = nullptr;

	CEngine::CEngine(eAPIFlag anAPIFlag, float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc)
		: myWindowWidth(aWindowWidth)
		, myWindowHeight(aWindowHeight)
	{
		CreateAppWindow(anInstance, aWndProc);

		switch (anAPIFlag)
		{
#if defined(_WIN32) || defined(_WIN64)
		case eAPIFlag::DIRECTX11:
			myAPI = new CDirectX11(myHWND, aWindowWidth, aWindowHeight);
			break;
		case eAPIFlag::DIRECTX12:
			myAPI = new CDirectX12();
			break;
#endif
		case eAPIFlag::OPENGL:
			myAPI = new COpenGL();
			break;
		case eAPIFlag::VULKAN:
			myAPI = new CVulkan;
			break;
		};

		std::stringstream windowText(myAPI->GetAPIName());
		SetWindowText(myHWND, windowText.str().c_str());
	}

	CEngine::~CEngine()
	{
		SAFE_DELETE(myAPI);
	}

	void CEngine::Create(eAPIFlag anAPIFlag, float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc)
	{
		assert(myInstance == nullptr && "Instance already created!");
		myInstance = new CEngine(anAPIFlag, aWindowWidth, aWindowHeight, anInstance, aWndProc);
	}

	void CEngine::Destroy()
	{
		assert(myInstance != nullptr && "Can't destroy the instance before it's created. Did you call Destroy twice?");
		delete myInstance;
		myInstance = nullptr;
	}

	CEngine* CEngine::GetInstance()
	{
		return myInstance;
	}

	void CEngine::Present()
	{
		myAPI->Present();
	}

	void CEngine::Clear()
	{
		myAPI->Clear();
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