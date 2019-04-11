#include "Game.h"
#include "CameraHandle.h"
#include "Player.h"

#include <CommonLib/Utilities.h>
#include <CommonLib/Randomizer.h>
#include <CommonLib/XML/XMLReader.h>

#include <DL_Debug/DL_Debug.h>

#include <Engine/Camera.h>
#include <Engine/Effect.h>
#include <Engine/Engine.h>
#include <Engine/EngineDefines.h>
#include <Engine/LevelFactory.h>
#include <Engine/ModelImporterFlags.h>
#include <Engine/MousePicker.h>
#include <Engine/Renderer.h>
#include <Engine/RenderNodeGeneral.h>
#include <Engine/RenderNodeVegetation.h>
#include <Engine/RenderNodeShadows.h>
#include <Engine/StateStack.h>
#include <Engine/Synchronizer.h>
#include <Engine/TerrainSystem.h>
#include <Engine/Texture.h>

#include <EntitySystem/EntityManager.h>

#include <EventManager/EventManager.h>

#include <Input/InputManager.h>
#include <Input/InputHandle.h>
#include <Input/InputDeviceMouse_Win32.h>
#include <Input/ControllerInput.h>

#include <Math/Vector/Vector.h>

#include <Physics/PhysicsManager.h>
#include <Engine/AssetsContainer.h>

#ifdef _DEBUG
#include "../include/hash/DebugEvents.h"
#endif



static float s_CamSpeed = 50.f;

static const char* camera_file = "camera_pos";
void Game::InitState(StateStack* state_stack)
{
	CU::TimeManager timer;
	timer.Update();
	float t0 = timer.GetMasterTimer().GetTotalTime().GetMilliseconds();

	m_StateStack = state_stack;
	m_Engine = Engine::GetInstance();
	Initiate("Data/pbr_level/fps_level_01");
	m_Engine->GetInputHandle()->AddController(0);

	timer.Update();
	float total = timer.GetMasterTimer().GetTotalTime().GetMilliseconds() - t0;
	std::stringstream ss;
	ss << "Time to initialize game " << total << " ms\n";
	OutputDebugStringA(ss.str().c_str());

}


void Game::Initiate(const std::string& level)
{
	m_Synchronizer = m_Engine->GetSynchronizer();

	m_World.Initiate(CU::Vector3f(1024, 1024, 1024)); //Might be a v2 instead and a set y pos 
	// CU::GrowingArray<TreeDweller*> dwellers = LevelFactory::CreatePBLLevel(8);


	Engine* engine = Engine::GetInstance();
	Renderer* renderer = engine->GetRenderer();

	graphics::RenderNodeGeneral* general_node = nullptr;
	renderer->GetNode(&general_node);

	graphics::RenderNodeShadows* shadow_node = nullptr;
	renderer->GetNode(&shadow_node);

	graphics::RenderNodeVegetation* vegetation_node = nullptr;
	renderer->GetNode(&vegetation_node);


	const char* data = "Data/exported/Japanese_Maple_LowPoly.emf";
	HashType hash = AssetsContainer::GetInstance()->LoadModel<Model>(data, "Shaders/debug_pbl_instanced.json", false);
	int tree_count = cl::Rand(128, 128, 0);
	for (int i = 0; i < tree_count; ++i)
	{
		float x = cl::Rand(0.f, 2048.f, 0);
		float z = cl::Rand(0.f, 2048.f, 0);


		int normalized_x = (x / 2048.f) * 1024.f;
		int normalized_z = (z / 2048.f) * 1024.f;

		float y = Engine::GetInstance()->GetRenderer()->GetTerrainSystem()->GetHeight(normalized_x, normalized_z) / 255.f * 128.f;

		CU::Matrix44f orientation;
		orientation = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(-90.f));
		orientation *= CU::Matrix44f::CreateScaleMatrix({ 0.1f, 0.1f, 0.1f, 1.f });
		orientation.SetTranslation({ x , y , z  , 1.f });

		ModelInstance instance;

		instance.SetOrientation(orientation);
		instance.SetMaterialKey(cl::Hash("tree"));
		instance.SetModel(Engine::GetInstance()->GetModelDirect(hash.m_Lower));
		vegetation_node->AddInstance(instance);
		shadow_node->AddInstance(instance);

	}

	//HashType suntemple = AssetsContainer::GetInstance()->LoadModel<Model>("Data/exported/SunTemple.emf", "Shaders/debug_pbl_instanced.json", false);
	//ModelInstance instance;
	//instance.SetMaterialKey(cl::Hash("nothing"));
	//instance.SetModel(Engine::GetInstance()->GetModelDirect(suntemple.m_Lower));
	//pNode->AddInstance(instance);
	//shadow_node->AddInstance(instance);
		
		
	m_Picker = new CMousePicker;

	m_Camera = m_Engine->GetCamera();
	m_Camera->SetPosition(CU::Vector3f(0, 20, 0));
	m_Orientation.SetPosition(m_Camera->GetPosition());

	m_Camera->Update();
	CameraHandle::Create();

	CameraHandle::GetInstance()->Initiate(&m_Camera->GetPixelOrientation());// /* this should be the player, or a child matrix to the player (relative position with an offset that can rotate around the player object) */);
	//CameraHandle::GetInstance()->Initiate(&m_Orientation);
	m_PauseState.InitState(m_StateStack);
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

void Game::HandleEvent(uint64 event, void* data)
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
		for (int32 i = 0; i < 16; i++)
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
	/*m_FrameCount++;
	m_AverageFPS += m_Engine->GetFPS();
	m_Time -= dt;
	if (m_Time <= 0.f)
	{
		m_FPSToPrint = uint32(m_AverageFPS / m_FrameCount);
		m_FrameCount = 0;
		m_AverageFPS = 0.f;
		m_Time = 1.f;
	}*/
	Input::InputManager* input = Engine::GetInstance()->GetInputManager();

	static LinePoint p0, p1;
	p0.position = m_Camera->GetPosition();

	ControllerInput* controller = m_Engine->GetInputHandle()->GetController(0);
	const ControllerState& input_state = controller->GetState();
	m_Camera->Update(controller->GetState());

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



	if (input->IsDown(Input::A_ROTATE_CAMERA))
	{
		m_Camera->Orient({ input->GetCursor().dx, input->GetCursor().dy });
	}

	float mul = 1.f;

	if (input->IsDown(Input::A_ACCELERATE))
		mul = 100.f;

	float acceleration = (s_CamSpeed * dt) * mul;

	if (input->IsDown(Input::A_MOVE_FORWARD))
		m_Camera->Move(eDirection::FORWARD, acceleration);

	if (input->IsDown(Input::A_MOVE_BACK))
		m_Camera->Move(eDirection::BACK, -acceleration);

	if (input->IsDown(Input::A_MOVE_LEFT))
		m_Camera->Move(eDirection::LEFT, -acceleration);

	if (input->IsDown(Input::A_MOVE_RIGHT))
		m_Camera->Move(eDirection::RIGHT, acceleration);

	if (input->IsDown(Input::A_MOVE_UP))
		m_Camera->Move(eDirection::UP, acceleration);

	if (input->IsDown(Input::A_MOVE_DOWN))
		m_Camera->Move(eDirection::DOWN, -acceleration);

	m_World.Update(dt, m_Paused);
}
