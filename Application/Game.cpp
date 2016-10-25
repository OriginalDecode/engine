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

bool CGame::Initiate(Snowblind::CSynchronizer* synchronizer)
{
	mySynchronizer = synchronizer;
	myEngine = Snowblind::CEngine::GetInstance();

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

	myPicker = new Snowblind::CMousePicker();
	m_ModelKey = myEngine->LoadModel("Data/Model/cube.fbx", "Data/Shaders/T_Deferred_Base.json");

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

	myTerrain.DeleteAll();
	if (myTerrain.Size() > 0)
		return false;

	return true;
}


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
	ss << myEngine->GetFPS() << "\n" << myFPSToPrint << "\nDeltaTime:" << aDeltaTime << "\n" << Snowblind::CEngine::GetInstance()->GetLocalTimeAsString();

	//if (CU::Input::InputWrapper::GetInstance()->MouseDown(0))
	//{
	//	pointHit = myPhysicsManager->RayCast(myEngine->GetCamera()->GetPosition(), myPicker->GetCurrentRay());

	//	currentRay = pointHit;
	//	raycast[0] = SLinePoint(myEngine->GetCamera()->GetPosition(), CU::Vector3f(0, 1, 0));
	//	raycast[1] = SLinePoint(pointHit, CU::Vector3f(0, 1, 0));
	//}

	/*std::stringstream p;
	p << "Raycast Point" << "\nX : " << pointHit.x << "\nY : " << pointHit.y << "\nZ : " << pointHit.z;*/

	/*std::stringstream c;
	c << "Cursor Coord" << "\nX : " << CU::Input::InputWrapper::GetInstance()->GetX() << "\nY : " << CU::Input::InputWrapper::GetInstance()->GetY() 
		<< "\nRay coord" << "\nX : " << currentRay.x << "\nY : " << currentRay.y << "\nZ : " << currentRay.z;

	mySynchronizer->AddRenderCommand(RenderCommand(c.str(), { 200, 900 }, eType::TEXT));*/
	//mySynchronizer->AddRenderCommand(RenderCommand(p.str(), { 0, 900 }, eType::TEXT));
	mySynchronizer->AddRenderCommand(RenderCommand(ss.str(), { 0, 0 }, eType::TEXT));
	mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, m_ModelKey, pointHit));
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, raycast[0], raycast[1]));

	mySynchronizer->AddRenderCommand(RenderCommand(eType::TERRAIN));


	std::stringstream b;
	if (rigidbody)
	{
		CU::Vector3f vec = rigidbody->GetLinearVelocity();
		b << "player_entity_velocity :\nX : " << vec.x << "\nY : " << vec.y << "\nZ : " << vec.z;
		mySynchronizer->AddRenderCommand(RenderCommand(b.str(), { 0, 500 }, eType::TEXT));
	}


	myEntityManager->Update(aDeltaTime);
}

bool CGame::CreateLevel(const char* level_path)
{
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/t_0.tga", CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512)));
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/t_1.tga", CU::Vector3f(0, 0, 500), CU::Vector2f(512, 512)));
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/t_2.tga", CU::Vector3f(500, 0, 0), CU::Vector2f(512, 512)));
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/t_3.tga", CU::Vector3f(500, 0, 500), CU::Vector2f(512, 512)));

	/*for (s32 i = 0; i < myTerrain.Size(); i++)
	{
		myTerrainBodies.Add(myPhysicsManager->CreateBody());
		myPhysicsManager->Add(myTerrainBodies[i]->InitAsTerrain(myTerrain[i]->GetVerticeArrayCopy(), myTerrain[i]->GetIndexArrayCopy()));
	}*/

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

	if (entityReader.HasElement("render"))
	{
		myEntityManager->AddComponent<RenderComponent>(e);
		RenderComponent& r = myEntityManager->GetComponent<RenderComponent>(e);
		std::string entityModel[2];
		entityReader.ReadElement("render", entityModel);
		r.myModelID = myEngine->LoadModel(entityModel[0], entityModel[1]);
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
		}
	}

	if (entityReader.HasElement("controller"))
	{
		std::string controller_type;
		entityReader.ReadElement("controller", controller_type);

		if (controller_type == "input")
		{
			PhysicsComponent& p = myEntityManager->GetComponent<PhysicsComponent>(e);
			rigidbody = p.myBody;

			myEntityManager->AddComponent<InputController>(e);
			InputController& input = myEntityManager->GetComponent<InputController>(e);
			input.m_ID = m_LocalPlayerCount++;
			//input.m_Controller = new ControllerInput(input.m_ID);
			input.m_InputHandle = new InputHandle;
			input.m_InputHandle->Initiate(input.m_ID);

			InputCommand* bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Jump, rigidbody);
			input.m_InputHandle->BindAButton(bind_me);

			bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Jump, rigidbody);
			input.m_InputHandle->BindSpaceBar(bind_me);

			bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Forward, rigidbody);
			input.m_InputHandle->BindLThumbYP(bind_me);

			bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Forward, rigidbody);
			input.m_InputHandle->BindWKey(bind_me);

			bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Backward, rigidbody);
			input.m_InputHandle->BindLThumbYN(bind_me);

			bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Backward, rigidbody);
			input.m_InputHandle->BindSKey(bind_me);

			bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Left, rigidbody);
			input.m_InputHandle->BindLThumbXN(bind_me);

			bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Left, rigidbody);
			input.m_InputHandle->BindAKey(bind_me);

			bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Right, rigidbody);
			input.m_InputHandle->BindLThumbXP(bind_me);

			bind_me = new InputCommand;
			bind_me->m_Function = std::bind(&Right, rigidbody);
			input.m_InputHandle->BindDKey(bind_me);


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

	if (entityReader.HasElement("camera"))
	{
		myEntityManager->AddComponent<CameraComponent>(e);
		CameraComponent& camera = myEntityManager->GetComponent<CameraComponent>(e);
		camera.m_Camera = Snowblind::CEngine::GetInstance()->GetCamera();
	}
	return true;
}

void Jump(CRigidBody* rigidbody)
{
	rigidbody->Impulse(CU::Vector3f(0, 1500, 0));
}

void Forward(CRigidBody* rigidbody)
{
	rigidbody->Impulse(CU::Vector3f(0, 0, 1500));
}

void Backward(CRigidBody* rigidbody)
{
	rigidbody->Impulse(CU::Vector3f(0, 0, -1500));
}

void Right(CRigidBody* rigidbody)
{
	rigidbody->Impulse(CU::Vector3f(1500, 0, 0));
}

void Left(CRigidBody* rigidbody)
{
	rigidbody->Impulse(CU::Vector3f(-1500, 0, 0));
}
