#include "Application.h"
#include <Engine.h>
#include <Effect.h>
#include <Model.h>
#include <Scene.h>
#include <Camera.h>
#include <Instance.h>
#include <EngineDefines.h>
#include "../Input/InputWrapper.h"
#include <Text.h>
#include <Sprite.h>
#include <Console.h>
#include <FBXFactory.h>
#include <DirectionalLight.h>
#include <PointLight.h>
#include "../Input/ControllerInput.h"
#include <AssetsContainer.h>
#include <Synchronizer.h>
#include "RenderCommand.h"
#define ROTATION_SPEED  50.f / 180.f * float(PI)
#define MOVE_SPEED 50.f

CApplication::CApplication()
{
}

CApplication::~CApplication()
{
	SAFE_DELETE(my2DScene);
	SAFE_DELETE(myWorldScene);
	SAFE_DELETE(myConsole);
	SAFE_DELETE(myController);
	SAFE_DELETE(myInstance);
}

void CApplication::Initiate(float aWindowWidth, float aWindowHeight)
{

	my2DCamera = new Snowblind::CCamera(aWindowWidth, aWindowHeight, CU::Vector3f(0.f, 0.f, 1.f));

	myEngine = Snowblind::CEngine::GetInstance();
	myWorldScene = new Snowblind::CScene();
	myCamera = myEngine->GetCamera();
	myWorldScene->Initiate(myCamera);
	myCamera->AddOrientation(&myOrientation);

	my2DScene = new Snowblind::CScene();
	my2DScene->Initiate(my2DCamera, true);

	myText = new Snowblind::CText("Data/Font/OpenSans-Bold.ttf", 16, my2DCamera);
	myText->SetScale({ 1, 1 });
	myText->SetPosition({ 0, 0 });
	my2DScene->AddToScene(myText);

	myModel = new Snowblind::CModel();
	myModel->CreateCube("Data/Shaders/Cube.fx", 1.f, 1.f, 1.f);

	myTexturedModel = new Snowblind::CModel();
	myTexturedModel->CreateTexturedCube("Data/Shaders/TexturedCube.fx", 1.f, 1.f, 1.f);

	Snowblind::CAssetsContainer::GetInstance()->GetEffect("Data/Shaders/TexturedCube.fx")->SetAlbedo(myText->GetAtlas());

	//myInstance = new Snowblind::CInstance();
	//myInstance->Initiate(myModel);
	//myInstance->SetPosition({ 5.f, 0.f, 0.f });
	//myWorldScene->AddToScene(myInstance);

	//myInstance = new Snowblind::CInstance();
	//myInstance->Initiate(myTexturedModel);
	//myInstance->SetPosition({ 0.f, 5.f, 0.f });
	//myWorldScene->AddToScene(myInstance);

	myTextTime = new Snowblind::CText("Data/Font/OpenSans-Bold.ttf", 16, my2DCamera);
	myTextTime->SetPosition({ 0, 82 });
	my2DScene->AddToScene(myTextTime);

	myConsole = new Snowblind::CConsole();
	myConsole->Initiate(my2DCamera);

	//FBXFactory factory;
	//Snowblind::CModel* newModel = factory.LoadModel("Data/Model/pblScene/pblScene_03_binary.fbx", "Data/Shaders/PBL_Shader.fx");
	//newModel->CreateModel();
	myInstance = new Snowblind::CInstance();
	myInstance->Initiate(Snowblind::CAssetsContainer::GetInstance()->GetModel("Data/Model/pblScene/pblScene_03_binary.fbx"));
	//myWorldScene->AddToScene(myInstance);

	//Snowblind::CDirectionalLight* dlight = new Snowblind::CDirectionalLight();
	//dlight->Initiate({ -1, -1 ,0 }, { 0,0,0 }, { 1.f, 1.f, 0.f, 1.f });
	//myWorldScene->AddLight(dlight);

	Snowblind::CPointLight* light = new Snowblind::CPointLight();
	light->Initiate({ 0, -2, 0 }, { 1, 0, 0, 1 }, 10);
	myWorldScene->AddLight(light);

	light = new Snowblind::CPointLight();
	light->Initiate({ 5, -2, 0 }, { 0, 1, 0, 1 }, 10);
	myWorldScene->AddLight(light);

	light = new Snowblind::CPointLight();
	light->Initiate({ -5, -2, 0 }, { 0, 0, 1, 1 }, 10);
	myWorldScene->AddLight(light);

	light = new Snowblind::CPointLight();
	light->Initiate({ 0,-2, 10 }, { 1,0,1,1 }, 10);
	myWorldScene->AddLight(light);

	myController = new CU::ControllerInput(0);
	myConsole->SetWorldScene(myWorldScene);

	mySynchronizer = myEngine->GetSynchronizer();

}

