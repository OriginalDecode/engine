#include "Application.h"
#include <Engine.h>
#include <Effect.h>
#include <Model.h>
#include <Camera.h>
#include <Instance.h>
#include <TimeManager.h>
CApplication::CApplication()
{
	CU::TimeManager::Create();
}

CApplication::~CApplication()
{
	CU::TimeManager::Destroy();
	delete myEffect;
	myEffect = nullptr;
	delete myModel;
	myModel = nullptr;
	delete myCamera;
	myCamera = nullptr;

	Snowblind::CEngine::Destroy();
}

void CApplication::Initiate(float aWindowWidth, float aWindowHeight)
{
	myCamera = new Snowblind::CCamera(aWindowWidth, aWindowHeight, Vector3f(0.f, 0.f, 5.f));

	myEffect = new Snowblind::CEffect();
	myEffect->Initiate("Data/Shaders/Cube.fx");

	myModel = new Snowblind::CModel();
	myModel->CreateCube(myEffect, 1, 1, 1);

	myInstance = new Snowblind::CInstance(myModel);
}

bool CApplication::Update()
{
	CU::TimeManager::Update();
	float deltaTime = CU::TimeManager::GetInstance()->GetDeltaTime();
	Snowblind::CEngine::GetInstance()->Clear();
	
	myInstance->Update(1.f*deltaTime);
	myInstance->Render(*myCamera);

	Snowblind::CEngine::GetInstance()->Present();

	return true;
}

void CApplication::OnPause()
{
	CU::TimeManager::GetInstance()->Pause();
}

void CApplication::OnResume()
{
	CU::TimeManager::GetInstance()->Start();
}
