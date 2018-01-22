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

#include <EventManager.h>
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
#include <Engine/LevelFactory.h>

#include "Player.h"
#ifdef _DEBUG
#include "../include/hash/DebugEvents.h"
#endif
static float s_CamSpeed = 50.f;

void Game::InitState(StateStack* state_stack)
{
	m_StateStack = state_stack;
	m_Engine = Engine::GetInstance();
	//Initiate("Data/Levels/level_01.level");
	Initiate("Data/pbr_level/pbr_level.level");
	m_Engine->GetInputHandle()->AddController(0);
}

static const char* camera_file = "camera_pos";

void Game::Initiate(const std::string& level)
{
	m_Synchronizer = m_Engine->GetSynchronizer();

	m_World.Initiate(CU::Vector3f(1024, 1024, 1024)); //Might be a v2 instead and a set y pos 

	//m_Player = new Player;
	//m_World.AddDweller(m_Player->Initiate());

	CU::GrowingArray<TreeDweller*> dwellers = LevelFactory::LoadLevel(level.c_str());
	m_World.AddDwellers(dwellers);


	m_Picker = new CMousePicker;

	m_Camera = m_Engine->GetCamera();
	m_Camera->SetPosition(CU::Vector3f(0, 10, 0));
	//m_Camera->RotateAroundY(cl::DegreeToRad(45.f));
	//m_Camera->RotateAroundX(cl::DegreeToRad(20.f));
	m_Camera->Update(CU::Vector2f(0.f,0.f));
	CameraHandle::Create();
	CameraHandle::GetInstance()->Initiate(nullptr /* this should be the player, or a child matrix to the player (relative position with an offset that can rotate around the player object) */);
	m_PauseState.InitState(m_StateStack);

#ifdef _DEBUG
	EventManager::GetInstance()->Subscribe(DebugEvents_AddEntity, this);
#endif
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
}

void Game::EndState()
{
	m_World.CleanUp();
	CameraHandle::Destroy(); //this probably should not be deleted here if we want to be able to render things in a menu that isn't the game state.
	SAFE_DELETE(m_Picker);
	SAFE_DELETE(m_Player);
}

void Game::Render(bool render_through)
{
}

void Game::HandleEvent(u64 event, void* data)
{
#ifdef _DEBUG
	if (event == DebugEvents_AddEntity)
	{
		TreeDweller* dweller = static_cast<TreeDweller*>(data);
		m_World.AddDweller(dweller);
	}
#endif
}

void Game::SaveCameraPosition()
{
	std::ofstream camera_save;
	camera_save.open(camera_file, std::ios::trunc);

	if (camera_save.is_open())
	{
		for (s32 i = 0; i < 16; i++)
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
	//m_Player->Update(dt);
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
	EventManager* pEventHandle = EventManager::GetInstance();
	if (input_wrapper->OnClick(MouseInput::LEFT))
		pEventHandle->SendMessage("pick_entity");

	if (input_wrapper->OnDown(KButton::ESCAPE))
	{
		m_StateStack->PopCurrentMainState();
	}

	static LinePoint p0, p1;
	p0.position = m_Camera->GetPosition();

#ifdef _DEBUG
	if (!input_wrapper->IsDown(KButton::LCTRL) && input_wrapper->OnClick(MouseInput::RIGHT))
	{
		const CU::Vector3f ray_dir = m_Picker->GetCurrentRay(input_wrapper->GetCursorPos());
		CU::Vector3f intersection = m_Engine->GetPhysicsManager()->RayCast(m_Camera->GetPosition(), ray_dir, 1000.f);
		p1.position = intersection;

		pEventHandle->SendMessage(DebugEvents_OnRightClick, &intersection);
	}
	if (input_wrapper->IsDown(KButton::LCTRL) && input_wrapper->OnDown(KButton::C))
		pEventHandle->SendMessage("copy_selected");

	if (input_wrapper->IsDown(KButton::LCTRL) && input_wrapper->OnDown(KButton::V))
		pEventHandle->SendMessage("paste_new");
#endif


	if (input_wrapper->IsDown(KButton::LCTRL) && input_wrapper->IsDown(MouseInput::RIGHT))
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

	HandleMovement(input_wrapper, entity_speed, dt);

	m_Synchronizer->AddRenderCommand(TextCommandA(CU::Vector2f(1920/2, 1080/2), "Sup nerd"));
	//m_Synchronizer->AddRenderCommand(LineCommand(p0, p1, false));
	//m_Synchronizer->AddRenderCommand(ParticleCommand(CU::Vector3f(5, 5, 5)));
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
