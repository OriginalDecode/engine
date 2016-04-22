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
#include <Text.h>
#include <Sprite.h>
#define ROTATION_SPEED  50.f / 180.f * float(PI)
#define MOVE_SPEED 50.f
CApplication::CApplication()
{
}

CApplication::~CApplication()
{
	SAFE_DELETE(myModel);
	SAFE_DELETE(myTexturedModel);
	SAFE_DELETE(my2DScene);
	SAFE_DELETE(myWorldScene);

	CU::TimeManager::Destroy();
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

void CApplication::OnInactive()
{
	CU::Input::InputWrapper::SetActiveWindow(false);
}

void CApplication::OnActive()
{
	CU::Input::InputWrapper::SetActiveWindow(true);
}

void CApplication::Initiate(float aWindowWidth, float aWindowHeight)
{
	CU::TimeManager::Create();

	myCamera = new Snowblind::CCamera(aWindowWidth, aWindowHeight, Vector3f(0.f, 0.f, 25.f));
	my2DCamera = new Snowblind::CCamera(aWindowWidth, aWindowHeight, Vector3f(0.f, 0.f, 1.f));

	myWorldScene = new Snowblind::CScene();
	myWorldScene->Initiate(myCamera);

	my2DScene = new Snowblind::CScene();
	my2DScene->Initiate(my2DCamera, true);


	myText = new Snowblind::CText("Data/Font/OpenSans-Bold.ttf", 16, my2DCamera);
	//myText->SetText("!ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz");
	myText->SetText("Hello World, my name is Linus Skold and I am a programmer.");
	myText->SetScale({ 1, 1});
	myText->SetPosition({ 0, 0});
	my2DScene->AddToScene(myText);

	 
	myText2 = new Snowblind::CText("Data/Font/OpenSans-Bold.ttf", 64, my2DCamera);
	myText2->SetText("#ffff00{Hello} #ac00b2{World}, #8bb272{my} #b2007f{name} #2068ff{is} #ff0000{Linus Skold} #ff8803{and} #007fb2{I} am a #45ff24{programmer}.");
	myText2->SetScale({ 1, 1 });
	myText2->SetPosition({ 0, 150 });
	my2DScene->AddToScene(myText2);


	myModel = new Snowblind::CModel(myCamera);
	myModel->CreateCube("Data/Shaders/Cube.fx", 1.f, 1.f, 1.f);

	myTexturedModel = new Snowblind::CModel(myCamera);
	myTexturedModel->CreateTexturedCube("Data/Shaders/TexturedCube.fx", 1.f, 1.f, 1.f);

	Snowblind::CEffectContainer::GetInstance()->GetEffect("Data/Shaders/TexturedCube.fx")->SetAlbedo(myText->GetAtlas());

	myInstance = new Snowblind::CInstance();
	myInstance->Initiate(myModel);
	myInstance->SetPosition({ 0.f, 0.f, 0.f });
	myWorldScene->AddToScene(myInstance);

	myInstance = new Snowblind::CInstance();
	myInstance->Initiate(myTexturedModel);
	myInstance->SetPosition({ 0.f,5.f,0.f });
	myWorldScene->AddToScene(myInstance);

	mySprite = new Snowblind::CSprite();
	mySprite->Initiate(myText->GetAtlas(), { 256,256}, { 0.f,0.f });
	mySprite->SetPosition({ 128, 128});
	mySprite->SetScale({ 1.f, 1.f });
	my2DScene->AddToScene(mySprite);
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

	std::stringstream ss;
	ss << CU::TimeManager::GetInstance()->GetFPS();
	myText->SetText(ss.str());

	UpdateInput(deltaTime);
	myWorldScene->Update(deltaTime);
	Render();
	return true;
}

void CApplication::Render()
{
	Snowblind::CEngine::Clear();
	myWorldScene->Render();
	my2DScene->Render();
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
