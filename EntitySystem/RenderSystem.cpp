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

#include "../Application/CameraHandle.h"
#include "DebugComponent.h"
#ifdef _PROFILE
#include <easy/profiler.h>
#endif
RenderSystem::RenderSystem(EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<TranslationComponent, RenderComponent>>())
{
	mySynchronizer = m_Engine->GetSynchronizer();
}


void RenderSystem::Update(float /*dt*/)
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Blue);
#endif
	const CU::GrowingArray<Entity>& entities = GetEntities();
#ifdef _PROFILE
	EASY_BLOCK("Render : Entity Loop");
#endif
	for (int i = 0; i < entities.Size(); i++)
	{
		bool visible = false;
		Entity e = entities[i];
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		RenderComponent& render = GetComponent<RenderComponent>(e);

#ifdef _PROFILE
		EASY_BLOCK("Frustum collision check", profiler::colors::Green);
#endif
		if (myEntityManager.HasComponent(e, CreateFilter<Requires<DebugComponent>>()))
		{
			DebugComponent& debug = GetComponent<DebugComponent>(e);
			CU::Vector3f position = translation.myOrientation.GetPosition();
			if (CameraHandle::GetInstance()->GetFrustum().Inside(position, 25.f))
					visible = true;

			position.z += render.m_MaxPos.z;
			if (CameraHandle::GetInstance()->GetFrustum().Inside(position, 25.f))
				visible = true;

			position.x += render.m_MaxPos.x;
			if (CameraHandle::GetInstance()->GetFrustum().Inside(position, 25.f))
				visible = true;

			position = translation.myOrientation.GetPosition();
			position.x += render.m_MaxPos.x;
			if (CameraHandle::GetInstance()->GetFrustum().Inside(position, 25.f))
				visible = true;
		}

		if(!visible)
			continue;

#ifdef _PROFILE
		EASY_END_BLOCK;
		EASY_BLOCK("RenderSystem : Entity Scale multiplication");
#endif
		CU::Matrix44f t = translation.myOrientation;
		t = CU::Matrix44f::CreateScaleMatrix(render.scale) * t;
#ifdef _PROFILE
		EASY_END_BLOCK;
#endif
		mySynchronizer->AddRenderCommand(ModelCommand(render.myModelID, t, render.m_RenderWireframe));

		//if (e == 0)
		{
			/**
			CU::Vector3f pos = { 256.f, 5.f, 256.f };
			CU::Vector3f original_pos = translation.myOrientation.GetPosition();
			translation.myOrientation.SetPosition(original_pos - pos);

			translation.myOrientation = translation.myOrientation * CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(25.f) * dt);
			translation.myOrientation.SetPosition(translation.myOrientation.GetPosition() + pos);
			/**/

		}
	}
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif
}
