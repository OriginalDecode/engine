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
#include <Engine/Engine.h>
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
			const CU::Matrix44f& matrix = translation.myOrientation;
			const CU::Vector4f forward = matrix.GetForward();
			const CU::Vector4f right = matrix.GetRight();
			const CU::Vector4f up = matrix.GetUp();
			CU::Vector4f position = matrix.GetTranslation() + (forward * CU::Vector4f(render.m_MaxPos));

			if (CameraHandle::GetInstance()->GetFrustum().Inside({ position.x, position.y, position.z }, 25.f))
				visible = true;

			if (e == 50)
			{
				SLinePoint p[2];
				p[0].position = matrix.GetTranslation();
				p[0].position.y += 1.f;

				p[1].position = matrix.GetTranslation();
				p[1].position.y -= 1.f;
				mySynchronizer->AddRenderCommand(LineCommand(p[0], p[1], true));

			}


			


			position = matrix.GetTranslation() + (right * CU::Vector4f(render.m_MaxPos));
			if (CameraHandle::GetInstance()->GetFrustum().Inside({ position.x, position.y, position.z }, 25.f))
				visible = true;

			if (e == 3333)
			{
				SLinePoint p[2];
				p[0].position = matrix.GetTranslation();
				p[0].position.y += 1.f;

				p[1].position = matrix.GetTranslation();
				p[1].position.y -= 1.f;
				mySynchronizer->AddRenderCommand(LineCommand(p[0], p[1], true));

			}




			position = matrix.GetTranslation() + (up * CU::Vector4f(render.m_MaxPos));
			if (CameraHandle::GetInstance()->GetFrustum().Inside({ position.x, position.y, position.z }, 25.f))
				visible = true;

			if (e == 50)
			{
				SLinePoint p[2];
				p[0].position = matrix.GetTranslation();
				p[0].position.y += 1.f;

				p[1].position = matrix.GetTranslation();
				p[1].position.y -= 1.f;
				mySynchronizer->AddRenderCommand(LineCommand(p[0], p[1], true));

			}



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
