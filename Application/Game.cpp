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
#include "CameraHandle.h"
#include <Engine/Effect.h>
#include <Engine/Texture.h>
#include <Input/ControllerInput.h>
#include <Engine/LevelFactory.h>

#include <Engine/Renderer.h>
#include "Player.h"
#ifdef _DEBUG
#include "../include/hash/DebugEvents.h"
#endif
static float s_CamSpeed = 50.f;

//#define LOAD_LEVEL
#ifndef LOAD_LEVEL
//#define SUNTEMPEL
#ifdef SUNTEMPEL
u64 building = 0;
#else
u64 building = 0;
u64 pole = 0;
u64 curtain = 0;
#endif
#endif

void Game::InitState(StateStack* state_stack)
{
	CU::TimeManager timer;
	timer.Update();
	float t0 = timer.GetMasterTimer().GetTotalTime().GetMilliseconds();

	m_StateStack = state_stack;
	m_Engine = Engine::GetInstance();
	Initiate("Data/pbr_level/fps_level_01");
	//Initiate("Data/pbr_level/pbr_level.level");
	m_Engine->GetInputHandle()->AddController(0);

	timer.Update();
	float total = timer.GetMasterTimer().GetTotalTime().GetMilliseconds() - t0;
	std::stringstream ss;
	ss << "Time to initialize game " << total << " ms\n";
	OutputDebugStringA(ss.str().c_str());

}

static const char* camera_file = "camera_pos";

