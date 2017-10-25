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

#include <CommonLib/HashString.h>
static float s_CamSpeed = 10.f;

void Game::InitState(StateStack* state_stack)
{
	m_StateStack = state_stack;
	m_Engine = Engine::GetInstance();
	Initiate("Data/Levels/level_01.level");
	m_Engine->GetInputHandle()->AddController(0);
}

static const char* camera_file = "camera_pos";

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
	//CameraHandle::GetInstance()->Initiate(&m_Orientation);
	CameraHandle::GetInstance()->Initiate(nullptr);
	m_PauseState.InitState(m_StateStack);
	component = &m_Engine->GetEntityManager().GetComponent<TranslationComponent>(0);


	bool read_camera = false;
	if (read_camera)
	{
		std::ifstream camera_load;
		std::string line;
		camera_load.open(camera_file);
		if (camera_load.is_open())
		{
			CU::Matrix44f init_orientation;
			int i = 0;
			while (getline(camera_load, line))
			{
				init_orientation[i] = stof(line);
				i++;
			}
			m_Camera->SetOrientation(init_orientation);
		}
	}

	spotlight.Add(m_Engine->RegisterLight());


	spotorient = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(90.f)) * spotorient;
	spotorient.SetPosition({ 5.f, 4.f, 5.f, 1.f });



#if !defined(_FINAL) && !defined(_PROFILE)
	debug::DebugHandle* pDebug = debug::DebugHandle::GetInstance();
	pDebug->RegisterFloatSlider(debug::DebugSlider<float>(0.f, 180.f, &degree, "Spotlight Degree"));
	pDebug->RegisterFloatSlider(debug::DebugSlider<float>(0.f, 180.f, &intensity, "Spotlight Intensity"));
	pDebug->RegisterFloatSlider(debug::DebugSlider<float>(0.f, 180.f, &range, "Spotlight Range"));
#endif
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

void Game::SaveCameraPosition()
{
	std::ofstream camera_save;
	camera_save.open(camera_file, std::ios::trunc);

	if (camera_save.is_open())
	{
		for (size_t i = 0; i < 16; i++)
		{
			camera_save << m_Camera->GetOrientation()[i];
			camera_save << "\n";
		}

		camera_save.flush();
		camera_save.close();
	}
}

void Game::Reload()
{
	m_Paused = true;
	m_World.CleanUp();
	m_Engine->GetEntityManager().Reset();
}

void Game::Update(float dt)
{
	CameraHandle::GetInstance()->Update();
	OldUpdate(dt);

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
	PostMaster* pEventHandle = PostMaster::GetInstance();
	if (input_wrapper->OnClick(MouseInput::LEFT))
	{
		CU::Vector3f ray_dir = m_Picker->GetCurrentRay(input_wrapper->GetCursorPos());
		pEventHandle->SendMessage(OnLeftClick(ray_dir.x, ray_dir.y, ray_dir.z, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z, m_Player));
	}

	if (input_wrapper->OnDown(KButton::W))
	{
		pEventHandle->SendMessage("w_down", nullptr);
	}

	if (input_wrapper->OnDown(KButton::E))
	{
		pEventHandle->SendMessage("e_down", nullptr);
	}

	if (input_wrapper->OnDown(KButton::R))
	{
		pEventHandle->SendMessage("r_down", nullptr);
	}




	if (input_wrapper->IsDown(KButton::LCTRL))
	{
		if (input_wrapper->OnDown(KButton::S))
		{
			SaveCameraPosition();
		}
		return;
	}

	if (input_wrapper->OnDown(KButton::ESCAPE))
	{
		m_StateStack->PopCurrentMainState();
	}

	if (input_wrapper->IsDown(MouseInput::RIGHT))
	{
		m_Camera->Update(m_Engine->GetInputHandle()->GetDeltaCursorPos());

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

	}

	static float entity_speed = 0.2f;

	if (input_wrapper->IsDown(KButton::H))
	{
		entity_speed += 1.f * dt;
	}
	if (input_wrapper->IsDown(KButton::G))
	{
		entity_speed -= 1.f * dt;
	}

	m_Synchronizer->AddRenderCommand(SpotlightCommand(
		spotlight.GetLast(),
		cl::DegreeToRad(degree * 0.5f),
		range,
		intensity,
		CU::Vector4f(1, 0, 0, 1),
		spotorient,
		false));


	HandleMovement(input_wrapper, entity_speed, dt);
	m_Synchronizer->AddRenderCommand(ParticleCommand(CU::Vector3f(5, 5, 5)));

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
