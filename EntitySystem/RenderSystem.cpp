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
RenderSystem::RenderSystem(NodeEntityManager& anEntityManager)
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
		if (m_Manager.HasComponent(e, CreateFilter<Requires<DebugComponent>>()))
		{
			DebugComponent& debug = GetComponent<DebugComponent>(e);
			const CU::Matrix44f& matrix = translation.myOrientation;
			const CU::Vector4f forward = matrix.GetForward();
			const CU::Vector4f right = matrix.GetRight();
			const CU::Vector4f up = matrix.GetUp();
			const CU::Vector4f translation = matrix.GetTranslation();


			/* could be improved still, take a matrice instead? */
			if (Inside(translation, up, render.m_MaxPos))
				visible = true;

			if (Inside(translation, right, render.m_MaxPos))
				visible = true;

			if (Inside(translation, forward, render.m_MaxPos))
				visible = true;

			if (Inside(translation, up, render.m_MinPos))
				visible = true;

			if (Inside(translation, right, render.m_MinPos))
				visible = true;

			if (Inside(translation, forward, render.m_MinPos))
				visible = true;

		}

#ifdef _PROFILE
		EASY_END_BLOCK;
#endif

		if(!visible)
			continue;

		CU::Matrix44f t = translation.myOrientation;
		t = CU::Matrix44f::CreateScaleMatrix(render.scale) * t;

		if (render.myModelID.empty())
			DL_ASSERT("Empty key!");

		mySynchronizer->AddRenderCommand(ModelCommand(render.myModelID, t, render.m_RenderWireframe));
	}
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif
}

bool RenderSystem::Inside(const CU::Vector4f& translation, const CU::Vector4f& direction, const CU::Vector4f& pos)
{
	CU::Vector4f position = translation + (direction * pos);
	if (CameraHandle::GetInstance()->GetFrustum().Inside({ position.x, position.y, position.z }, 25.f))
		return true;
	return false;
}
