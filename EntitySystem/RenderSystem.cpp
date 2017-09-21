#include "RenderSystem.h"

#include <Engine.h>
#include "../Engine/Model.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
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
#include <profile_defines.h>
//#define VISIBLE_CHECK

RenderSystem::RenderSystem(NodeEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<TranslationComponent, RenderComponent>>())
{
	mySynchronizer = m_Engine->GetSynchronizer();
}


void RenderSystem::Update(float /*dt*/, bool paused)
{
	PROFILE_FUNCTION(profiler::colors::Blue);
	const CU::GrowingArray<Entity>& entities = GetEntities();

	PROFILE_BLOCK("Render : Entity Loop");
	for (int i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		RenderComponent& render = GetComponent<RenderComponent>(e);

		PROFILE_BLOCK("Frustum collision check", profiler::colors::Green);
#ifdef VISIBLE_CHECK
		bool visible = false;
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
		if(!visible)
			continue;
#endif
		PROFILE_BLOCK_END;

		CU::Matrix44f t = translation.myOrientation;
		t = CU::Matrix44f::CreateScaleMatrix(render.scale) * t;

		/*if (render.myModelID.empty())
			DL_ASSERT("Empty key!");*/

		/*for (const ModelInstance& i : render.m_Instances)
		{
			CU::Matrix44f txi = t * i.m_Orientation;
			AddRenderCommand(ModelCommand(i.m_ModelID, txi, i.m_RenderWireframe));
		}
*/
		if (m_Manager.HasComponent(e, CreateFilter<Requires<PBLComponent>>()))
		{
			auto& pbl = m_Manager.GetComponent<PBLComponent>(e);
			AddRenderCommand(ModelCommand(render.m_ModelID, translation.myOrientation.GetPosition(), render.m_RenderWireframe, pbl.m_PBLValues.m_Roughness, pbl.m_PBLValues.m_Metalness));
		}


	}
	PROFILE_BLOCK_END;
}

bool RenderSystem::Inside(const CU::Vector4f& translation, const CU::Vector4f& direction, const CU::Vector4f& pos)
{
	return true;
	CU::Vector4f position = translation + (direction * pos);
	if (CameraHandle::GetInstance()->GetFrustum().Inside({ position.x, position.y, position.z }, 25.f))
		return true;
	return false;
}

void RenderSystem::AddRenderCommand(const ModelCommand& command)
{
	const u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBufferIndex();
	memory::CommandAllocator& allocator = Engine::GetInstance()->GetMemorySegmentHandle().GetCommandAllocator(current_buffer ^ 1, m_Manager.GetMemoryBlockIndex());
	void * current = allocator.Alloc(sizeof(ModelCommand));
	memcpy(current, &command, sizeof(ModelCommand));
}
