#include "Game.h"

/* Entity System Includes */
/* Components */

/* Systems */

/* End of Entity System Includes*/
/* Physics */
#include <PhysicsManager.h>
#include <RigidBody.h>

/* Physics End */

#include <Synchronizer.h>
#include <RenderCommand.h>
#include <Line3D.h>

#include <AssetsContainer.h>
#include <EngineDefines.h>
#include <DL_Debug.h>
#include <Engine.h>
#include <Terrain.h>
#include <Camera.h>
#include <Utilities.h>
#include <MousePicker.h>
#include "../Input/InputWrapper.h"
#include "../Input/ControllerInput.h"
#include <Model.h>

#include "../Input/InputHandle.h"
#include <RigidBody.h>

#include <Work.h>
#include <Worker.h>

#include <DataStructures/Hashmap/Hash.h>
#include <AABBComponent.h>
#include <hashlist.h>
#include <AABB.h>

bool Game::Initiate()
{
	myEngine = Snowblind::Engine::GetInstance();
	mySynchronizer = myEngine->GetSynchronizer();

	myEngine->LoadLevel("Data/Levels/level_01.json");
	/*if (!CreateLevel("Data/Levels/level_01.json"))
		return false;*/

	myPicker = new Snowblind::CMousePicker;

	myEngine->ToggleVsync();
	return true;
}

bool Game::CleanUp()
{
	SAFE_DELETE(myPicker);
	if (myPicker)
		return false;

	myTerrain.RemoveAll();
	if (myTerrain.Size() > 0)
		return false;

	return true;
}
static float speed = 50.f;

void Game::Update(float aDeltaTime)
{
	myFrameCount++;
	myAverageFPS += myEngine->GetFPS();
	myTime -= aDeltaTime;
	if (myTime <= 0.f)
	{
		myFPSToPrint = static_cast<u32>(myAverageFPS / myFrameCount);
		myFrameCount = 0;
		myAverageFPS = 0.f;
		myTime = 1.f;
	}

	std::stringstream ss;
	ss << "FPS : " << myEngine->GetFPS() << "\n" << "Average FPS : " << myFPSToPrint << "\nDeltaTime:" << aDeltaTime << "\n" << Snowblind::Engine::GetInstance()->GetLocalTimeAsString()
		<< "\ncamera_speed_multiplier : " << speed;
	//myEngine->AddDebugText(ss.str());

	// Should move to level? Should move...
	//mySynchronizer->AddRenderCommand(RenderCommand(eType::TERRAIN));
/*
	CU::Vector4f ray_dir = pointHit - myEngine->GetCamera()->GetPosition();
	CU::Math::Normalize(ray_dir);
*/
	//for (cl::AABB& aabb : m_AABBs)
	//{
	//	aabb.Update();
	//	//aabb.Intersect(m_Camera->GetOrientation().GetTranslation(), ray_dir);
	//	for (const RenderCommand& command : aabb.GetCommands())
	//	{
	//		mySynchronizer->AddRenderCommand(command);
	//	}
	//}

	mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, "Data/Model/cube.fbx", pointHit));
}
//
//bool Game::CreateLevel(const char* level_path)
//{

//
//	JSONReader reader(level_path);
//	const JSONElement& el = reader.GetElement("root");
//	for (JSONElement::ConstMemberIterator it = el.MemberBegin(); it != el.MemberEnd(); it++)
//	{
//		DL_ASSERT_EXP(CreateEntity(it->value["entity"].GetString(), reader, it), "Failed to create entity!");
//	}
//
//	return true;
//}

