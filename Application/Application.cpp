#include "Application.h"
#include <thread>

#include <Engine.h>
#include <Camera.h>

#include "../Input/InputWrapper.h"
#include <Sprite.h>

#include <FBXFactory.h>

#include <PointLight.h>

#include <Synchronizer.h>
#include <RenderCommand.h>
#include <EmitterInstance.h>
#include <DeferredRenderer.h>
#include <SystemMonitor.h>
#include <EngineDefines.h>

#define ROTATION_SPEED  50.f / 180.f * float(PI)
#define MOVE_SPEED 50.f

CApplication::CApplication()
{
}

CApplication::~CApplication()
{
	myLogicThread->join();
	SAFE_DELETE(myLogicThread);
	SAFE_DELETE(myPointLight);
	SAFE_DELETE(myEmitter);
}

void CApplication::Initiate(float aWindowWidth, float aWindowHeight)
{
	myWindowWidth = aWindowWidth;
	myWindowHeight = aWindowHeight;
	myAverageFPS = 0;
	myQuitFlag = false;
	myEngine = Snowblind::CEngine::GetInstance();
	myCamera = myEngine->GetCamera();
	myCamera->AddOrientation(&myOrientation);

	myPointLight = new Snowblind::CPointLight();
	myPointLight->SetRange(10);

	mySynchronizer = myEngine->GetSynchronizer();

	myEmitter = new Snowblind::CEmitterInstance();
	myEmitter->Initiate(mySynchronizer);

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
		UpdateInput(deltaTime);
		Render();
		mySynchronizer->LogicIsDone();
		mySynchronizer->WaitForRender();
	}
	myQuitFlag = true;
}

void CApplication::Render()
{
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::SKYSPHERE, myOrientation.GetPosition()));
	//mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::MODEL, "PBL_Room", CU::Vector3f(0.f, 0.f, 0.f)));
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::SPRITE, "colors", CU::Vector2f(0.f, 0.f)));
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::POINTLIGHT, CU::Vector3f(0.f, 0.f, 0.f), CU::Vector3f(1.f, 0.f, 0.f), 40.f, 10.f));
	//mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::PARTICLE, myEmitter));
	
	std::stringstream ss;
	ss << myEngine->GetFPS() << "\n"
		<< myAverageFPSToPrint
		<< "\nCamera Position : \nX : "
		<< myOrientation.GetPosition().x
		<< "\nY : " << myOrientation.GetPosition().y
		<< "\nZ : " << myOrientation.GetPosition().z;
	mySynchronizer->AddRenderCommand(SRenderCommand(ss.str(), CU::Math::Vector2<float>(0, 0)));
	
	std::stringstream cpuAndMem;
	cpuAndMem << "CPU: " << Snowblind::CSystemMonitor::GetCPUUsage() << "%" << "\n" << "Mem: " << Snowblind::CSystemMonitor::GetMemoryUsage(true) << " kb";
	mySynchronizer->AddRenderCommand(SRenderCommand(cpuAndMem.str(), CU::Vector2f(myWindowWidth - 100.f, 0)));

}

void CApplication::UpdateInput(float aDeltaTime)
{
	if (myWindowIsActive)
	{
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(ESCAPE))
		{
			myEngine->OnExit();
			myQuitFlag = true;
		}

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

void CApplication::OnAltEnter()
{
	if (this)
	{
		myEngine->OnAltEnter();
	}
}

bool CApplication::HasQuit()
{
	return myQuitFlag;
}
