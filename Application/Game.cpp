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
	//Initiate("Data/pbr_level/pbr_level.level");
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


	graphics::IRenderNode* pNode = Engine::GetInstance()->GetRenderer()->GetNode(graphics::RenderNodeGeneral::GetType());
	graphics::IRenderNode* vegetation_node = Engine::GetInstance()->GetRenderer()->GetNode(graphics::RenderNodeVegetation::GetType());

	m_Engine->LoadModelA("Data/Model/trees/Japanese Maple/LowPoly/Japanese_Maple_LowPoly.fbx", "Shaders/debug_pbl_instanced.json", false);

	HashType hash = m_Engine->LoadModelA("Data/exported/Japanese_Maple_lowpoly.EMF", "Shaders/debug_pbl_instanced.json", false);
	int tree_count = cl::Rand(2048, 2048, 0);
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

	}


	m_Picker = new CMousePicker;

	m_Camera = m_Engine->GetCamera();
	m_Camera->SetPosition(CU::Vector3f(0, 20, 0));
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
	//m_FrameCount++;
	//m_AverageFPS += m_Engine->GetFPS();
	//m_Time -= dt;
	//if (m_Time <= 0.f)
	//{
	//	m_FPSToPrint = uint32(m_AverageFPS / m_FrameCount);
	//	m_FrameCount = 0;
	//	m_AverageFPS = 0.f;
	//	m_Time = 1.f;
	//}
	Input::InputManager* input = Engine::GetInstance()->GetInputManager();
	//InputWrapper* input_wrapper = m_Engine->GetInputHandle()->GetInputWrapper();
	//EventManager* pEventHandle = EventManager::GetInstance();
	//if (input_wrapper->OnClick(MouseInput::LEFT))
	//	pEventHandle->SendMessage("left_click");

	//if (input_wrapper->OnDown(KButton::ESCAPE))
	//	m_StateStack->PopCurrentMainState();

	//if (input_wrapper->OnDown(KButton::NUMADD))
	//	Engine::GetInstance()->GetRenderer()->font_scale += 0.25f;

	//if (input_wrapper->OnDown(KButton::NUMMINUS))
	//	Engine::GetInstance()->GetRenderer()->font_scale -= 0.25f;

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


//#if !defined(_PROFILE) && !defined(_FINAL)
//	if (input_wrapper->OnDown(KButton::P))
//		m_Engine->GetRenderer()->CreateCubemaps();
//#endif

	const CU::Vector4f forward = m_Orientation.GetForward();
	const CU::Vector4f right = m_Orientation.GetRight();
	const CU::Vector4f up = m_Orientation.GetUp();
	CU::Vector4f translation = m_Orientation.GetTranslation();

	//static float speed = 10.f;
	//if (input_wrapper->IsDown(KButton::NUMMINUS))
	//	speed -= 0.5f * dt;
	//if (input_wrapper->IsDown(KButton::NUMADD))
	//	speed += 0.5f * dt;



	//if (input_wrapper->IsDown(KButton::UP_ARROW))
	//	translation += forward * speed;

	//if (input_wrapper->IsDown(KButton::DOWN_ARROW))
	//	translation += forward * -speed;

	//if (input_wrapper->IsDown(KButton::RIGHT_ARROW))
	//	translation += right * speed;

	//if (input_wrapper->IsDown(KButton::LEFT_ARROW))
	//	translation += right * -speed;

	//if (input_wrapper->IsDown(KButton::PGDOWN))
	//	translation += up * -speed;

	//if (input_wrapper->IsDown(KButton::PGUP))
	//	translation += up * speed;

	//m_Orientation.SetTranslation(translation);


	//if (input_wrapper->IsDown(KButton::NUMPAD6))
	//	m_Orientation.RotateAroundPointY(m_Orientation.GetPosition(), cl::DegreeToRad(90.f) * dt);

	//if (input_wrapper->IsDown(KButton::NUMPAD4))
	//	m_Orientation.RotateAroundPointY(m_Orientation.GetPosition(), cl::DegreeToRad(-90.f) * dt);

	//if (input_wrapper->IsDown(KButton::NUMPAD8))
	//	m_Orientation.RotateAroundPointX(m_Orientation.GetPosition(), cl::DegreeToRad(90.f) * dt);

	//if (input_wrapper->IsDown(KButton::NUMPAD2))
	//	m_Orientation.RotateAroundPointX(m_Orientation.GetPosition(), cl::DegreeToRad(-90.f) * dt);

	//if (input_wrapper->IsDown(KButton::NUMPAD9))
	//	m_Orientation.RotateAroundPointZ(m_Orientation.GetPosition(), cl::DegreeToRad(90.f) * dt);

	//if (input_wrapper->IsDown(KButton::NUMPAD7))
	//	m_Orientation.RotateAroundPointZ(m_Orientation.GetPosition(), cl::DegreeToRad(-90.f) * dt);

	m_World.Update(dt, m_Paused);
}
