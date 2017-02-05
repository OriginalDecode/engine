#include "RenderSystem.h"

#include <Engine.h>
#include "../Engine/Model.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "LightComponent.h"

/* Engine Includes */
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "AABBComponent.h"
#include <Utilities.h>

#include "../Engine/Engine.h"

CRenderSystem::CRenderSystem(EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, RenderComponent>>())
{
	mySynchronizer = m_Engine->GetSynchronizer();
}


void CRenderSystem::Update(float dt)
{
	
	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (int i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		RenderComponent& render = GetComponent<RenderComponent>(e);

		

		//#LINUS This needs to be profiled.
		CU::Matrix44f t = translation.myOrientation;
		t = CU::Matrix44f::CreateScaleMatrix(render.scale) * t;

		//if (e == 0)
		{
			/**/CU::Vector3f pos = { 256.f, 5.f, 256.f };
			CU::Vector3f original_pos = translation.myOrientation.GetPosition();
			translation.myOrientation.SetPosition(original_pos - pos);

			translation.myOrientation = translation.myOrientation * CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(25.f) * dt);
			translation.myOrientation.SetPosition(translation.myOrientation.GetPosition() + pos);/**/
		}

		mySynchronizer->AddRenderCommand(RenderCommand(
			eType::MODEL
			, render.myModelID
			, translation.myOrientation
			, render.scale));


	}
}
