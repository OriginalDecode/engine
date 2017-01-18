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
	mySynchronizer = Hex::Engine::GetInstance()->GetSynchronizer();
}

void CRenderSystem::Update(float /*aDeltaTime*/)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();

	for (int i = 0; i < entities.Size(); i++)
	{
			Entity e = entities[i];
			TranslationComponent& translation = GetComponent<TranslationComponent>(e);
			RenderComponent& render = GetComponent<RenderComponent>(e);


			//#LINUS This needs to be profiled.
			CU::Matrix44f t = translation.myOrientation;
		/*	t = CU::Matrix44f::CreateRotateAroundX(CL::DegreeToRad(90.f) * render.m_Rotation.x) * t;
			t = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(90.f) * render.m_Rotation.y) * t;
			t = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(90.f) * render.m_Rotation.z) * t;*/
			t = CU::Matrix44f::CreateScaleMatrix(render.scale) * t;

			mySynchronizer->AddRenderCommand(RenderCommand(
				eType::MODEL
				, render.myModelID
				, translation.myOrientation
				, render.scale));
	}
}
