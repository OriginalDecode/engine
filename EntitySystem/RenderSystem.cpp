#include "RenderSystem.h"

#include "GraphicsComponent.h"
#include "../Engine/Model.h"
#include "TranslationComponent.h"
#include "LightComponent.h"
#include "PBLComponent.h"

/* Engine Includes */
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "AABBComponent.h"
#include <Utilities.h>

#include "../Engine/Engine.h"

#include "../Application/CameraHandle.h"
#include "DebugComponent.h"
#include <Engine/Engine.h>
#include "../Engine/profile_defines.h"
#include <Engine.h>

//#define VISIBLE_CHECK
RenderSystem::RenderSystem(_EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<TranslationComponent, GraphicsComponent>>())
{
	mySynchronizer = m_Engine->GetSynchronizer();
}


void RenderSystem::Update(float /*dt*/, bool paused)
{
	PROFILE_FUNCTION(profiler::colors::Blue);
	Synchronizer* sync = Engine::GetInstance()->GetSynchronizer();

	const CU::GrowingArray<Entity>& entities = GetEntities();
	PROFILE_BLOCK("Render : Entity Loop");
	for (Entity e : entities)
	{
		TranslationComponent translation = GetComponent<TranslationComponent>(e);
		const GraphicsComponent& render = GetComponent<GraphicsComponent>(e);

#ifdef VISIBLE_CHECK
		PROFILE_BLOCK("Frustum collision check", profiler::colors::Green);
		bool visible = false;
		const CU::Matrix44f& matrix = translation.m_Orientation;
		const CU::Vector4f forward = matrix.GetForward();
		const CU::Vector4f right = matrix.GetRight();
		const CU::Vector4f up = matrix.GetUp();
		const CU::Vector4f translation0 = matrix.GetTranslation();


		/* could be improved still, take a matrice instead? */
		if (Inside(translation0, up, render.m_MaxPos))
			visible = true;

		if (Inside(translation0, right, render.m_MaxPos))
			visible = true;

		if (Inside(translation0, forward, render.m_MaxPos))
			visible = true;

		if (Inside(translation0, up, render.m_MinPos))
			visible = true;

		if (Inside(translation0, right, render.m_MinPos))
			visible = true;

		if (Inside(translation0, forward, render.m_MinPos))
			visible = true;

		if (!visible)
			continue;
		PROFILE_BLOCK_END;
#endif

		CU::Matrix44f world;
		for (const ModelInstanceCmpt& instance : render.m_Instances)
		{
			break;
		}
	}
	PROFILE_BLOCK_END;
}

bool RenderSystem::Inside(const CU::Vector4f& translation, const CU::Vector4f& direction, const CU::Vector4f& pos)
{
	CU::Vector4f position = translation + (direction * pos);
	if (CameraHandle::GetInstance()->GetFrustum().Inside({ position.x, position.y, position.z }, 25.f))
		return true;
	return true;
}
