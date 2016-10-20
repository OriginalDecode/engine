#include "Application.h"
#include <thread>

#include <Engine.h>
#include <Camera.h>

#include "../Input/InputWrapper.h"
#include <Sprite.h>

#include <PointLight.h>

#include <Synchronizer.h>
#include <RenderCommand.h>
#include <SystemMonitor.h>
#include <EngineDefines.h>
#include <sstream>

#include "Game.h"

#define ROTATION_SPEED  50.f / 180.f * float(PI)

CApplication::CApplication()
{
}

CApplication::~CApplication()
{
	myLogicThread->join();
	SAFE_DELETE(myLogicThread);
	SAFE_DELETE(myGame);
}

void CApplication::Initiate(float aWindowWidth, float aWindowHeight)
{
	aWindowHeight;
	aWindowWidth;
	myEngine = Snowblind::CEngine::GetInstance();
	myCamera = myEngine->GetCamera();
	myCamera->AddOrientation(&myOrientation);

	mySynchronizer = myEngine->GetSynchronizer();
	myGame = new CGame(mySynchronizer);

	//Keep at the end of initiate...
	myLogicThread = new std::thread([&] { CApplication::Update(); });
}

void CApplication::Update()
{
	while (mySynchronizer->HasQuit() == false)
	{
		
		float deltaTime = 0.f;
		myEngine->GetDeltaTime(deltaTime);
		UpdateInput(deltaTime);
		std::stringstream ss;
		ss << "X : " << myOrientation.GetPosition().x << "\n" <<
		"Y : " << myOrientation.GetPosition().y << "\n" <<
		"Z : " << myOrientation.GetPosition().z << "\n";
		mySynchronizer->AddRenderCommand(RenderCommand(ss.str(), CU::Vector2f(500, 0)));
		mySynchronizer->AddRenderCommand(RenderCommand(eType::SKYSPHERE, myOrientation.GetPosition()));

		myGame->Update(deltaTime);

		mySynchronizer->LogicIsDone();
		mySynchronizer->WaitForRender();
	}
	myQuitFlag = true;
}

void CApplication::UpdateInput(float aDeltaTime)
{
	CU::Input::InputWrapper::GetInstance()->Update();
	if (myWindowIsActive)
	{
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(ESCAPE))
		{
			myEngine->OnExit();
			myQuitFlag = true;
		}

		//myCursorPosition.x += static_cast<float>(CU::Input::InputWrapper::GetInstance()->MouseDirectX()) * 0.01f;

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(UP_ARROW))
		{
			myCursorPosition.y += 0.01f;
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(DOWN_ARROW))
		{
			myCursorPosition.y -= 0.01f;
		}

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(RIGHT_ARROW))
		{
			myCursorPosition.x += 0.01f;
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(LEFT_ARROW))
		{
			myCursorPosition.x -= 0.01f;
		}

		//myCursorPosition.y += static_cast<float>(CU::Input::InputWrapper::GetInstance()->MouseDirectY()) * 0.01f;


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

		float multiplier = 1.f;

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(LSHIFT))
		{
			multiplier = 100.f;
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(W))
		{
			myCamera->Move(Snowblind::eDirection::FORWARD, moveSpeed * aDeltaTime);
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(S))
		{
			myCamera->Move(Snowblind::eDirection::BACK, -moveSpeed * aDeltaTime);
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(SPACE))
		{
			myCamera->Move(Snowblind::eDirection::UP, moveSpeed * aDeltaTime);
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(X))
		{
			myCamera->Move(Snowblind::eDirection::DOWN, -moveSpeed * aDeltaTime);
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(D))
		{
			myCamera->Move(Snowblind::eDirection::RIGHT, moveSpeed * aDeltaTime);
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(A))
		{
			myCamera->Move(Snowblind::eDirection::LEFT, -moveSpeed * aDeltaTime);
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(UP_ARROW))
		{
			moveSpeed += 0.01f * multiplier;
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(DOWN_ARROW))
		{
			moveSpeed -= 0.01f * multiplier;
		}
	}
}

void CApplication::RandomWork()
{
	float delta_time;
	Snowblind::CEngine::GetInstance()->GetDeltaTime(delta_time);
}

void CApplication::OtherRandomWork()
{
	RandomWork();
	RandomWork();
	RandomWork();
	RandomWork();
	RandomWork();
	RandomWork();
	RandomWork();
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
	myEngine->OnAltEnter();
}

bool CApplication::HasQuit()
{
	return myQuitFlag;
}
