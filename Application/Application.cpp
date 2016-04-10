#include "Application.h"
#include <Engine.h>
#include <Effect.h>
#include <Model.h>
#include <Scene.h>
#include <Camera.h>
#include <Instance.h>
#include <TimeManager.h>                                                                                                                                                                                                                                                                                                                                                                                   
#include <EngineDefines.h>
#include "EffectContainer.h"
#include "../Input/InputWrapper.h"

#define ROTATION_SPEED  50.f / 180.f * float(PI)
#define MOVE_SPEED 50.f
CApplication::CApplication()
{
	CU::TimeManager::Create();
}

CApplication::~CApplication()
{
	SAFE_DELETE(myWorldScene);
	SAFE_DELETE(myInstance);
	SAFE_DELETE(myModel);
	SAFE_DELETE(myTexturedModel);
	SAFE_DELETE(myCamera);
	CU::TimeManager::Destroy();
	Snowblind::CEffectContainer::Destroy();
	Snowblind::CEngine::Destroy();
}

void CApplication::OnPause()
{
	CU::TimeManager::GetInstance()->Pause();
}

void CApplication::OnResume()
{
	CU::TimeManager::GetInstance()->Start();
}

void CApplication::Initiate(float aWindowWidth, float aWindowHeight)
{
	myCamera = new Snowblind::CCamera(aWindowWidth, aWindowHeight, Vector3f(0.f, 0.f, 25.f));
	myWorldScene = new Snowblind::CScene();
	myWorldScene->Initiate(myCamera);

	myModel = new Snowblind::CModel(myCamera);
	myModel->CreateCube("Data/Shaders/Cube.fx", 1.f, 1.f, 1.f);
	
	//myTexturedModel = new Snowblind::CModel(myCamera);
	//myTexturedModel->CreateTexturedCube("Data/Shaders/TexturedCube.fx", 1.f, 1.f, 1.f);
	myInstance = new Snowblind::CInstance(myModel);
	myInstance->SetPosition({ 0.f, 0.f, 0.f });
	myWorldScene->AddToScene(myInstance);
}

bool CApplication::Update()
{
	CU::TimeManager::Update();
	CU::Input::InputWrapper::GetInstance()->Update();
	float deltaTime = CU::TimeManager::GetInstance()->GetDeltaTime();

	if (CU::Input::InputWrapper::GetInstance()->KeyDown(ESCAPE))
	{
		return false;
	}

	UpdateInput(deltaTime);
	myWorldScene->Update(deltaTime);
	/*for (int i = 0; i < myInstances.Size(); ++i)
	{
		myInstances[i]->Update(((90.f / 180.f)*float(PI)) * deltaTime);
		myInstances[i]->Render(*myCamera);
	}*/
	Render();
	return true;
}

void CApplication::Render()
{
	Snowblind::CEngine::Clear();
	myWorldScene->Render();
	Snowblind::CEngine::Present();
}

void CApplication::UpdateInput(float aDeltaTime)
{
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

	if (CU::Input::InputWrapper::GetInstance()->KeyDown(UP_ARROW))
	{
		myCamera->Rotate(Snowblind::eRotation::X_AXIS, -ROTATION_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(DOWN_ARROW))
	{
		myCamera->Rotate(Snowblind::eRotation::X_AXIS, ROTATION_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(LEFT_ARROW))
	{
		myCamera->Rotate(Snowblind::eRotation::Y_AXIS, -ROTATION_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(RIGHT_ARROW))
	{
		myCamera->Rotate(Snowblind::eRotation::Y_AXIS, ROTATION_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(Q))
	{
		myCamera->Rotate(Snowblind::eRotation::Z_AXIS, ROTATION_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(E))
	{
		myCamera->Rotate(Snowblind::eRotation::Z_AXIS, -ROTATION_SPEED * aDeltaTime);
	}
}
