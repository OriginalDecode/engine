#include "stdafx.h"
#include "RenderObject.h"
#include <Engine/Engine.h>
#include <Engine/Synchronizer.h>
#include <Engine/NodeEntityManager.h>

void RenderObject::AddRenderCommand(const ModelCommand& command, const NodeEntityManager& manager)
{
	Engine* pEngine = Engine::GetInstance();
	const u16 current_buffer = pEngine->GetSynchronizer()->GetCurrentBufferIndex();
	memory::CommandAllocator& allocator = pEngine->GetMemorySegmentHandle().GetCommandAllocator(current_buffer ^ 1, manager.GetMemoryBlockIndex());
	void * current = allocator.Alloc(sizeof(ModelCommand));
	memcpy(current, &command, sizeof(ModelCommand));
}

void RenderObject::AddRenderCommand(const LineCommand& command, const NodeEntityManager& manager)
{

}

void RenderObject::AddRenderCommand(const SpotlightCommand& command, const NodeEntityManager& manager)
{

}

void RenderObject::AddRenderCommand(const PointlightCommand& command, const NodeEntityManager& manager)
{

}