bool CApplication::Update()
{
	CU::Input::InputWrapper::GetInstance()->Update();
	float deltaTime = myEngine->GetDeltaTime();

	if (CU::Input::InputWrapper::GetInstance()->KeyDown(ESCAPE))
	{
		myEngine->OnExit();
		return false;
	}


	if (CU::Input::InputWrapper::GetInstance()->KeyClick(DIK_GRAVE))
	{
		myConsole->ToggleConsole();
	}

	if (myConsole->GetIsActive())
		myConsole->Update();
	else
		UpdateInput(deltaTime);

	std::stringstream ss;
	ss << myEngine->GetFPS() << "\n" << "Camera Position : \nX : " << myOrientation.GetPosition().x << "\nY : " << myOrientation.GetPosition().y << "\nZ : " << myOrientation.GetPosition().z;
	myText->SetText(ss.str());


	myWorldScene->Update(deltaTime);
	Render();
	std::stringstream rText;
	rText << "Render Time : " << myText->GetRenderTime() << "ms\n" << "Update Time : " << myText->GetUpdateTime() << "ms";
	myTextTime->SetText(rText.str());

	mySynchronizer->LogicIsDone();
	mySynchronizer->WaitForRender();
	return true;
}

void CApplication::Render()
{
	//Snowblind::CEngine::Clear();
	//myWorldScene->Render();
	//my2DScene->Render();
	//myConsole->Render();
	//Snowblind::CEngine::Present();*/

	mySynchronizer->AddRenderCommand(SRenderCommand(myInstance, CU::Vector3f(0, 0, 0), SRenderCommand::eType::MODEL));

}

void CApplication::UpdateInput(float aDeltaTime)
{
	if (myController->IsConnected())
	{
		if (myController->RightThumbstickX() > 0.5f || myController->RightThumbstickX() < -0.5f)
		{
			myCursorPosition.x += myController->RightThumbstickX() * 0.005f;
		}
		if (myController->RightThumbstickY() > 0.5f || myController->RightThumbstickY() < -0.5f)
		{
			myCursorPosition.y -= myController->RightThumbstickY() * 0.005f;
		}


	}
	else
	{
		myCursorPosition.x += static_cast<float>(CU::Input::InputWrapper::GetInstance()->MouseDirectX()) * 0.01f;
		myCursorPosition.y += static_cast<float>(CU::Input::InputWrapper::GetInstance()->MouseDirectY()) * 0.01f;
	}

	myCursorPosition.y = fmaxf(fminf(3.1415f / 2.f, myCursorPosition.y), -3.1415f / 2.f);

	myPitch = CU::Quaternion(CU::Vector3f(1.f, 0, 0), myCursorPosition.y);
	myYaw = CU::Quaternion(CU::Vector3f(0, 1.f, 0), myCursorPosition.x);

	CU::Vector3f axisX(1.f, 0, 0);
	CU::Vector3f axisY(0, 1.f, 0);
	CU::Vector3f axisZ(0, 0, 1.f);

	axisX = myYaw * myPitch * axisX;
	axisY = myYaw * myPitch * axisY;
	axisZ = myYaw * myPitch * axisZ;

	myOrientation.myMatrix[0] = axisX.x;
	myOrientation.myMatrix[1] = axisX.y;
	myOrientation.myMatrix[2] = axisX.z;
	myOrientation.myMatrix[4] = axisY.x;
	myOrientation.myMatrix[5] = axisY.y;
	myOrientation.myMatrix[6] = axisY.z;
	myOrientation.myMatrix[8] = axisZ.x;
	myOrientation.myMatrix[9] = axisZ.y;
	myOrientation.myMatrix[10] = axisZ.z;

	if (CU::Input::InputWrapper::GetInstance()->KeyDown(W))
	{
		myCamera->Move(Snowblind::eDirection::FORWARD, MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(S))
	{
		myCamera->Move(Snowblind::eDirection::BACK, -MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(SPACE))
	{
		myCamera->Move(Snowblind::eDirection::UP, MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(X))
	{
		myCamera->Move(Snowblind::eDirection::DOWN, -MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(D))
	{
		myCamera->Move(Snowblind::eDirection::RIGHT, MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(A))
	{
		myCamera->Move(Snowblind::eDirection::LEFT, -MOVE_SPEED * aDeltaTime);
	}

	if (myController->IsConnected())
	{
		if (myController->LeftThumbstickY() > 0.5f)
		{
			myCamera->Move(Snowblind::eDirection::FORWARD, MOVE_SPEED * aDeltaTime);
		}
		if (myController->LeftThumbstickY() < -0.5f)
		{
			myCamera->Move(Snowblind::eDirection::BACK, -MOVE_SPEED * aDeltaTime);
		}
		if (myController->LeftThumbstickX() < -0.5f)
		{
			myCamera->Move(Snowblind::eDirection::LEFT, -MOVE_SPEED * aDeltaTime);
		}
		if (myController->LeftThumbstickX() > 0.5f)
		{
			myCamera->Move(Snowblind::eDirection::RIGHT, MOVE_SPEED * aDeltaTime);
		}
	}
}

void CApplication::OnPause()
{
	myEngine->OnPause();
}

void CApplication::OnResume()
{
	myEngine->OnResume();
}

void CApplication::OnInactive()
{
	CU::Input::InputWrapper::SetActiveWindow(false);
}

void CApplication::OnActive()
{
	CU::Input::InputWrapper::SetActiveWindow(true);
}

void CApplication::OnExit()
{

}
