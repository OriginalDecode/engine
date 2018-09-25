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
		//world.SetPosition({ 512, 0, 512, 1 });
		return;
		for (const ModelInstance& instance : render.m_Instances)
		{

			const CU::Matrix44f relative = CU::Matrix44f::CreateScaleMatrix(instance.m_Scale) * instance.m_Orientation;
			const CU::Matrix44f orientation = world * (relative * translation.GetOrientation());

			const Frustum& f = CameraHandle::GetInstance()->GetFrustum();

			ASSERT(instance.m_ModelID > 0, "Invalid Model Key!");

			AddRenderCommand(ModelCommand(instance.m_ModelID
										  , instance.m_MaterialKey
										  , orientation
										  , render.m_RenderWireframe
#ifdef _DEBUG
										  , e));
#else 
											));
#endif
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

	//this is the slowdown, this is what is causing the start up to go so immensly slow.
#ifdef _PER_NODE_SYSTEM
	const u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBufferIndex();
	memory::CommandAllocator& allocator = Engine::GetInstance()->GetMemorySegmentHandle().GetCommandAllocator(current_buffer ^ 1, m_Manager.GetMemoryBlockIndex());
	void * current = allocator.Alloc(sizeof(ModelCommand));
	memcpy(current, &command, sizeof(ModelCommand));
#else
	Engine& engine = Engine::GetRef();
	const u16 current_buffer = engine.GetSynchronizer()->GetCurrentBufferIndex();
	memory::CommandAllocator& allocator = engine.GetMemorySegmentHandle().GetCommandAllocator(current_buffer ^ 1, 0);
	void * current = allocator.Alloc(sizeof(ModelCommand));
	memcpy(current, &command, sizeof(ModelCommand));
#endif
}