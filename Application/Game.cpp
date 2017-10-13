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

#include <PostMaster.h>
#include <OnLeftClick.h>
#include <StateStack.h>
#include <Math/Vector/Vector.h>
#include "../Physics/PhysicsManager.h"
#include "../EntitySystem/EntityManager.h"
#include <imgui.h>
#include "CameraHandle.h"
#include <Engine/Effect.h>
#include <Engine/Texture.h>
#include <Input/ControllerInput.h>

static std::string key = "Data/Model/sponza_pbr/sponza.fbx";
static std::string cube = "Data/Model/cube.fbx";
static std::string wall = "Data/Model/wall.fbx";
static u64 default_cube = Hash("default_cube");
#define KEY_USED default_cube

#include <CommonLib/HashString.h>
static float s_CamSpeed = 50.f;

void Game::InitState(StateStack* state_stack)
{
	m_StateStack = state_stack;
	m_Engine = Engine::GetInstance();
	/*
	#if !defined(_PROFILE) && !defined(_FINAL)
		m_Engine->AddFunction("Data/Levels/level_01.level", [&]() { Initiate("Data/Levels/level_01.level"); });
		m_Engine->AddFunction("Data/Levels/level_02.level", [&]() { Initiate("Data/Levels/level_02.level"); });
		m_Engine->AddFunction("Data/Levels/level_03.level", [&]() { Initiate("Data/Levels/level_03.level"); });
	#endif*/
	Initiate("Data/Levels/level_01.level");
	//m_Engine->LoadModel(key, "Shaders/debug_pbl.json", true);

	//m_Engine->LoadModel(key, "Shaders/deferred_base.json", true);
	//m_Engine->LoadModel(KEY_USED, "Shaders/debug_pbl.json", true);

// 	Effect* volume_shader = m_Engine->GetEffect("Shaders/volume.json");
// 	m_VolumeTexture = new Texture;
// 	m_VolumeTexture->Initiate3DTexture(128, 128, 128, TextureFormat(2), 18, "Volume Texture");
// 	m_VolumeTexture->Load("Data/Model/box_v3.dds");
// 	volume_shader->AddShaderResource(m_VolumeTexture, Effect::_3DTEX);

	light = m_Engine->RegisterLight();

	//m_MainCharacter = new Texture;
	//m_MainCharacter->Load("Data/Textures/main_character.dds");

	//m_MainCharacter = m_Engine->GetSprite("Data/Textures/particles/test_normal.dds");
	Engine::GetInstance()->LoadSprite("Data/Textures/main_character.dds");
	m_MainKey = Hash("Data/Textures/main_character.dds");


	m_Engine->GetInputHandle()->AddController(0);
	m_Position = { 1920.f / 2.f, 1080.f / 2.f };
	//m_Engine->RegisterFloatSider(&s_CamSpeed, "Camera Move Speed", 0.f, 100.f);
	//m_Engine->AddFunction("Reload", [&] { Reload(); });
}

void Game::Initiate(const std::string& level)
{
	m_Synchronizer = m_Engine->GetSynchronizer();

	m_World.Initiate(CU::Vector3f(256, 256, 256)); //Might be a v2 instead and a set y pos 

	CU::GrowingArray<TreeDweller*> dwellers = m_Engine->LoadLevel(level);
	m_World.AddDwellers(dwellers);

	m_Player = new TreeDweller;
	m_Player->Initiate(m_Engine->GetEntityManager().CreateEntity(), TreeDweller::eType::DYNAMIC);
	m_Engine->GetEntityManager().AddComponent<TranslationComponent>(m_Player->GetEntity());
	TranslationComponent& translation = m_Engine->GetEntityManager().GetComponent<TranslationComponent>(m_Player->GetEntity());
	m_Player->AddComponent(&translation, TreeDweller::TRANSLATION | TreeDweller::DEBUG);
	m_World.AddDweller(m_Player);

	m_Picker = new CMousePicker;

	//m_Engine->ToggleVsync(); //settings
	m_Camera = m_Engine->GetCamera();
	m_Camera->SetPosition(CU::Vector3f(0, 0, 0));
	CameraHandle::Create();
	CameraHandle::GetInstance()->Initiate(&m_Orientation);
	//CameraHandle::GetInstance()->Initiate(nullptr);
	m_PauseState.InitState(m_StateStack);
	component = &m_Engine->GetEntityManager().GetComponent<TranslationComponent>(0);

	//PostMaster::GetInstance()->Subscribe("hello_world", this);
	//PostMaster::GetInstance()->Subscribe("left_click", this);
}

