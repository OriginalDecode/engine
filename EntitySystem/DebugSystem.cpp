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
			p8.position = translation.myOrientation.GetTranslation();

			p1.position -= translation.myOrientation.GetForward() * debug.m_WHD.z;
			p8.position += translation.myOrientation.GetForward() * debug.m_WHD.z;

			p1.position -= translation.myOrientation.GetRight() * debug.m_WHD.x;
			p8.position += translation.myOrientation.GetRight() * debug.m_WHD.x;

			p1.position -= translation.myOrientation.GetUp() * debug.m_WHD.y;
			p8.position += translation.myOrientation.GetUp() * debug.m_WHD.y;

			p2.position = p1.position;
			p2.position.x = p8.position.x;
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p1, p2));

			p3.position = p8.position;
			p3.position.y = p1.position.y;
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p2, p3));

			p4.position = p1.position;
			p4.position.z = p8.position.z;
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p3, p4));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p4, p1));

			p5.position = p1.position;
			p5.position.y = p8.position.y;
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p1, p5));

			p6.position = p8.position;
			p6.position.x = p1.position.x;
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p5, p6));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p6, p8));

			p7.position = p8.position;
			p7.position.z = p1.position.z;
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p8, p7));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p7, p5));

			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p6, p4));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p7, p2));
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p8, p3));
		}
}

void DebugSystem::ReceiveMessage(const OnLeftClick& message)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();
	bool collision = false;
	for (Entity e : entities)
	{
		DebugComponent& debug = GetComponent<DebugComponent>(e);
		debug.debugColor = { 255.f,255.f,255.f,255.f };
	}
	for (Entity i = entities.Size() -1; i >= 0; i--)
	{
		Entity e = entities[i];
		DebugComponent& debug = GetComponent<DebugComponent>(e);
		for (s32 i = 0; i < 50; i++)
		{
			CU::Vector3f pos = CU::Vector3f(message.camera_pos_x, message.camera_pos_y, message.camera_pos_z);
			CU::Vector3f step = (CU::Vector3f(message.ray_dir_x, message.ray_dir_y, message.ray_dir_z) * float(i));
			CU::Vector3f new_post = pos + step;

			if (debug.m_OBB.Inside(new_post))
			{
				PostMaster::GetInstance()->SendMessage(OnLeftClick());
				collision = true;
				debug.debugColor = { 255.f,0.f,0.f,255.f };
				break;
			}

		}
		if (collision)
			break;
	}
}

