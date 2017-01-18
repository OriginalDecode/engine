#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"
#include "DebugComponent.h"

#include "../Physics/RigidBody.h"
#include "../Physics/PhysicsManager.h"
#include "AABBComponent.h"

#include "../Engine/Engine.h"
#include "../Engine/Synchronizer.h"
#include "../Engine/RenderCommand.h"

CPhysicsSystem::CPhysicsSystem(EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, DebugComponent>>())
{
	myPhysicsManager = Hex::Engine::GetInstance()->GetPhysicsManager();
}

void CPhysicsSystem::Update(float aDeltaTime)
{
	myAccumulatedTime += aDeltaTime;
	while (myAccumulatedTime >= 1.f / 60.f)
	{
		const CU::GrowingArray<Entity>& entities = GetEntities();
		for (int i = 0; i < entities.Size(); i++)
		{
			Entity e = entities[i];
			STranslationComponent& translation = GetComponent<STranslationComponent>(e);
			/*S
			PhysicsComponent& physics = GetComponent<SPhysicsComponent>(e);
			if (!physics.myBody->IsEnabled())
				continue;
			

			translation.myOrientation = physics.myBody->GetOrientation();
			physics.myBody->Update(aDeltaTime);
			*/
			
			//DebugComponent& debug = GetComponent<DebugComponent>(e);
			//CU::Vector3f pos = translation.myOrientation.GetPosition();
			//debug.m_Body->SetPosition(pos);
			//debug.m_Body->
			//Hex::Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE,)
			//if(!physics.m_IsPlayer)
			//{
			//	AABBComponent& aabb = GetComponent<AABBComponent>(e);
			//	if (aabb.m_Body)
			//		aabb.m_Body->Update(aDeltaTime);
			//}

		}
		myPhysicsManager->Update(myAccumulatedTime); //ASync Physics?
		myAccumulatedTime -= 1.f / 60.f;
	}
}
