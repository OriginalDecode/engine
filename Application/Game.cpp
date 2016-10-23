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
#include <JSON/JSONReader.h>
#include <DL_Debug.h>
#include <Engine.h>
#include <Terrain.h>
#include <Camera.h>
#include <Utilities.h>
#include <MousePicker.h>
#include "../Input/InputWrapper.h"
#include <Model.h>

#include <RigidBody.h>

CGame::CGame(Snowblind::CSynchronizer* aSynchronizer)
	: mySynchronizer(aSynchronizer)
{
	myEntityManager = new CEntityManager();
	myPhysicsManager = new CPhysicsManager();
	myEngine = Snowblind::CEngine::GetInstance();

	myEngine->ToggleVsync();

	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/t_0.tga", CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512)));
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/t_1.tga", CU::Vector3f(0, 0, 500), CU::Vector2f(512, 512)));
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/t_2.tga", CU::Vector3f(500, 0, 0), CU::Vector2f(512, 512)));
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/t_3.tga", CU::Vector3f(500, 0, 500), CU::Vector2f(512, 512)));

	for (s32 i = 0; i < myTerrain.Size(); i++)
	{
		myTerrainBodies.Add(myPhysicsManager->CreateBody());
		myPhysicsManager->Add(myTerrainBodies[i]->InitAsTerrain(myTerrain[i]->GetVerticeArrayCopy(), myTerrain[i]->GetIndexArrayCopy()));
	}

	JSONReader reader("Data/Levels/level_01.json");
	const JSONElement& el = reader.GetElement("root");
	for (JSONElement::ConstMemberIterator it = el.MemberBegin(); it != el.MemberEnd(); it++)
	{
		std::string entityPath;
		entityPath = it->value["entity"].GetString();
		CU::Vector3f pos;
		reader._ReadElement(it->value["position"], pos);

		JSONReader entityReader(entityPath);

		Entity e = myEntityManager->CreateEntity();

		//All entities usually have a start position.
		myEntityManager->AddComponent<TranslationComponent>(e);
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
				reader._ReadElement(it->value["color"], l.color);
				l.color.r /= 255.f;
				l.color.g /= 255.f;
				l.color.b /= 255.f;
				reader.ReadElement(it->value["intensity"], l.intensity);
				reader.ReadElement(it->value["range"], l.range);
			}
			else if (type == "spotlight")
			{
				l.myType = eLightType::eSPOTLIGHT;
				reader._ReadElement(it->value["color"], l.color);
				l.color.r /= 255.f;
				l.color.g /= 255.f;
				l.color.b /= 255.f;

				reader._ReadElement(it->value["direction"], l.direction);

				l.orientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(l.direction.y)) * l.orientation;
				l.orientation = CU::Matrix44f::CreateRotateAroundX(CL::DegreeToRad(l.direction.x)) * l.orientation;
				l.orientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(l.direction.z)) * l.orientation;


				reader.ReadElement(it->value["angle"], l.angle);
				l.angle = CL::DegreeToRad(l.angle);
				reader.ReadElement(it->value["range"], l.range);
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
	}

	reader.CloseDocument();
	reader.OpenDocument("Data/Levels/player.json");
	if (reader.HasElement("controller"))
	{
		Entity e = myEntityManager->CreateEntity();
		std::string controller_type;
		reader.ReadElement("controller", controller_type);

		if (controller_type == "input")
		{
			myEntityManager->AddComponent<InputController>(e);
			myEntityManager->AddComponent<PhysicsComponent>(e);
			myEntityManager->AddComponent<TranslationComponent>(e);
			PhysicsComponent& p = myEntityManager->GetComponent<PhysicsComponent>(e);
			p.myBody = myPhysicsManager->CreateBody();
			myPhysicsManager->Add(p.myBody->InitAsSphere(1.f, 10, myPhysicsManager->GetGravityForce(), 1.293f, CU::Vector3f(5,20,5)));
			rigidbody = p.myBody;
			myEntityManager->AddComponent<CameraComponent>(e);
			CameraComponent& camera = myEntityManager->GetComponent<CameraComponent>(e);
			camera.m_Camera = Snowblind::CEngine::GetInstance()->GetCamera();

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

	myEntityManager->AddSystem<CPhysicsSystem>(myPhysicsManager);
	myEntityManager->AddSystem<CRenderSystem>(mySynchronizer);
	myEntityManager->AddSystem<CLightSystem>(mySynchronizer);

	myEntityManager->AddSystem<InputSystem>();
	myEntityManager->AddSystem<NetworkSystem>();
	myEntityManager->AddSystem<AISystem>();
	myEntityManager->AddSystem<CameraSystem>();

	myPicker = new Snowblind::CMousePicker();
	m_ModelKey = myEngine->LoadModel("Data/Model/cube.fbx", "Data/Shaders/T_Deferred_Base.json");
}

CGame::~CGame()
{
	SAFE_DELETE(myPicker);
	SAFE_DELETE(myPhysicsManager);
	SAFE_DELETE(myEntityManager);
	myTerrain.DeleteAll();
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

	if (CU::Input::InputWrapper::GetInstance()->MouseDown(0))
	{
		pointHit = myPhysicsManager->RayCast(myEngine->GetCamera()->GetPosition(), myPicker->GetCurrentRay());

		currentRay = pointHit;
		raycast[0] = SLinePoint(myEngine->GetCamera()->GetPosition(), CU::Vector3f(0, 1, 0));
		raycast[1] = SLinePoint(pointHit, CU::Vector3f(0, 1, 0));
	}

	std::stringstream p;
	p << "Raycast Point" << "\nX : " << pointHit.x << "\nY : " << pointHit.y << "\nZ : " << pointHit.z;

	std::stringstream c;
	c << "Cursor Coord" << "\nX : " << CU::Input::InputWrapper::GetInstance()->GetX() << "\nY : " << CU::Input::InputWrapper::GetInstance()->GetY() 
		<< "\nRay coord" << "\nX : " << currentRay.x << "\nY : " << currentRay.y << "\nZ : " << currentRay.z;

	mySynchronizer->AddRenderCommand(RenderCommand(c.str(), { 200, 900 }, eType::TEXT));
	mySynchronizer->AddRenderCommand(RenderCommand(p.str(), { 0, 900 }, eType::TEXT));
	mySynchronizer->AddRenderCommand(RenderCommand(ss.str(), { 0, 0 }, eType::TEXT));
	mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, m_ModelKey, pointHit));
	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, raycast[0], raycast[1]));

	mySynchronizer->AddRenderCommand(RenderCommand(eType::TERRAIN));


	std::stringstream b;
	CU::Vector3f vec = rigidbody->GetLinearVelocity();
	b << "player_entity_velocity :\nX : " << vec.x << "\nY : "<< vec.y << "\nZ : " << vec.z;
	mySynchronizer->AddRenderCommand(RenderCommand(b.str(), { 0, 500 }, eType::TEXT));



	myEntityManager->Update(aDeltaTime);
}
