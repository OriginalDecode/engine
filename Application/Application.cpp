#include "Application.h"
#include <Engine.h>
#include <Effect.h>
#include <Model.h>
#include <Camera.h>
#include <Instance.h>
#include <TimeManager.h>                                                                                                                                                                                                                                                                                                                                                                                   
#include <EngineDefines.h>
#include "EffectContainer.h"
#include "../Input/InputWrapper.h"

#define MOVE_SPEED 50.f

CApplication::CApplication()
{
	CU::TimeManager::Create();
}

CApplication::~CApplication()
{
	myInstances.DeleteAll();
	CU::TimeManager::Destroy();
	SAFE_DELETE(myTexturedModel);
	SAFE_DELETE(myModel);
	SAFE_DELETE(myCamera);
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

	myModel = new Snowblind::CModel(myCamera);
	myTexturedModel = new Snowblind::CModel(myCamera);


	
	myModel->CreateCube("Data/Shaders/Cube.fx", 1.f, 1.f, 1.f);
	myTexturedModel->CreateTexturedCube("Data/Shaders/TexturedCube.fx", 1.f, 1.f, 1.f);


	Snowblind::CInstance* temp;

	temp = new Snowblind::CInstance(myModel);
	temp->SetPosition({ 0.f, 5.f, 0.f });
	myInstances.Add(temp);

	temp = new Snowblind::CInstance(myTexturedModel);
	temp->SetPosition({ 0.f, 0.f, 0.f });
	myInstances.Add(temp);
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

	Snowblind::CEngine::Clear();
	for (int i = 0; i < myInstances.Size(); ++i)
	{
		//myInstances[i]->Update(90.f * deltaTime);
		myInstances[i]->Render(*myCamera);
	}
	Snowblind::CEngine::Present();
	return true;
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
		myCamera->Rotate(Snowblind::eRotation::X_AXIS, -MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(DOWN_ARROW))
	{
		myCamera->Rotate(Snowblind::eRotation::X_AXIS, MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(LEFT_ARROW))
	{
		myCamera->Rotate(Snowblind::eRotation::Y_AXIS, -MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(RIGHT_ARROW))
	{
		myCamera->Rotate(Snowblind::eRotation::Y_AXIS, MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(Q))
	{
		myCamera->Rotate(Snowblind::eRotation::Z_AXIS, MOVE_SPEED * aDeltaTime);
	}
	if (CU::Input::InputWrapper::GetInstance()->KeyDown(E))
	{
		myCamera->Rotate(Snowblind::eRotation::Z_AXIS, -MOVE_SPEED * aDeltaTime);
	}



}
