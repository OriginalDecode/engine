#include "Game.h"

#include <EntityManager.h>

#include <Synchronizer.h>
#include <RenderCommand.h>
#include <Line3D.h>

#include <DL_Debug.h>
#include <Engine.h>
#include <Utilities.h>
#include <MousePicker.h>

#include <EngineDefines.h>

#include <Camera.h>
#include "../Input/InputHandle.h"

#include <RenderCommand_Shared.h>
#include <PhysicsManager.h>
#include <PostMaster.h>
#include <OnLeftClick.h>
bool Game::Initiate()
{
	myEngine = Hex::Engine::GetInstance();
	mySynchronizer = myEngine->GetSynchronizer();

	myEngine->LoadLevel("Data/Levels/level_01.json");

	myPicker = new Hex::CMousePicker;

	myEngine->ToggleVsync(); //settings
	m_Camera = myEngine->GetCamera();


	CU::Plane<float> plane0; plane0.InitWithPointAndNormal(CU::Vector3f(0.f, 0.f, 5.f), CU::Vector3f(0.f, 0.f, -1.f));
	CU::Plane<float> plane1; plane1.InitWithPointAndNormal(CU::Vector3f(0.f, 0.f, 10.f), CU::Vector3f(0.f, 0.f, 1.f));

	CU::Plane<float> plane2; plane2.InitWithPointAndNormal(CU::Vector3f(5.f, 0.f, 7.5f), CU::Vector3f(1.f, 0.f, 0.f));
	CU::Plane<float> plane3; plane3.InitWithPointAndNormal(CU::Vector3f(0.f, 0.f, 7.5f), CU::Vector3f(-1.f, 0.f, 0.f));

	CU::Plane<float> plane4; plane4.InitWithPointAndNormal(CU::Vector3f(0.f, -5.f, 7.5f), CU::Vector3f(0.f, -1.f, 0.f));
	CU::Plane<float> plane5; plane5.InitWithPointAndNormal(CU::Vector3f(0.f, 5.f, 7.5f), CU::Vector3f(0.f, 1.f, 0.f));

	m_PlaneVolume.AddPlane(plane0);
	m_PlaneVolume.AddPlane(plane1);
	m_PlaneVolume.AddPlane(plane2);
	m_PlaneVolume.AddPlane(plane3);
	m_PlaneVolume.AddPlane(plane4);
	m_PlaneVolume.AddPlane(plane5);

	m_ModelKey = myEngine->LoadModel("Data/Model/cube.fbx", "Data/Shaders/T_Cube.json");

	PostMaster::GetInstance()->Subscribe(eMessageType::COLLIDED, this);

	return true;
}

bool Game::CleanUp()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::COLLIDED, this);

	SAFE_DELETE(myPicker);
	if (myPicker)
		return false;

	myTerrain.RemoveAll();
	if (myTerrain.Size() > 0)
		return false;

	return true;
}