void Game::Initiate(const std::string& level)
{
	m_Synchronizer = m_Engine->GetSynchronizer();

	m_World.Initiate(CU::Vector3f(1024, 1024, 1024)); //Might be a v2 instead and a set y pos 

	//m_Player = new Player;
	//m_World.AddDweller(m_Player->Initiate());
	//LevelFactory::CreateTerrain("Data/Textures/terrain/britannia.tga");

#ifdef LOAD_LEVEL
	//CU::GrowingArray<TreeDweller*> dwellers = LevelFactory::CreatePBLLevel(8);
	//CU::GrowingArray<TreeDweller*> dwellers = LevelFactory::CreatePBLLevel(16, 1, 3, CU::Vector3f(-110.f, 0.f, -16.f), 15.f, 0.f, 15.f);
	//LevelFactory::CreateTerrain("Data/Textures/terrain/britannia.tga");
	CU::GrowingArray<TreeDweller*> dwellers = LevelFactory::LoadLevel(level.c_str());
	m_World.AddDwellers(dwellers);
#else

#ifdef SUNTEMPEL
	building = m_Engine->LoadModelA("Data/models/suntemple/suntemple.fbx", "Shaders/deferred_base.json", false);
#else
	curtain = m_Engine->LoadModelA("Data/model/sponza_pbr/curtain.fbx", "Shaders/deferred_base.json", false);
	building = m_Engine->LoadModelA("Data/model/sponza_pbr/building.fbx", "Shaders/deferred_base.json", false);
	pole = m_Engine->LoadModelA("Data/model/sponza_pbr/poles.fbx", "Shaders/deferred_base.json", false);
	CU::GrowingArray<TreeDweller*> dwellers = LevelFactory::CreatePBLLevel(8);
	//CU::GrowingArray<TreeDweller*> dwellers = LevelFactory::CreatePBLLevel(16, 1, 3, CU::Vector3f(-110.f, 0.f, -16.f), 15.f, 0.f, 15.f);



	m_World.AddDwellers(dwellers); 

#endif
#endif
	m_Picker = new CMousePicker;

	m_Camera = m_Engine->GetCamera();
	m_Camera->SetPosition(CU::Vector3f(512, 20, 512));
	m_Orientation.SetPosition(m_Camera->GetPosition());

	m_Camera->Update();
	CameraHandle::Create();

	CameraHandle::GetInstance()->Initiate(&m_Camera->GetPixelOrientation());// /* this should be the player, or a child matrix to the player (relative position with an offset that can rotate around the player object) */);
	//CameraHandle::GetInstance()->Initiate(&m_Orientation);
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
	CameraHandle::Destroy(); //This should be moved to a more logical place.
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

#include <engine/engine_shared.h>
static float _lifetime = 0.f;
static float _lifeTime2 = 0.f;
static int _pointCount = 0;

static bool second_curve = false;
static bool done = false;
static int _index = 0;
static bool skip = true;

CU::Vector3f _position;
std::vector<CU::Vector3f> _pointList;
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
	EventManager* pEventHandle = EventManager::GetInstance();
	if (input_wrapper->OnClick(MouseInput::LEFT))
		pEventHandle->SendMessage("pick_entity");

	if (input_wrapper->OnDown(KButton::ESCAPE))
		m_StateStack->PopCurrentMainState();

	static LinePoint p0, p1;
	p0.position = m_Camera->GetPosition();

//#ifdef _DEBUG
//	if (!input_wrapper->IsDown(KButton::LCTRL) && input_wrapper->OnClick(MouseInput::RIGHT))
//	{
//		const CU::Vector3f ray_dir = m_Picker->GetCurrentRay(input_wrapper->GetCursorPos());
//		CU::Vector3f intersection = m_Engine->GetPhysicsManager()->RayCast(m_Camera->GetPosition(), ray_dir, 1000.f);
//		p1.position = intersection;
//
//		pEventHandle->SendMessage(DebugEvents_OnRightClick, &intersection);
//	}
//	if (input_wrapper->IsDown(KButton::LCTRL) && input_wrapper->OnDown(KButton::C))
//		pEventHandle->SendMessage("copy_selected");
//
//	if (input_wrapper->IsDown(KButton::LCTRL) && input_wrapper->OnDown(KButton::V))
//		pEventHandle->SendMessage("paste_new");
//	s_CamSpeed = debug::DebugHandle::GetInstance()->m_CameraSpeed;
//#endif


	ControllerInput* input = m_Engine->GetInputHandle()->GetController(0);
	const ControllerState& input_state = input->GetState();
	m_Camera->Update(input->GetState());

	float x_value = (float)input_state.m_ThumbLX;
	float y_value = (float)input_state.m_ThumbLY;

	float magnitude = (x_value * x_value + y_value * y_value);
	float normalized = 0.f;
	const float r_thumb_deadzone = 8689.f;

	if (magnitude > r_thumb_deadzone * r_thumb_deadzone)
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

	if (x_value / SHRT_MAX > 0.15)
		m_Camera->Move(eDirection::RIGHT, s_CamSpeed * dt);

	if (x_value / SHRT_MAX < -0.15)
		m_Camera->Move(eDirection::LEFT, -s_CamSpeed * dt);

	if (y_value / SHRT_MAX > 0.15)
		m_Camera->Move(eDirection::FORWARD, s_CamSpeed * dt);

	if (y_value / SHRT_MAX < -0.15)
		m_Camera->Move(eDirection::BACK, -s_CamSpeed * dt);



	if (input_wrapper->IsDown(MouseInput::RIGHT))
	{
		m_Camera->Orient(m_Engine->GetInputHandle()->GetDeltaCursorPos());
	}

	

	float acceleration = s_CamSpeed * dt;

	if (input_wrapper->IsDown(KButton::W))
		m_Camera->Move(eDirection::FORWARD, acceleration);

	if (input_wrapper->IsDown(KButton::S))
		m_Camera->Move(eDirection::BACK, -acceleration);

	if (input_wrapper->IsDown(KButton::A))
		m_Camera->Move(eDirection::LEFT, -acceleration);

	if (input_wrapper->IsDown(KButton::D))
		m_Camera->Move(eDirection::RIGHT, acceleration);

	if (input_wrapper->IsDown(KButton::SPACE))
		m_Camera->Move(eDirection::UP, acceleration);

	if (input_wrapper->IsDown(KButton::X))
		m_Camera->Move(eDirection::DOWN, -acceleration);


#if !defined(_PROFILE) && !defined(_FINAL)
	if (input_wrapper->OnDown(KButton::P))
		m_Engine->GetRenderer()->CreateCubemaps();
#endif

	const CU::Vector4f forward = m_Orientation.GetForward();
	const CU::Vector4f right = m_Orientation.GetRight();
	const CU::Vector4f up = m_Orientation.GetUp();
	CU::Vector4f translation = m_Orientation.GetTranslation();

	static float speed = 10.f;
	if (input_wrapper->IsDown(KButton::NUMMINUS))
		speed -= 0.5f * dt;
	if (input_wrapper->IsDown(KButton::NUMADD))
		speed += 0.5f * dt;

	//m_Synchronizer->AddRenderCommand(TextCommandA(CU::Vector2f(0.5,0.5), "%.3f", speed));


	if (input_wrapper->IsDown(KButton::UP_ARROW))
		translation += forward * speed;

	if (input_wrapper->IsDown(KButton::DOWN_ARROW))
		translation += forward * -speed;

	if (input_wrapper->IsDown(KButton::RIGHT_ARROW))
		translation += right * speed;

	if (input_wrapper->IsDown(KButton::LEFT_ARROW))
		translation += right * -speed;

	if (input_wrapper->IsDown(KButton::PGDOWN))
		translation += up * -speed;

	if (input_wrapper->IsDown(KButton::PGUP))
		translation += up * speed;

	m_Orientation.SetTranslation(translation);


	if (input_wrapper->IsDown(KButton::NUMPAD6))
		m_Orientation.RotateAroundPointY(m_Orientation.GetPosition(), cl::DegreeToRad(90.f) * dt);

	if (input_wrapper->IsDown(KButton::NUMPAD4))
		m_Orientation.RotateAroundPointY(m_Orientation.GetPosition(), cl::DegreeToRad(-90.f) * dt);

	if (input_wrapper->IsDown(KButton::NUMPAD8))
		m_Orientation.RotateAroundPointX(m_Orientation.GetPosition(), cl::DegreeToRad(90.f) * dt);

	if (input_wrapper->IsDown(KButton::NUMPAD2))
		m_Orientation.RotateAroundPointX(m_Orientation.GetPosition(), cl::DegreeToRad(-90.f) * dt);

	if (input_wrapper->IsDown(KButton::NUMPAD9))
		m_Orientation.RotateAroundPointZ(m_Orientation.GetPosition(), cl::DegreeToRad(90.f) * dt);

	if (input_wrapper->IsDown(KButton::NUMPAD7))
		m_Orientation.RotateAroundPointZ(m_Orientation.GetPosition(), cl::DegreeToRad(-90.f) * dt);

	m_World.Update(dt, m_Paused); //This function takes a long time
}

void Game::AddRenderCommand(const ModelCommand& command)
{
	Engine& engine = Engine::GetRef();
	const u16 current_buffer = engine.GetSynchronizer()->GetCurrentBufferIndex();
	memory::CommandAllocator& allocator = engine.GetMemorySegmentHandle().GetCommandAllocator(current_buffer ^ 1, 0);
	void * current = allocator.Alloc(sizeof(ModelCommand));
	memcpy(current, &command, sizeof(ModelCommand));
}
