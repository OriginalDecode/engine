#include "DebugSystem.h"
#include "Engine.h"
#include "DebugComponent.h"
#include "TranslationComponent.h"
#include <RenderCommand.h>
#include <Synchronizer.h>
#include <OnLeftClick.h>
#include <PostMaster.h>
DebugSystem::DebugSystem(EntityManager& entity_manager)
	: BaseSystem(entity_manager, CreateFilter<Requires<TranslationComponent, DebugComponent>>())
{
	m_Synchronizer = Hex::Engine::GetInstance()->GetSynchronizer();
	PostMaster::GetInstance()->Subscribe(eMessageType::ON_LEFT_CLICK, this);
}

DebugSystem::~DebugSystem()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::ON_LEFT_CLICK, this);
}

void DebugSystem::Update(float dt)
{
		const CU::GrowingArray<Entity>& entities = GetEntities();
		for (Entity e : entities)
		{
			DebugComponent& debug = GetComponent<DebugComponent>(e);
			TranslationComponent& translation = GetComponent<TranslationComponent>(e);
			
			SLinePoint p1, p2, p3, p4, p5, p6, p7, p8;
			p1.color = debug.debugColor;
			p2.color = p1.color;
			p3.color = p1.color;
			p4.color = p1.color;
			p5.color = p1.color;
			p6.color = p1.color;
			p7.color = p1.color;
			p8.color = p1.color;

			p1.position = translation.myOrientation.GetTranslation();
			p2.position = p1.position;
			p3.position = p1.position;
			p4.position = p1.position;
			p5.position = p1.position;
			p6.position = p1.position;
			p7.position = p1.position;
			p8.position = p1.position;

			p1.position -= translation.myOrientation.GetRight() * debug.m_WHD.x;
			p1.position -= translation.myOrientation.GetUp() * debug.m_WHD.y;
			p1.position -= translation.myOrientation.GetForward() * debug.m_WHD.z;

			p2.position += translation.myOrientation.GetRight() * debug.m_WHD.x;
			p2.position -= translation.myOrientation.GetUp() * debug.m_WHD.y;
			p2.position -= translation.myOrientation.GetForward() * debug.m_WHD.z;

			p3.position += translation.myOrientation.GetRight() * debug.m_WHD.x;
			p3.position -= translation.myOrientation.GetUp() * debug.m_WHD.y;
			p3.position += translation.myOrientation.GetForward() * debug.m_WHD.z;

			p4.position -= translation.myOrientation.GetRight() * debug.m_WHD.x;
			p4.position -= translation.myOrientation.GetUp() * debug.m_WHD.y;
			p4.position += translation.myOrientation.GetForward() * debug.m_WHD.z;

			p5.position -= translation.myOrientation.GetRight() * debug.m_WHD.x;
			p5.position += translation.myOrientation.GetUp() * debug.m_WHD.y;
			p5.position -= translation.myOrientation.GetForward() * debug.m_WHD.z;

			p6.position -= translation.myOrientation.GetRight() * debug.m_WHD.x;
			p6.position += translation.myOrientation.GetUp() * debug.m_WHD.y;
			p6.position += translation.myOrientation.GetForward() * debug.m_WHD.z;

			p7.position += translation.myOrientation.GetRight() * debug.m_WHD.x;
			p7.position += translation.myOrientation.GetUp() * debug.m_WHD.y;
			p7.position -= translation.myOrientation.GetForward() * debug.m_WHD.z;

			p8.position += translation.myOrientation.GetRight() * debug.m_WHD.x;
			p8.position += translation.myOrientation.GetUp() * debug.m_WHD.y;
			p8.position += translation.myOrientation.GetForward() * debug.m_WHD.z;

			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p1, p2));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p2, p3));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p3, p4));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p4, p1));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p1, p5));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p5, p6));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p6, p8));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p8, p7));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p7, p5));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p6, p4));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p7, p2));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p8, p3));
		}
}

//This needs to be optimized as hell.
void DebugSystem::ReceiveMessage(const OnLeftClick& message)
{
	CU::Vector3f cam_pos = CU::Vector3f(message.camera_pos_x, message.camera_pos_y, message.camera_pos_z);
	//Should be optimized for a quad/oct -tree solution to only retrieve the entities in THIS part
	const CU::GrowingArray<Entity>& entities = GetEntities();
	CU::GrowingArray<entity_collisions> collisions;
	for (Entity i = entities.Size() -1; i >= 0; i--)
	{
		Entity e = entities[i];
		DebugComponent& debug = GetComponent<DebugComponent>(e);
		debug.debugColor = { 255.f,255.f,255.f,255.f };
		for (float i = 0; i < 25.f; i += 0.2f)
		{
			CU::Vector3f step = (CU::Vector3f(message.ray_dir_x, message.ray_dir_y, message.ray_dir_z) * i);
			CU::Vector3f new_post = cam_pos + step;

			if (debug.m_OBB.Inside(new_post))
			{
				PostMaster::GetInstance()->SendMessage(OnLeftClick());

				entity_collisions collision;
				collision.m_ID = e;
				collision.m_Position = new_post;
				collisions.Add(collision);
				break;
				//debug.debugColor = { 255.f,0.f,0.f,255.f };
			}
		}
	}

	float prev_length = FLT_MAX;
	entity_collisions closest;
	Entity prev_entity = 0;
	for (const entity_collisions& collision : collisions)
	{
		float new_length = CU::Math::Length2(collision.m_Position - cam_pos);
		if (new_length < prev_length)
		{
			prev_length = new_length;
			closest = collision;
			prev_entity = closest.m_ID;
		}
	}
	if (prev_entity == closest.m_ID)
	{
		DebugComponent& debug = GetComponent<DebugComponent>(closest.m_ID);
		debug.debugColor = { 255.f,0.f,0.f,255.f };
	}
}

