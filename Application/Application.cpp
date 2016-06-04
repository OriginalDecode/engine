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
#include <thread>
#include <Texture.h>
#include <EmitterInstance.h>
#include <DeferredRenderer.h>
#define ROTATION_SPEED  50.f / 180.f * float(PI)
#define MOVE_SPEED 50.f

CApplication::CApplication()
{
}

CApplication::~CApplication()
{
	myLogicThread->join();
	SAFE_DELETE(myLogicThread);
	SAFE_DELETE(my2DScene);
	SAFE_DELETE(myWorldScene);
	SAFE_DELETE(myConsole);
	SAFE_DELETE(myController);
	SAFE_DELETE(myInstance);
	SAFE_DELETE(myPointLight);
	SAFE_DELETE(myEmitter);
}

void CApplication::Initiate(float aWindowWidth, float aWindowHeight)
{
	aWindowWidth;
	aWindowHeight;
	myAverageFPS = 0;
	myQuitFlag = false;
	myEngine = Snowblind::CEngine::GetInstance();
	myCamera = myEngine->GetCamera();
	myCamera->AddOrientation(&myOrientation);

	myInstance = new Snowblind::CInstance();
	myInstance->Initiate("Data/Model/ls_engine_test/Radio_2.fbx", "Data/Shaders/DeferredBase.fx");

	myPointLight = new Snowblind::CPointLight();
	myPointLight->SetRange(10);

	mySynchronizer = myEngine->GetSynchronizer();

	myEmitter = new Snowblind::CEmitterInstance();
	myEmitter->Initiate(mySynchronizer);

	//Snowblind::CTexture* tex = new Snowblind::CTexture();
	//tex->CreateTexture("Data/Textures/colors.dds");


	float pos = 0.f;
	for (int i = 0; i < 3; i++)
	{
		pos += 5;
		myPositions.Add(CU::Vector3f(pos, 0, 0));
		myPositions.Add(CU::Vector3f(-pos, 0, 0));
		myPositions.Add(CU::Vector3f(0, 0, pos));
		myPositions.Add(CU::Vector3f(0, 0, -pos));

	}


	//Keep at the end of initiate...
	myLogicThread = new std::thread([&] {CApplication::Update(); });
}

void CApplication::Update()
{
	float time = 0.f;
	int frameCount = 0;
	while (mySynchronizer->HasQuit() == false)
	{
		float deltaTime = myEngine->GetDeltaTime();

		myEmitter->Update(deltaTime);

		myAverageFPS += myEngine->GetFPS();
		time -= deltaTime;
		frameCount++;
		if (time < 0.f)
		{
			time = 1.f;
			myAverageFPSToPrint = myAverageFPS / frameCount;
			myAverageFPS = 0;
			frameCount = 0;
		}

		CU::Input::InputWrapper::GetInstance()->Update();

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(ESCAPE))
		{
			myEngine->OnExit();
			myQuitFlag = true;
		}
		UpdateInput(deltaTime);

		std::stringstream ss;
		ss << "#000000{" << myEngine->GetFPS() << "\n" 
			<< myAverageFPSToPrint 
			<< "\nCamera Position : \nX : " 
			<< myOrientation.GetPosition().x 
			<< "\nY : "	<< myOrientation.GetPosition().y 
			<< "\nZ : " << myOrientation.GetPosition().z << "}";
		mySynchronizer->AddRenderCommand(SRenderCommand(ss.str(), CU::Math::Vector2<float>(0, 0)));


		Render();
		mySynchronizer->LogicIsDone();
		mySynchronizer->WaitForRender();
	}
	myQuitFlag = true;
}

void CApplication::Render()
{
	mySynchronizer->AddRenderCommand(SRenderCommand(myInstance, CU::Vector3f(0.f, 0.f, 0.f) , SRenderCommand::eType::MODEL));
	mySynchronizer->AddRenderCommand(SRenderCommand(myEmitter));

}

void CApplication::UpdateInput(float aDeltaTime)
{
	if (myWindowIsActive)
	{
		myCursorPosition.x += static_cast<float>(CU::Input::InputWrapper::GetInstance()->MouseDirectX()) * 0.01f;
		myCursorPosition.y += static_cast<float>(CU::Input::InputWrapper::GetInstance()->MouseDirectY()) * 0.01f;


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
	myWindowIsActive = false;
}

void CApplication::OnActive()
{
	CU::Input::InputWrapper::SetActiveWindow(true);
	myWindowIsActive = true;
}

void CApplication::OnExit()
{

}

bool CApplication::HasQuit()
{
	return myQuitFlag;
}
