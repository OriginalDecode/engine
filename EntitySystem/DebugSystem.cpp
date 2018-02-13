#include "DebugSystem.h"
#include "DebugComponent.h"
#include "TranslationComponent.h"
#include <RenderCommand.h>
#include <Synchronizer.h>
#include <OnLeftClick.h>
#include <EventManager.h>
#include "TypeID.h"
#include "GraphicsComponent.h"
#include "EntityManager.h"
#include <Input/InputHandle.h>
#include <Input/InputWrapper.h>
#include <Camera.h>
#include "../Application/CameraHandle.h"
#include "NodeEntityManager.h"

#include <TreeNode.h>
#include <TreeDweller.h>
#include <GizmoBase.h>
#include <Engine/Model.h>
#include <CommonLib/DataStructures/Hashmap/Hash.h>
#include "profile_defines.h"
#include <Engine.h>

#if !defined(_FINAL) && !defined(_PROFILE)

DebugSystem::DebugSystem(NodeEntityManager& entity_manager)
	: BaseSystem(entity_manager, CreateFilter<Requires<TranslationComponent, DebugComponent>>())
{
	m_Synchronizer = Engine::GetInstance()->GetSynchronizer();
	EventManager::GetInstance()->Subscribe(eMessageType::ON_LEFT_CLICK, this);
	EventManager::GetInstance()->Subscribe("pick_entity", this);
	//PostMaster::GetInstance()->Subscribe("left_click", this);
	m_CurrentEntity = -1;
	m_MouseDeltaModifier = 100.f;
}

DebugSystem::~DebugSystem()
{
	EventManager::GetInstance()->UnSubscribe(eMessageType::ON_LEFT_CLICK, this);
}

void DebugSystem::Update(float /*dt*/, bool)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();
	if (entities.Empty())
		return;

	for (s32 i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		DebugComponent& debug = GetComponent<DebugComponent>(e);
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		RenderBox(debug, translation.m_Orientation);
	}

	return;	
}

void DebugSystem::HandleEvent(u64 event, void* data /*= nullptr*/)
{
	return;
	if (event == Hash("pick_entity"))
	{
		debug::DebugHandle::GetInstance()->ConfirmEntity();
		m_Engine->PickEntity();
		Entity e = debug::DebugHandle::GetInstance()->GetSelectedEntity();
		const CU::GrowingArray<Entity>& entities = GetEntities();
		if (entities.Empty())
			return;
// 		if (entities.Find(e))
// 		{
// 			TranslationComponent& translation = GetComponent<TranslationComponent>(e);
// 			debug::DebugHandle::GetInstance()->SetObjectMatrix(&translation.m_Orientation);
// 		}
	}
}

void DebugSystem::ReceiveMessage(const OnLeftClick& message)
{
	debug::DebugHandle::GetInstance()->ConfirmEntity();
	m_Engine->PickEntity();
}

void DebugSystem::RenderBox(const DebugComponent& component, const CU::Matrix44f& orientation)
{
	LinePoint p1, p2, p3, p4, p5, p6, p7, p8;
	p1.color = component.debugColor;
	p2.color = p1.color;
	p3.color = p1.color;
	p4.color = p1.color;
	p5.color = p1.color;
	p6.color = p1.color;
	p7.color = p1.color;
	p8.color = p1.color;

	p1.position = orientation.GetTranslation(); 
	p2.position = p1.position;
	p3.position = p1.position;
	p4.position = p1.position;
	p5.position = p1.position;
	p6.position = p1.position;
	p7.position = p1.position;
	p8.position = p1.position;

	const CU::Vector4f right = orientation.GetRight();
	const CU::Vector4f up = orientation.GetUp();
	const CU::Vector4f forward = orientation.GetForward();

	p1.position += right * component.m_MinPoint.x;
	p1.position += up * component.m_MinPoint.y;
	p1.position += forward * component.m_MinPoint.z;

	p2.position += right * component.m_MaxPoint.x;
	p2.position += up * component.m_MinPoint.y;
	p2.position += forward * component.m_MinPoint.z;

	p3.position += right * component.m_MaxPoint.x;
	p3.position += up * component.m_MinPoint.y;
	p3.position += forward * component.m_MaxPoint.z;

	p4.position += right * component.m_MinPoint.x;
	p4.position += up * component.m_MinPoint.y;
	p4.position += forward * component.m_MaxPoint.z;

	p5.position += right * component.m_MinPoint.x;
	p5.position += up * component.m_MaxPoint.y;
	p5.position += forward * component.m_MinPoint.z;

	p6.position += right * component.m_MinPoint.x;
	p6.position += up * component.m_MaxPoint.y;
	p6.position += forward * component.m_MaxPoint.z;

	p7.position += right * component.m_MaxPoint.x;
	p7.position += up * component.m_MaxPoint.y;
	p7.position += forward * component.m_MinPoint.z;

	p8.position += right * component.m_MaxPoint.x;
	p8.position += up * component.m_MaxPoint.y;
	p8.position += forward * component.m_MaxPoint.z;
	
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p1, p2), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p2, p3), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p3, p4), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p4, p1), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p1, p5), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p5, p6), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p6, p8), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p8, p7), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p7, p5), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p6, p4), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p7, p2), false));
	m_Synchronizer->AddRenderCommand(LineCommand(Line(p8, p3), false));
}

#endif