void Game::Update(float dt)
{
	InputWrapper* input_wrapper = myEngine->GetInputHandle()->GetInputWrapper();

	myFrameCount++;
	myAverageFPS += myEngine->GetFPS();
	myTime -= dt;
	if (myTime <= 0.f)
	{
		myFPSToPrint = static_cast<u32>(myAverageFPS / myFrameCount);
		myFrameCount = 0;
		myAverageFPS = 0.f;
		myTime = 1.f;
	}


	if (input_wrapper->IsDown(MouseInput::LEFT))
	{
		//pointHit = Hex::Engine::GetInstance()->GetPhysicsManager()->RayCast(m_Camera->GetPosition(), myPicker->GetCurrentRay(input_wrapper->GetCursorPos()));


		CU::Vector3f ray_dir = myPicker->GetCurrentRay(input_wrapper->GetCursorPos());
		//This is the shitties event system ever
		PostMaster::GetInstance()->SendMessage(OnLeftClick(ray_dir.x, ray_dir.y, ray_dir.z, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z));

		CU::Vector3f new_pos = m_Camera->GetPosition();
		for (int i = 0; i < 1000; i++)
		{
			m_collision = false;
			new_pos = m_Camera->GetPosition() + (ray_dir * float(i));

			if (m_PlaneVolume.Inside(new_pos))
			{
				m_collision = true;
				break;
			}
		}
	}

	SLinePoint p1, p2, p3, p4, p5, p6, p7, p8;
	CU::Vector3f min = { 0.f, -5.f, 5.f };
	CU::Vector3f max = { 5.f, 5.f, 10.f };

	float depth = max.z - min.z;
	float width = max.x - min.x;
	float height = max.y - min.y;
	p1.color = !m_collision ? CU::Vector4f(255.f, 255.f, 255.f, 255.f) : CU::Vector4f(255.f, 0.f, 0.f, 255.f);
	p2.color = p1.color;
	p3.color = p1.color;
	p4.color = p1.color;
	p5.color = p1.color;
	p6.color = p1.color;
	p7.color = p1.color;
	p8.color = p1.color;



	p1.position = min;
	p8.position = max;

	p2.position = p1.position;
	p2.position.x = p8.position.x;
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p1, p2));

	p3.position = p8.position;
	p3.position.y = p1.position.y;
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p2, p3));

	p4.position = p1.position;
	p4.position.z = p8.position.z;
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p3, p4));
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p4, p1));

	p5.position = p1.position;
	p5.position.y = p8.position.y;
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p1, p5));

	p6.position = p8.position;
	p6.position.x = p1.position.x;
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p5, p6));
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p6, p8));

	p7.position = p8.position;
	p7.position.z = p1.position.z;
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p8, p7));
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p7, p5));

	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p6, p4));
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p7, p2));
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p8, p3));

	mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, m_ModelKey, pointHit));

	//m_Camera->Update(myEngine->GetInputHandle()->GetDeltaCursorPos());
	if (input_wrapper->IsDown(KButton::W))
		m_Camera->Move(Hex::eDirection::FORWARD, 50.f * dt);
	if (input_wrapper->IsDown(KButton::S))
		m_Camera->Move(Hex::eDirection::BACK, -50.f * dt);
	if (input_wrapper->IsDown(KButton::A))
		m_Camera->Move(Hex::eDirection::LEFT, -50.f * dt);
	if (input_wrapper->IsDown(KButton::D))
		m_Camera->Move(Hex::eDirection::RIGHT, 50.f * dt);
	if (input_wrapper->IsDown(KButton::SPACE))
		m_Camera->Move(Hex::eDirection::UP, 50.f * dt);
	if (input_wrapper->IsDown(KButton::X))
		m_Camera->Move(Hex::eDirection::DOWN, -50.f * dt);
	if (input_wrapper->OnDown(KButton::Y))
		Hex::Engine::GetInstance()->ToggleWireframe();
	if (input_wrapper->IsDown(KButton::UP_ARROW))
		m_Camera->RotateAroundX(0.5f * dt);
	if (input_wrapper->IsDown(KButton::DOWN_ARROW))
		m_Camera->RotateAroundX(-0.5f * dt);



	mySynchronizer->AddRenderCommand(RenderCommand(eType::PARTICLE, CU::Vector3f(5.f, 5.f, 5.f)));

	CU::Vector2f cur_pos = myEngine->GetInputHandle()->GetDeltaCursorPos();
	std::stringstream ss;
	ss << "FPS : " << myEngine->GetFPS()
		<< "\nAverage FPS : " << myFPSToPrint
		<< "\nDeltaTime:" << dt
		<< "\n" << Hex::Engine::GetInstance()->GetLocalTimeAsString()
		<< "\ncursor_pos : \n" << cur_pos.x << "\n" << cur_pos.y
		<< "\nCollision : " << (m_collision ? "true" : "false");
	myEngine->AddDebugText(ss.str());



	myEngine->GetEntityManager()->Update(dt);
}

void Game::ReceiveMessage(const OnLeftClick& message)
{
	int apa;
	apa = 5;
}
