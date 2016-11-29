#include "Game.h"

/* Entity System Includes */
#include <EntityManager.h>
/* Components */
#include <PhysicsComponent.h>
#include <RenderComponent.h>
#include <TranslationComponent.h>
#include <LightComponent.h>
#include <InputComponent.h>
#include <AIComponent.h>
#include <NetworkComponent.h>
#include <CameraComponent.h>
/* Systems */
#include <PhysicsSystem.h>
#include <RenderSystem.h>
#include <LightSystem.h>
#include <InputSystem.h>
#include <AISystem.h>
#include <NetworkSystem.h>
#include <CameraSystem.h>
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

bool CGame::Initiate(Snowblind::CSynchronizer* synchronizer)
{
	mySynchronizer = synchronizer;
	myEngine = Snowblind::Engine::GetInstance();

	myEntityManager = new CEntityManager;
	myPhysicsManager = new CPhysicsManager;


	if (!CreateLevel("Data/Levels/level_01.json"))
		return false;

	myEntityManager->AddSystem<CPhysicsSystem>(myPhysicsManager);
	myEntityManager->AddSystem<CRenderSystem>(mySynchronizer);
	myEntityManager->AddSystem<CLightSystem>(mySynchronizer);
	myEntityManager->AddSystem<InputSystem>();
	myEntityManager->AddSystem<NetworkSystem>();
	myEntityManager->AddSystem<AISystem>();
	myEntityManager->AddSystem<CameraSystem>();

	myPicker = new Snowblind::CMousePicker;

	myEngine->ToggleVsync();
	return true;
}

bool CGame::CleanUp()
{
	SAFE_DELETE(myPicker);
	if (myPicker)
		return false;

	SAFE_DELETE(myPhysicsManager);
	if (myPhysicsManager)
		return false;

	SAFE_DELETE(myEntityManager);
	if (myEntityManager)
		return false;

	myTerrain.RemoveAll();
	if (myTerrain.Size() > 0)
		return false;

	return true;
}
static float speed = 50.f;

void CGame::Update(float aDeltaTime)
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
	myEngine->AddDebugText(ss.str());
	mySynchronizer->AddRenderCommand(RenderCommand(eType::TERRAIN));

	CU::Vector4f ray_dir = pointHit - myEngine->GetCamera()->GetPosition();
	CU::Math::Normalize(ray_dir);
	for (cl::AABB& aabb : m_AABBs)
	{
		aabb.Update();
		/*aabb.Intersect(pointHit, ray_dir);*/
		for (const RenderCommand& command : aabb.GetCommands())
		{
			mySynchronizer->AddRenderCommand(command);
		}
	}

	mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, "Data/Model/cube.fbx", pointHit));

	myEntityManager->Update(aDeltaTime);
	
}

bool CGame::CreateLevel(const char* level_path)
{
	Snowblind::Engine::GetInstance()->GetThreadpool().AddWork(
		Work([&]() {
		Snowblind::CTerrain* terrain = myEngine->CreateTerrain("Data/Textures/flat_height.tga", CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512));
		terrain->AddNormalMap("Data/Textures/normal.dds");
		myTerrain.Add(terrain);

		//terrain = myEngine->CreateTerrain("Data/Textures/t_1.tga", CU::Vector3f(0, 0, 510), CU::Vector2f(512, 512));
		//terrain->AddNormalMap("Data/Textures/t1_n.dds");
		//myTerrain.Add(terrain);

		//terrain = myEngine->CreateTerrain("Data/Textures/t_2.tga", CU::Vector3f(510, 0, 0), CU::Vector2f(512, 512));
		//terrain->AddNormalMap("Data/Textures/t2_n.dds");
		//myTerrain.Add(terrain);

		//terrain = myEngine->CreateTerrain("Data/Textures/t_3.tga", CU::Vector3f(510, 0, 510), CU::Vector2f(512, 512));
		//terrain->AddNormalMap("Data/Textures/t3_n.dds");
		//myTerrain.Add(terrain);

		for (s32 i = 0; i < myTerrain.Size(); i++)
		{
			myTerrainBodies.Add(myPhysicsManager->CreateBody());
			myPhysicsManager->Add(myTerrainBodies[i]->InitAsTerrain(myTerrain[i]->GetVerticeArrayCopy(), myTerrain[i]->GetIndexArrayCopy()));
		}

	}));

	JSONReader reader(level_path);
	const JSONElement& el = reader.GetElement("root");
	for (JSONElement::ConstMemberIterator it = el.MemberBegin(); it != el.MemberEnd(); it++)
	{
		DL_ASSERT_EXP(CreateEntity(it->value["entity"].GetString(), reader, it), "Failed to create entity!");
	}

	return true;
}

