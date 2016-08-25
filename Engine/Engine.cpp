#include "stdafx.h"
#include "Synchronizer.h"
#include "Renderer.h"
#include "AssetsContainer.h"
#include "Terrain.h"
namespace Snowblind
{
	CEngine* CEngine::myInstance = nullptr;
	CDirectX11* CEngine::myAPI = nullptr;

	CEngine::CEngine(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc)
		: myWindowSize(aWindowWidth, aWindowHeight)
	{
		CreateAppWindow(anInstance, aWndProc);
		CU::Input::InputWrapper::Create(myHWND, anInstance);
	}


	CEngine::~CEngine()
	{
		SAFE_DELETE(myAssetsContainer);
		//SAFE_DELETE(myConsole);
		SAFE_DELETE(model);
		SAFE_DELETE(mySynchronizer);
		SAFE_DELETE(myRenderer);
		SAFE_DELETE(myCamera);
		SAFE_DELETE(myFontManager);
		SAFE_DELETE(myTimeManager);
		CU::Input::InputWrapper::Destroy(); //Remove
		Randomizer::Destroy();

		myAPI->CleanUp();
		SAFE_DELETE(myAPI);
	}


	void CEngine::Create(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc)
	{
		DL_ASSERT_EXP(myInstance == nullptr, "Instance already created!");
		myInstance = new CEngine(aWindowWidth, aWindowHeight, anInstance, aWndProc);
		myInstance->Initiate();
	}

	void CEngine::Destroy()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "Can't destroy the instance before it's created. Did you call Destroy twice?");
		SAFE_DELETE(myInstance);
	}

	CEngine* CEngine::GetInstance()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "Can't Get the instance before it's created. Did you call Destroy twice?");
		return myInstance;
	}

	CDirectX11* CEngine::GetDirectX()
	{
		return myAPI;
	}


	void CEngine::Initiate()
	{
		myAPI = new CDirectX11(myHWND, myWindowSize.myWidth, myWindowSize.myHeight);

		myCamera = new Snowblind::CCamera(myWindowSize.myWidth, myWindowSize.myHeight);
		my2DCamera = new Snowblind::CCamera(myWindowSize.myWidth, myWindowSize.myHeight, CU::Vector3f(0, 0, 0.f));
		SetWindowText(myHWND, "Snowblind Engine");

		myAssetsContainer = new CAssetsContainer();
		myAssetsContainer->Initiate();

		Randomizer::Create();


		myFontManager = new CFontManager();
		myFontManager->Initiate();

		mySynchronizer = new CSynchronizer();
		myRenderer = new CRenderer(*mySynchronizer, myCamera);
		myRenderer->Add2DCamera(my2DCamera);

		//myConsole = new CConsole();
		//myConsole->Initiate(my2DCamera);
		myTimeManager = new CU::TimeManager();

	}

	CCamera* CEngine::GetCamera()
	{
		return myCamera;
	}

	Snowblind::CCamera* CEngine::Get2DCamera()
	{
		return my2DCamera;
	}

	void CEngine::Update()
	{
		myInstance->myAssetsContainer->Update();
		myInstance->myTimeManager->Update();
		myInstance->myRenderer->Render();
	}

	void CEngine::Present()
	{
		if (myInstance->myUsingVSync)
			myInstance->myAPI->Present(1, 0);
		else
			myInstance->myAPI->Present(0, 0);
	}

	void CEngine::Clear()
	{
		myInstance->myAPI->Clear();
	}

	const Snowblind::SWindowSize& CEngine::GetWindowSize() const
	{
		return myWindowSize;
	}

	CFont* CEngine::LoadFont(const s8* aFilepath, u16 aFontWidth, u16 aBorderWidth)
	{
		return myFontManager->LoadFont(aFilepath, aFontWidth, aBorderWidth);
	}

	const float CEngine::GetDeltaTime()
	{
		return myTimeManager->GetDeltaTime();
	}

	const float CEngine::GetFPS()
	{
		return myTimeManager->GetFPS();
	}

	const float CEngine::GetFrameTime()
	{
		return myTimeManager->GetFrameTime();
	}

	const std::string& CEngine::GetAPIName()
	{
		return myAPI->GetAPIName();
	}

	Snowblind::CTexture* CEngine::GetTexture(const std::string& aFilePath)
	{
		return myAssetsContainer->GetTexture(aFilePath);
	}

	Snowblind::CEffect* CEngine::GetEffect(const std::string& aFilePath)
	{
		return myAssetsContainer->GetEffect(aFilePath);
	}

	Snowblind::CModel* CEngine::GetModel(const std::string& aFilePath)
	{
		return myAssetsContainer->GetModel(aFilePath);
	}

	const std::string& CEngine::LoadModel(const std::string& aFilePath, const std::string& effect)
	{
		return myAssetsContainer->LoadModel(aFilePath, effect);
	}

	void CEngine::ResetRenderTargetAndDepth()
	{
		myAPI->ResetRenderTargetAndDepth();
	}

	void CEngine::ToggleVsync()
	{
		myUsingVSync = !myUsingVSync;
	}

	void CEngine::OnAltEnter()
	{
		myAPI->OnAltEnter();
	}

	void CEngine::OnPause()
	{
		myTimeManager->Pause();
	}

	void CEngine::OnResume()
	{
		myTimeManager->Start();
	}

	void CEngine::OnExit()
	{
		mySynchronizer->Quit();
	}

	CSynchronizer* CEngine::GetSynchronizer()
	{
		return mySynchronizer;
	}

	const SLocalTime& CEngine::GetLocalTime()
	{
		/* This function should be a callback? When minute changes? How? */
		SYSTEMTIME time;
		::GetLocalTime(&time);
		myLocalTime.hour = time.wHour;
		myLocalTime.minute = time.wMinute;
		myLocalTime.second = time.wSecond;

		return myLocalTime;
	}

	Snowblind::CTerrain* CEngine::CreateTerrain(const std::string& aFile, const CU::Vector3f& position, const CU::Vector2f& aSize)
	{
		CTerrain* newTerrain = new CTerrain(aFile, position, aSize);
		myRenderer->AddTerrain(newTerrain);
		return newTerrain;
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