//bool Game::CreateEntity(const char* entity_path, JSONReader& level_reader, JSONElement::ConstMemberIterator it)
//{
//	if (entityReader.HasElement("light"))
//	{
//		myEntityManager->AddComponent<LightComponent>(e);
//		LightComponent& l = myEntityManager->GetComponent<LightComponent>(e);
//
//		std::string type;
//		entityReader.ReadElement("light", "type", type);
//		if (type == "pointlight")
//		{
//			l.myType = eLightType::ePOINTLIGHT;
//			level_reader._ReadElement(it->value["color"], l.color);
//			l.color.x /= 255.f;
//			l.color.y /= 255.f;
//			l.color.z /= 255.f;
//			level_reader.ReadElement(it->value["intensity"], l.intensity);
//			level_reader.ReadElement(it->value["range"], l.range);
//		}
//		else if (type == "spotlight")
//		{
//			l.myType = eLightType::eSPOTLIGHT;
//			level_reader._ReadElement(it->value["color"], l.color);
//			l.color.x /= 255.f;
//			l.color.y /= 255.f;
//			l.color.z /= 255.f;
//
//			level_reader._ReadElement(it->value["direction"], l.direction);
//
//			l.orientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(l.direction.y)) * l.orientation;
//			l.orientation = CU::Matrix44f::CreateRotateAroundX(CL::DegreeToRad(l.direction.x)) * l.orientation;
//			l.orientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(l.direction.z)) * l.orientation;
//
//
//			level_reader.ReadElement(it->value["angle"], l.angle);
//			l.angle = CL::DegreeToRad(l.angle);
//			level_reader.ReadElement(it->value["range"], l.range);
//		}
//	}
//
//