bool CGame::CreateEntity(const char* entity_path, JSONReader& level_reader, JSONElement::ConstMemberIterator it)
{
	JSONReader entityReader(entity_path);
	Entity e = myEntityManager->CreateEntity();

	myEntityManager->AddComponent<TranslationComponent>(e);

	CU::Vector3f pos;
	level_reader._ReadElement(it->value["position"], pos);

	TranslationComponent& t = myEntityManager->GetComponent<TranslationComponent>(e);
	t.myOrientation.SetPosition(pos);

	if (entityReader.HasElement("light"))
	{
		myEntityManager->AddComponent<LightComponent>(e);
		LightComponent& l = myEntityManager->GetComponent<LightComponent>(e);

		std::string type;
		entityReader.ReadElement("light", "type", type);
		if (type == "pointlight")
		{
			l.myType = eLightType::ePOINTLIGHT;
			level_reader._ReadElement(it->value["color"], l.color);
			l.color.r /= 255.f;
			l.color.g /= 255.f;
			l.color.b /= 255.f;
			level_reader.ReadElement(it->value["intensity"], l.intensity);
			level_reader.ReadElement(it->value["range"], l.range);
		}
		else if (type == "spotlight")
		{
			l.myType = eLightType::eSPOTLIGHT;
			level_reader._ReadElement(it->value["color"], l.color);
			l.color.r /= 255.f;
			l.color.g /= 255.f;
			l.color.b /= 255.f;

			level_reader._ReadElement(it->value["direction"], l.direction);

			l.orientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(l.direction.y)) * l.orientation;
			l.orientation = CU::Matrix44f::CreateRotateAroundX(CL::DegreeToRad(l.direction.x)) * l.orientation;
			l.orientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(l.direction.z)) * l.orientation;


			level_reader.ReadElement(it->value["angle"], l.angle);
			l.angle = CL::DegreeToRad(l.angle);
			level_reader.ReadElement(it->value["range"], l.range);
		}
	}


	std::string model_key;
	if (entityReader.HasElement("render"))
	{
		myEntityManager->AddComponent<RenderComponent>(e);
		RenderComponent& r = myEntityManager->GetComponent<RenderComponent>(e);

		std::string entityModel[2];
		entityReader.ReadElement("render", entityModel);
		r.myModelID = myEngine->LoadModel(entityModel[0], entityModel[1]);
		model_key = r.myModelID;
		TranslationComponent& t = myEntityManager->GetComponent<TranslationComponent>(e);

		cl::AABB aabb;
		aabb.Initiate(e, &t.myOrientation, Snowblind::Engine::GetInstance()->GetModel(r.myModelID)->GetWHD());
		m_AABBs.Add(aabb);
	}

	if (entityReader.HasElement("physics"))
	{
		const JSONElement& phys = entityReader.GetElement("physics");
		for (JSONElement::ConstMemberIterator obj = phys.MemberBegin(); obj != phys.MemberEnd(); obj++)
		{
			float mass = static_cast<float>(obj->value.GetDouble());
			myEntityManager->AddComponent<PhysicsComponent>(e);
			PhysicsComponent& p = myEntityManager->GetComponent<PhysicsComponent>(e);
			p.myBody = myPhysicsManager->CreateBody();
			myPhysicsManager->Add(p.myBody->InitAsSphere(1.f, mass, myPhysicsManager->GetGravityForce(), 1.293f, pos));

			myEntityManager->AddComponent<AABBComponent>(e);
			AABBComponent& aabb = myEntityManager->GetComponent<AABBComponent>(e);
			aabb.m_Body = myPhysicsManager->CreateBody();
			CU::Vector3f whd = Snowblind::Engine::GetInstance()->GetModel(model_key)->GetWHD();
			myPhysicsManager->Add(aabb.m_Body->InitAsBox(whd.x, whd.y, whd.z, pos));


		}
	}

	if (entityReader.HasElement("camera"))
	{
		myEntityManager->AddComponent<CameraComponent>(e);
		CameraComponent& camera = myEntityManager->GetComponent<CameraComponent>(e);
		camera.m_Camera = Snowblind::Engine::GetInstance()->GetCamera();
	}

	if (entityReader.HasElement("controller"))
	{
		std::string controller_type;
		entityReader.ReadElement("controller", controller_type);

		if (controller_type == "input")
		{
			//PhysicsComponent& p = myEntityManager->GetComponent<PhysicsComponent>(e);
			//rigidbody = p.myBody;
			myEntityManager->AddComponent<InputController>(e);
			InputController& input = myEntityManager->GetComponent<InputController>(e);
			input.m_ID = m_LocalPlayerCount++;

			input.m_InputHandle = new InputHandle;
			input.m_InputHandle->Initiate(input.m_ID);

			Snowblind::Engine::GetInstance()->InitiateDebugSystem(mySynchronizer, input.m_InputHandle);


			JSONReader read_input_config("Data/Config/input_config.json");
			CameraComponent& camera = myEntityManager->GetComponent<CameraComponent>(e);

			if (read_input_config.HasElement("Forward"))
			{
				const JSONElement& el = read_input_config.GetElement("Forward");
				std::string first = read_input_config.ReadElement(el, 0);
				std::string second = read_input_config.ReadElement(el, 1);

				std::function<void()> function;

				if (rigidbody)
					function = [&]() { Forward(rigidbody); };
				else
					function = [&]() { Forward(camera.m_Camera); };

				input.m_InputHandle->Bind(Hash(first.c_str()), function);
				input.m_InputHandle->Bind(Hash(second.c_str()), function);
			}

			if (read_input_config.HasElement("Back"))
			{
				const JSONElement& el = read_input_config.GetElement("Back");
				std::string first = read_input_config.ReadElement(el, 0);
				std::string second = read_input_config.ReadElement(el, 1);

				std::function<void()> function;

				if (rigidbody)
					function = [&]() { Backward(rigidbody); };
				else
					function = [&]() { Backward(camera.m_Camera); };

				input.m_InputHandle->Bind(Hash(first.c_str()), function);
				input.m_InputHandle->Bind(Hash(second.c_str()), function);
			}

			if (read_input_config.HasElement("Left"))
			{
				const JSONElement& el = read_input_config.GetElement("Left");
				std::string first = read_input_config.ReadElement(el, 0);
				std::string second = read_input_config.ReadElement(el, 1);

				std::function<void()> function;

				if (rigidbody)
					function = [&]() { Left(rigidbody); };
				else
					function = [&]() { Left(camera.m_Camera); };

				input.m_InputHandle->Bind(Hash(first.c_str()), function);
				input.m_InputHandle->Bind(Hash(second.c_str()), function);
			}

			if (read_input_config.HasElement("Right"))
			{
				const JSONElement& el = read_input_config.GetElement("Right");
				std::string first = read_input_config.ReadElement(el, 0);
				std::string second = read_input_config.ReadElement(el, 1);

				std::function<void()> function;

				if (rigidbody)
					function = [&]() { Right(rigidbody); };
				else
					function = [&]() { Right(camera.m_Camera); };

				input.m_InputHandle->Bind(Hash(first.c_str()), function);
				input.m_InputHandle->Bind(Hash(second.c_str()), function);
			}

			if (read_input_config.HasElement("Jump"))
			{
				const JSONElement& el = read_input_config.GetElement("Jump");
				std::string first = read_input_config.ReadElement(el, 0);
				std::string second = read_input_config.ReadElement(el, 1);

				std::function<void()> function;

				if (rigidbody)
					function = [&]() { Jump(rigidbody); };
				else
					function = [&]() { Up(camera.m_Camera); };

				input.m_InputHandle->Bind(Hash(first.c_str()), function);
				input.m_InputHandle->Bind(Hash(second.c_str()), function);
			}

			input.m_InputHandle->Bind(Hash("RThumbYP"), [&]() {
				//p.myBody->UpdateOrientation(input.m_InputHandle->GetController().GetState());
				camera.m_Camera->Update(input.m_InputHandle->GetController().GetState());
			});

			input.m_InputHandle->Bind(Hash("RThumbYN"), [&]() {
				//p.myBody->UpdateOrientation(input.m_InputHandle->GetController().GetState());
				camera.m_Camera->Update(input.m_InputHandle->GetController().GetState());
			});

			input.m_InputHandle->Bind(Hash("RThumbXP"), [&]() {
				//p.myBody->UpdateOrientation(input.m_InputHandle->GetController().GetState());
				camera.m_Camera->Update(input.m_InputHandle->GetController().GetState());
			});

			input.m_InputHandle->Bind(Hash("RThumbXN"), [&]() {
				//p.myBody->UpdateOrientation(input.m_InputHandle->GetController().GetState());
				camera.m_Camera->Update(input.m_InputHandle->GetController().GetState());
			});

			input.m_InputHandle->Bind(Hash("YButton"), [&]() {
				Snowblind::Engine::GetInstance()->ToggleWireframe(); 
			});

			input.m_InputHandle->Bind(Hash("LMouseButton"), [&]() {
				CGame::LeftClick(input.m_InputHandle->GetX(), input.m_InputHandle->GetY());
			});

			input.m_InputHandle->Bind(s_NumpadAdd_hash, [&]() {
				speed += 0.1f;
			});

			input.m_InputHandle->Bind(s_NumpadMinus_hash, [&]() {
				speed -= 0.1f;
			});
			
			input.m_InputHandle->Bind(s_RBumper_hash, [&]() {
				camera.m_Camera->IncreaseLookModifier();
			});

			input.m_InputHandle->Bind(s_LBumper_hash, [&]() {
				camera.m_Camera->DecreaseLookModifier();
			});

			input.m_InputHandle->Bind(s_MoveMouse_hash, [&]() {
				if(!m_DisableMouseCameraMovement)
					camera.m_Camera->Update(input.m_InputHandle->GetDX(), input.m_InputHandle->GetDY());
			});

			input.m_InputHandle->Bind(s_TabKey_hash, [&]() {
				m_DisableMouseCameraMovement = !m_DisableMouseCameraMovement;
			});

		}
		else if (controller_type == "ai")
		{
			myEntityManager->AddComponent<AIController>(e);
		}
		else if (controller_type == "network")
		{
			myEntityManager->AddComponent<NetworkController>(e);
		}
	}

	return true;
}