void Game::EndState()
{
	m_World.CleanUp();
	CameraHandle::Destroy();
	SAFE_DELETE(m_Picker);
}

void Game::Render(bool render_through)
{
}

void Game::Reload()
{
	m_Paused = true;
	m_World.CleanUp();
	m_Engine->GetEntityManager().Reset();
	//Initiate("Data/Levels/level_03.json");
}

void Game::HandleEvent(u64 event, void*)
{
	if(event == Hash("hello_world"))
		event_happen = true;
}

void Game::Update(float dt)
{
	CameraHandle::GetInstance()->Update();
	//_2DGame(dt);
	OldUpdate(dt);

}

void Game::_2DGame(float dt)
{
	m_Synchronizer->AddRenderCommand(SpriteCommand(m_MainKey, m_Position));

	ControllerInput * controller = m_Engine->GetInputHandle()->GetController(0);

	const ControllerState& state = controller->GetState();
	const ControllerState& prev = controller->GetPrevState();
	if (state.m_Buttons & eA)
	{
		m_Position.y -= 100.f * dt;
	}



	float x_value = (float)state.m_ThumbLX;
	float y_value = -(float)state.m_ThumbLY;

	float magnitude = sqrt(x_value * x_value + y_value * y_value); //Do something to skip the sqrt?
	float normalized = 0.f;
	const float r_thumb_deadzone = 8689.f;

	if (magnitude > r_thumb_deadzone)
	{
		if (magnitude > SHRT_MAX)
			magnitude = SHRT_MAX;

		magnitude -= r_thumb_deadzone;

		normalized = magnitude / (SHRT_MAX - r_thumb_deadzone);

	}
	else
	{
		x_value = 0.f;
		y_value = 0.f;
	}


	if (normalized < -0.5f || normalized > 0.5f)
	{
		x_value /= 2.f;
		y_value /= 2.f;
	}

	m_Position.x += (x_value / 100.f * dt);
	m_Position.y += (y_value / 100.f * dt);


	m_Synchronizer->AddRenderCommand(TextCommandA(CU::Vector2f(0.5f, 0.5f), "x: %.3f\ny: %.3f", m_Position.x, m_Position.y));


}

void Game::OldUpdate(float dt)
{
	m_FrameCount++;
	m_AverageFPS += m_Engine->GetFPS();
	m_Time -= dt;
	if (m_Time <= 0.f)
	{
		m_FPSToPrint = u32(m_AverageFPS / m_FrameCount);
		m_FrameCount = 0;
		m_AverageFPS = 0.f;
		m_Time = 1.f;
	}

	InputWrapper* input_wrapper = m_Engine->GetInputHandle()->GetInputWrapper();
// 	if (input_wrapper->OnClick(MouseInput::LEFT))
// 	{
// 		CU::Vector3f ray_dir = m_Picker->GetCurrentRay(input_wrapper->GetCursorPos());
// 		PostMaster::GetInstance()->SendMessage(OnLeftClick(ray_dir.x, ray_dir.y, ray_dir.z, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z, m_Player));
// 	}


	//CU::Vector3f ray_dir = m_Picker->GetCurrentRay(input_wrapper->GetCursorPos());




	if (input_wrapper->OnDown(KButton::ESCAPE))
	{
		m_StateStack->PopCurrentMainState();
	}

	if (input_wrapper->IsDown(MouseInput::RIGHT))
	{
		m_Camera->Update(m_Engine->GetInputHandle()->GetDeltaCursorPos());
	}


	if (input_wrapper->IsDown(KButton::W))
		m_Camera->Move(eDirection::FORWARD, s_CamSpeed * dt);
	if (input_wrapper->IsDown(KButton::S))
		m_Camera->Move(eDirection::BACK, -s_CamSpeed * dt);
	if (input_wrapper->IsDown(KButton::A))
		m_Camera->Move(eDirection::LEFT, -s_CamSpeed * dt);
	if (input_wrapper->IsDown(KButton::D))
		m_Camera->Move(eDirection::RIGHT, s_CamSpeed * dt);
	if (input_wrapper->IsDown(KButton::SPACE))
		m_Camera->Move(eDirection::UP, s_CamSpeed * dt);
	if (input_wrapper->IsDown(KButton::X))
		m_Camera->Move(eDirection::DOWN, -s_CamSpeed * dt);


	static float entity_speed = 0.2f;

	if (input_wrapper->IsDown(KButton::H))
	{
		entity_speed += 1.f * dt;
	}
	if (input_wrapper->IsDown(KButton::G))
	{
		entity_speed -= 1.f * dt;
	}


	//CU::Vector3f pos = m_Camera->GetOrientation().GetPosition();
	//m_Synchronizer->AddRenderCommand(TextCommandA(CU::Vector2f(0.75f, 0.1f), "\nx:%.3f\ny:%.3f\nz:%.3f\n#%s(%d)", pos.x, pos.y, pos.z,
	//	((m_FPSToPrint >= 50.f) ? "00FF00" : (m_FPSToPrint < 25.f) ? "FF0000" : "FFFF00"), m_FPSToPrint));

	//if (event_happen)
	//	m_Synchronizer->AddRenderCommand(TextCommandA(CU::Vector2f(0.5f, 0.7f), "Hello World has been sent & recieved"));

	//AddRenderCommand(ModelCommand(KEY_USED, CU::Vector3f(5.f, 0.f, 5.f), false));

	//AddRenderCommand(ModelCommand(key, CU::Vector3f(256, 0, 256), false));
	/*CU::Matrix44f orientation;
	orientation.SetPosition(CU::Vector3f(50, 10, 50));
	orientation = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(90.f)) * orientation;
	m_Synchronizer->AddRenderCommand(SpotlightCommand(0, 53, 12, 1, CU::Vector4f(255, 0, 0, 255), orientation, false));*/


	HandleMovement(input_wrapper, entity_speed, dt);

	m_World.Update(dt, m_Paused);
}