//
//	if (entityReader.HasElement("physics"))
//	{
//		const JSONElement& phys = entityReader.GetElement("physics");
//		for (JSONElement::ConstMemberIterator obj = phys.MemberBegin(); obj != phys.MemberEnd(); obj++)
//		{
//			float mass = static_cast<float>(obj->value.GetDouble());
//			myEntityManager->AddComponent<PhysicsComponent>(e);
//			PhysicsComponent& p = myEntityManager->GetComponent<PhysicsComponent>(e);
//			p.myBody = myPhysicsManager->CreateBody();
//			myPhysicsManager->Add(p.myBody->InitAsSphere(1.f, mass, myPhysicsManager->GetGravityForce(), 1.293f, pos));
//
//			if (!model_key.empty())
//			{
//				myEntityManager->AddComponent<AABBComponent>(e);
//				AABBComponent& aabb = myEntityManager->GetComponent<AABBComponent>(e);
//				aabb.m_Body = myPhysicsManager->CreateBody();
//				CU::Vector3f whd = Snowblind::Engine::GetInstance()->GetModel(model_key)->GetWHD();
//				myPhysicsManager->Add(aabb.m_Body->InitAsBox(whd.x, whd.y, whd.z, pos));
//			}
//
//		}
//	}
//
//	if (entityReader.HasElement("camera"))
//	{
//		myEntityManager->AddComponent<CameraComponent>(e);
//		CameraComponent& camera = myEntityManager->GetComponent<CameraComponent>(e);
//		camera.m_Camera = Snowblind::Engine::GetInstance()->GetCamera();
//	}
//
//	if (entityReader.HasElement("controller"))
//	{
//		std::string controller_type;
//		entityReader.ReadElement("controller", controller_type);
//
//		if (controller_type == "input")
//		{
//			PhysicsComponent& p = myEntityManager->GetComponent<PhysicsComponent>(e);
//			rigidbody = p.myBody;
//			p.m_IsPlayer = true;
//			myEntityManager->AddComponent<InputController>(e);
//			InputController& input = myEntityManager->GetComponent<InputController>(e);
//			input.m_ID = m_LocalPlayerCount++;
//
//			input.m_InputHandle = new InputHandle;
//			input.m_InputHandle->Initiate(input.m_ID);
//
//
//			Snowblind::Engine::GetInstance()->InitiateDebugSystem(input.m_InputHandle);
//
//
//			JSONReader read_input_config("Data/Config/input_config.json");
//			CameraComponent& camera = myEntityManager->GetComponent<CameraComponent>(e);
//
//			if (read_input_config.HasElement("Forward"))
//			{
//				const JSONElement& el = read_input_config.GetElement("Forward");
//				std::string first = read_input_config.ReadElement(el, 0);
//				std::string second = read_input_config.ReadElement(el, 1);
//
//				std::function<void()> function = [&]() {
//					if (rigidbody->IsEnabled())
//						Forward(rigidbody);
//					else
//						Forward(camera.m_Camera);
//				};
//
//			input.m_InputHandle->Bind(Hash(first.c_str()), function);
//			input.m_InputHandle->Bind(Hash(second.c_str()), function);
//			}
//
//			if (read_input_config.HasElement("Back"))
//			{
//				const JSONElement& el = read_input_config.GetElement("Back");
//				std::string first = read_input_config.ReadElement(el, 0);
//				std::string second = read_input_config.ReadElement(el, 1);
//
//				std::function<void()> function = [&]() {
//					if (rigidbody->IsEnabled())
//						Backward(rigidbody);
//					else
//						Backward(camera.m_Camera);
//				};
//
//				input.m_InputHandle->Bind(Hash(first.c_str()), function);
//				input.m_InputHandle->Bind(Hash(second.c_str()), function);
//			}
//
//			if (read_input_config.HasElement("Left"))
//			{
//				const JSONElement& el = read_input_config.GetElement("Left");
//				std::string first = read_input_config.ReadElement(el, 0);
//				std::string second = read_input_config.ReadElement(el, 1);
//
//				std::function<void()> function = [&]() {
//					if (rigidbody->IsEnabled())
//						Left(rigidbody);
//					else
//						Left(camera.m_Camera);
//				};
//
//				input.m_InputHandle->Bind(Hash(first.c_str()), function);
//				input.m_InputHandle->Bind(Hash(second.c_str()), function);
//			}
//
//			if (read_input_config.HasElement("Right"))
//			{
//				const JSONElement& el = read_input_config.GetElement("Right");
//				std::string first = read_input_config.ReadElement(el, 0);
//				std::string second = read_input_config.ReadElement(el, 1);
//
//				std::function<void()> function = [&]() {
//					if (rigidbody->IsEnabled())
//						Right(rigidbody);
//					else
//						Right(camera.m_Camera);
//				};
//
//				input.m_InputHandle->Bind(Hash(first.c_str()), function);
//				input.m_InputHandle->Bind(Hash(second.c_str()), function);
//			}
//
//			if (read_input_config.HasElement("Jump"))
//			{
//				const JSONElement& el = read_input_config.GetElement("Jump");
//				std::string first = read_input_config.ReadElement(el, 0);
//				std::string second = read_input_config.ReadElement(el, 1);
//
//				std::function<void()> function = [&]() {
//					if (rigidbody->IsEnabled())
//						Jump(rigidbody);
//					else
//						Up(camera.m_Camera);
//				};
//
//				input.m_InputHandle->Bind(Hash(first.c_str()), function);
//				input.m_InputHandle->Bind(Hash(second.c_str()), function);
//			}
//
//			input.m_InputHandle->Bind(Hash("RThumbYP"), [&]() {
//
//				if(rigidbody->IsEnabled())
//					p.myBody->UpdateOrientation(input.m_InputHandle->GetController().GetState());
//				else
//					camera.m_Camera->Update(input.m_InputHandle->GetController().GetState());
//
//
//			});
//
//			input.m_InputHandle->Bind(Hash("RThumbYN"), [&]() {
//				if (rigidbody->IsEnabled())
//					p.myBody->UpdateOrientation(input.m_InputHandle->GetController().GetState());
//				else
//					camera.m_Camera->Update(input.m_InputHandle->GetController().GetState());
//			});
//
//			input.m_InputHandle->Bind(Hash("RThumbXP"), [&]() {
//				if (rigidbody->IsEnabled())
//					p.myBody->UpdateOrientation(input.m_InputHandle->GetController().GetState());
//				else
//					camera.m_Camera->Update(input.m_InputHandle->GetController().GetState());
//			});
//
//			input.m_InputHandle->Bind(Hash("RThumbXN"), [&]() {
//				if (rigidbody->IsEnabled())
//					p.myBody->UpdateOrientation(input.m_InputHandle->GetController().GetState());
//				else
//					camera.m_Camera->Update(input.m_InputHandle->GetController().GetState());
//			});
//
//			input.m_InputHandle->Bind(Hash("YButton"), [&]() {
//				Snowblind::Engine::GetInstance()->ToggleWireframe();
//			});
//
//			input.m_InputHandle->Bind(Hash("LMouseButton"), [&]() {
//				Game::LeftClick(input.m_InputHandle->GetX(), input.m_InputHandle->GetY());
//			});
//
//			input.m_InputHandle->Bind(s_NumpadAdd_hash, [&]() {
//				speed += 0.1f;
//			});
//
//			input.m_InputHandle->Bind(s_NumpadMinus_hash, [&]() {
//				speed -= 0.1f;
//			});
//
//			input.m_InputHandle->Bind(s_RBumper_hash, [&]() {
//				camera.m_Camera->IncreaseLookModifier();
//			});
//
//			input.m_InputHandle->Bind(s_LBumper_hash, [&]() {
//				camera.m_Camera->DecreaseLookModifier();
//			});
//
//			input.m_InputHandle->Bind(s_MoveMouse_hash, [&]() {
//				if (!m_DisableMouseCameraMovement)
//					camera.m_Camera->Update(input.m_InputHandle->GetDX(), input.m_InputHandle->GetDY());
//			});
//
//			input.m_InputHandle->Bind(s_TabKey_hash, [&]() {
//				m_DisableMouseCameraMovement = !m_DisableMouseCameraMovement;
//			});
//
//			input.m_InputHandle->Bind(s_F9Key_hash, [&]() {
//				rigidbody->ToggleBody();
//				camera.m_Camera->ToggleFreefly();
//			});
//
//			m_Camera = camera.m_Camera;
//
//		}
//		else if (controller_type == "ai")
//		{
//			myEntityManager->AddComponent<AIController>(e);
//		}
//		else if (controller_type == "network")
//		{
//			myEntityManager->AddComponent<NetworkController>(e);
//		}
//	}
//
//	return true;
//}
//
//void Game::LeftClick(float x, float y)
//{
//	pointHit = myPhysicsManager->RayCast(myEngine->GetCamera()->GetPosition(), myPicker->GetCurrentRay(x, y));
//}
//
//// (#LINUS) Needs to be addressed in the future.
//
//void Jump(RigidBody* rigidbody)
//{
//	rigidbody->Impulse(CU::Vector3f(0, 1500, 0));
//}
//
//void Forward(RigidBody* rigidbody)
//{
//	CU::Matrix44f orientation = rigidbody->GetOrientation();
//	CU::Vector4f forward = orientation.GetForward();
//	forward *= 150.f;
//	rigidbody->Impulse(CU::Vector3f(forward.x, 0, forward.z));
//}
//
//void Backward(RigidBody* rigidbody)
//{
//	CU::Matrix44f orientation = rigidbody->GetOrientation();
//	CU::Vector4f forward = orientation.GetForward();
//	forward *= -150.f;
//	rigidbody->Impulse(CU::Vector3f(forward.x, 0, forward.z));
//}
//
//void Right(RigidBody* rigidbody)
//{
//	CU::Matrix44f orientation = rigidbody->GetOrientation();
//	CU::Vector4f right = orientation.GetRight();
//	right *= 150.f;
//	rigidbody->Impulse(CU::Vector3f(right.x, 0, right.z));
//}
//
//void Left(RigidBody* rigidbody)
//{
//	CU::Matrix44f orientation = rigidbody->GetOrientation();
//	CU::Vector4f right = orientation.GetRight();
//	right *= -150.f;
//	rigidbody->Impulse(CU::Vector3f(right.x, 0, right.z));
//}
//
//
//void Up(Snowblind::Camera* camera)
//{
//	camera->Move(Snowblind::eDirection::UP, speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
//}
//
//void Down(Snowblind::Camera* camera)
//{
//	camera->Move(Snowblind::eDirection::DOWN, -speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
//}
//
//void Forward(Snowblind::Camera* camera)
//{
//	camera->Move(Snowblind::eDirection::FORWARD, speed* Snowblind::Engine::GetInstance()->GetDeltaTime());
//}
//
//void Backward(Snowblind::Camera* camera)
//{
//	camera->Move(Snowblind::eDirection::BACK, -speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
//}
//
//void Right(Snowblind::Camera* camera)
//{
//	camera->Move(Snowblind::eDirection::RIGHT, speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
//}
//
//void Left(Snowblind::Camera* camera)
//{
//	camera->Move(Snowblind::eDirection::LEFT, -speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
//}