void CGame::LeftClick(float x, float y)
{
	pointHit = myPhysicsManager->RayCast(myEngine->GetCamera()->GetPosition(), myPicker->GetCurrentRay(x, y));
}

// (#LINUS) Needs to be addressed in the future.

void Jump(CRigidBody* rigidbody)
{
	rigidbody->Impulse(CU::Vector3f(0, 1500, 0));
}

void Forward(CRigidBody* rigidbody)
{
	CU::Matrix44f orientation = rigidbody->GetOrientation();
	CU::Vector4f forward = orientation.GetForward();
	forward *= 150.f;
	rigidbody->Impulse(CU::Vector3f(forward.x, 0, forward.z));
}

void Backward(CRigidBody* rigidbody)
{
	CU::Matrix44f orientation = rigidbody->GetOrientation();
	CU::Vector4f forward = orientation.GetForward();
	forward *= -150.f;
	rigidbody->Impulse(CU::Vector3f(forward.x, 0, forward.z));
}

void Right(CRigidBody* rigidbody)
{
	CU::Matrix44f orientation = rigidbody->GetOrientation();
	CU::Vector4f right = orientation.GetRight();
	right *= 150.f;
	rigidbody->Impulse(CU::Vector3f(right.x, 0, right.z));
}

void Left(CRigidBody* rigidbody)
{
	CU::Matrix44f orientation = rigidbody->GetOrientation();
	CU::Vector4f right = orientation.GetRight();
	right *= -150.f;
	rigidbody->Impulse(CU::Vector3f(right.x, 0, right.z));
}


void Up(Snowblind::CCamera* camera)
{
	camera->Move(Snowblind::eDirection::UP, speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
}

void Down(Snowblind::CCamera* camera)
{
	camera->Move(Snowblind::eDirection::DOWN, -speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
}

void Forward(Snowblind::CCamera* camera)
{
	camera->Move(Snowblind::eDirection::FORWARD, speed* Snowblind::Engine::GetInstance()->GetDeltaTime());
}

void Backward(Snowblind::CCamera* camera)
{
	camera->Move(Snowblind::eDirection::BACK, -speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
}

void Right(Snowblind::CCamera* camera)
{
	camera->Move(Snowblind::eDirection::RIGHT, speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
}

void Left(Snowblind::CCamera* camera)
{
	camera->Move(Snowblind::eDirection::LEFT, -speed * Snowblind::Engine::GetInstance()->GetDeltaTime());
}
