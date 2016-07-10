#include "stdafx.h"
#include "Synchronizer.h"
#include "Renderer.h"
#include "Console.h"
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

		myCamera = new Snowblind::CCamera(aWindowWidth, aWindowHeight);
		my2DCamera = new Snowblind::CCamera(aWindowWidth, aWindowHeight, CU::Vector3f(0, 0, 0.f));
		Randomizer::Create();
		SetWindowText(myHWND, "Snowblind Engine");

		myConsole = new CConsole();
	}

	CEngine::~CEngine()
	{
		SAFE_DELETE(myConsole);
		SAFE_DELETE(model);
		SAFE_DELETE(mySynchronizer);
		SAFE_DELETE(myRenderer);
		SAFE_DELETE(myCamera);
		SAFE_DELETE(myFontManager);
		SAFE_DELETE(myTimeManager);
		CU::Input::InputWrapper::Destroy();
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
		myTimeManager = new CU::TimeManager();
		myFontManager = new CFontManager();
		myFontManager->Initiate();

		mySynchronizer = new CSynchronizer();
		myRenderer = new CRenderer(*mySynchronizer, myCamera);
		myRenderer->Add2DCamera(my2DCamera);

		model = new Snowblind::CModel();
		model->CreateCube("Data/Shaders/T_Cube_Shader.json", CU::Vector3f(1, 1, 1));
		myRenderer->AddModel(model, "Cube");

		myRenderer->AddModel(Snowblind::CAssetsContainer::GetInstance()->GetModel("Data/Model/ls_engine_test/Radio_DDS.fbx", "Data/Shaders/T_Deferred_Base.json"), "Radio");
		myRenderer->AddModel(Snowblind::CAssetsContainer::GetInstance()->GetModel("Data/Model/pblScene/pblScene_03_binary.fbx", "Data/Shaders/T_Deferred_Base.json"), "PBL_Room");
		myConsole->Initiate(my2DCamera);

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
		CAssetsContainer::GetInstance()->Update();

		myInstance->myTimeManager->Update();
		myInstance->myRenderer->Render();

		if (CU::Input::InputWrapper::GetInstance()->KeyClick(DIK_GRAVE))
		{
			myInstance->myConsole->ToggleConsole();

		}


		if (myInstance->myConsole->GetIsActive())
			myInstance->mySynchronizer->AddRenderCommand(SRenderCommand("Console is Active", CU::Vector2f(100.f, 500.f)));
		else
			myInstance->mySynchronizer->AddRenderCommand(SRenderCommand("Console is Inactive", CU::Vector2f(100.f, 500.f)));
		myInstance->myConsole->Update();
		myInstance->myConsole->Render();
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

	CFont* CEngine::LoadFont(const char* aFilepath, short aFontWidth, int aBorderWidth)
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

	const std::string& CEngine::GetAPIName()
	{
		return myAPI->GetAPIName();
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