void Game::HandleMovement(InputWrapper* input_wrapper, float entity_speed, float dt)
{
	CU::Vector4f translation = m_Orientation.GetTranslation();
	if (input_wrapper->IsDown(KButton::UP_ARROW))
	{
		CU::Math::Vector4<float> forward = m_Orientation.GetForward();
		translation += forward * entity_speed;
	}
	if (input_wrapper->IsDown(KButton::DOWN_ARROW))
	{
		CU::Math::Vector4<float> forward = m_Orientation.GetForward();
		translation += forward * -entity_speed;
	}

	if (input_wrapper->IsDown(KButton::LEFT_ARROW))
	{
		CU::Math::Vector4<float> forward = m_Orientation.GetRight();
		translation += forward * -entity_speed;
	}

	if (input_wrapper->IsDown(KButton::RIGHT_ARROW))
	{
		CU::Math::Vector4<float> forward = m_Orientation.GetRight();
		translation += forward * entity_speed;
	}

	if (input_wrapper->IsDown(KButton::M))
	{
		CU::Math::Vector4<float> forward = m_Orientation.GetUp();
		translation += forward * entity_speed;
	}

	if (input_wrapper->IsDown(KButton::N))
	{
		CU::Math::Vector4<float> forward = m_Orientation.GetUp();
		translation += forward * -entity_speed;
	}

	if (input_wrapper->IsDown(KButton::M))
	{
		CU::Math::Vector4<float> forward = m_Orientation.GetUp();
		translation += forward * entity_speed;
	}

	if (input_wrapper->IsDown(KButton::NUMPAD8))
	{
		m_Orientation.RotateAroundPointX(m_Orientation.GetPosition(), cl::DegreeToRad(90.f) * dt);
	}
	if (input_wrapper->IsDown(KButton::NUMPAD2))
	{
		m_Orientation.RotateAroundPointX(m_Orientation.GetPosition(), -cl::DegreeToRad(90.f) * dt);
	}
	if (input_wrapper->IsDown(KButton::NUMPAD4))
	{
		m_Orientation.RotateAroundPointY(m_Orientation.GetPosition(), -cl::DegreeToRad(90.f) * dt);
	}
	if (input_wrapper->IsDown(KButton::NUMPAD6))
	{
		m_Orientation.RotateAroundPointY(m_Orientation.GetPosition(), cl::DegreeToRad(90.f) * dt);
	}
	if (input_wrapper->IsDown(KButton::NUMPAD7))
	{
		m_Orientation.RotateAroundPointZ(m_Orientation.GetPosition(), -cl::DegreeToRad(90.f) * dt);
	}
	if (input_wrapper->IsDown(KButton::NUMPAD9))
	{
		m_Orientation.RotateAroundPointZ(m_Orientation.GetPosition(), cl::DegreeToRad(90.f) * dt);
	}


	m_Orientation.SetTranslation(translation);
}

void Game::AddRenderCommand(const ModelCommand& command)
{
	Engine& engine = Engine::GetRef();
	const u16 current_buffer = engine.GetSynchronizer()->GetCurrentBufferIndex();
	memory::CommandAllocator& allocator = engine.GetMemorySegmentHandle().GetCommandAllocator(current_buffer ^ 1, 0);
	void * current = allocator.Alloc(sizeof(ModelCommand));
	memcpy(current, &command, sizeof(ModelCommand));
}
