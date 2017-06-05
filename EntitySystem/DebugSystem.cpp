#include "DebugSystem.h"
#include <Engine.h>
#include "DebugComponent.h"
#include "TranslationComponent.h"
#include <RenderCommand.h>
#include <Synchronizer.h>
#include <OnLeftClick.h>
#include <PostMaster.h>
#include "TypeID.h"
#include "RenderComponent.h"
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
DebugSystem::DebugSystem(EntityManager& entity_manager)
	: BaseSystem(entity_manager, CreateFilter<Requires<TranslationComponent, DebugComponent>>())
{
	m_Synchronizer = Engine::GetInstance()->GetSynchronizer();
	PostMaster::GetInstance()->Subscribe(eMessageType::ON_LEFT_CLICK, this);
	m_CurrentEntity = -1;
	m_MouseDeltaModifier = 100.f;
}

DebugSystem::~DebugSystem()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::ON_LEFT_CLICK, this);
}

void DebugSystem::Update(float /*dt*/)
{
	//m_Synchronizer->AddRenderCommand(RenderCommand(eType::TEXT, current_model, CU::Vector2f(0.75, 0)));
	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (Entity e : entities)
	{
		DebugComponent& debug = GetComponent<DebugComponent>(e);
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);


//#ifdef _EDITOR
		if (myEntityManager.HasComponent(e, CreateFilter<Requires<RenderComponent>>()))
		{
			RenderComponent& r = GetComponent<RenderComponent>(e);
			Model* m = Engine::GetInstance()->GetModel(r.myModelID);
			debug.m_MinPoint = m->GetMinPoint();//r.m_MinPos;
			debug.m_MaxPoint = m->GetMaxPoint();//r.m_MaxPos;

		}
		//RenderBox(debug, translation.myOrientation);
//#endif
	}

	if (m_Engine->GetInputHandle()->GetInputWrapper()->OnRelease(MouseInput::LEFT))
	{
		m_Holding = false;
	}

	if (m_CurrentEntity > -1)
	{
		DebugComponent& debug = GetComponent<DebugComponent>(m_CurrentEntity);
		debug.m_PositionGizmo.Render();
		debug.m_RotationGizmo.Render();
		//debug.m_ScaleGizmo.Render();
		if (m_Holding && m_Direction)
		{
			const CU::Vector2f& delta_pos = m_Engine->GetInputHandle()->GetDeltaCursorPos();

			TranslationComponent& translation = GetComponent<TranslationComponent>(m_CurrentEntity);
			CU::Vector4f position = translation.myOrientation.GetTranslation();
			CU::Vector3f position3 = translation.myOrientation.GetPosition();
			CU::Vector4f dir = m_Direction->m_Orientation.GetRight();

			CU::Vector4f cam_pos = m_Engine->GetCamera()->GetPosition();

			/*if (m_Engine->GetInputHandle()->GetInputWrapper()->OnDown(KButton::M))
			{
				if (debug.m_PositionGizmo.IsActive() || debug.m_RotationGizmo.IsActive())
				{
					debug.m_PositionGizmo.ToggleActive();
					debug.m_RotationGizmo.ToggleActive();
				}
			}*/

			if (debug.m_PositionGizmo.IsActive())
			{
				if (m_Direction->direction == GizmoHandle::eDirection::FORWARD)
				{
					CU::Vector4f horizontal_modifier = cam_pos - position;
					CU::Math::Normalize(horizontal_modifier);
					position += (dir * (delta_pos.x * horizontal_modifier.x)) / m_MouseDeltaModifier;

				}
				else if (m_Direction->direction == GizmoHandle::eDirection::RIGHT)
				{
					CU::Vector4f horizontal_modifier = cam_pos - position;
					CU::Math::Normalize(horizontal_modifier);
					position += (dir * (delta_pos.x * -horizontal_modifier.z)) / m_MouseDeltaModifier;
				}
				else if (m_Direction->direction == GizmoHandle::eDirection::UP)
				{
					position += (dir * -delta_pos.y) / m_MouseDeltaModifier;
				}
			}
			else if(debug.m_RotationGizmo.IsActive())
			{
				if (m_Direction->direction == GizmoHandle::eDirection::X)
				{
					CU::Vector4f horizontal_modifier = cam_pos - position;
					CU::Math::Normalize(horizontal_modifier);
					float angle = 0.f;
					angle += ((delta_pos.x * horizontal_modifier.x)) / m_MouseDeltaModifier;

					translation.myOrientation.RotateAroundPointX(position3, angle);
					//translation.myOrientation = CU::Matrix44f::CreateRotateAroundX(angle) * translation.myOrientation;
				}
				else if (m_Direction->direction == GizmoHandle::eDirection::Y)
				{
					CU::Vector4f horizontal_modifier = cam_pos - position;
					CU::Math::Normalize(horizontal_modifier);
					float angle = 0.f;
					angle += ( ( delta_pos.x * horizontal_modifier.x ) ) / m_MouseDeltaModifier;
					translation.myOrientation.RotateAroundPointY(position3, angle);

				}
				else if (m_Direction->direction == GizmoHandle::eDirection::Z)
				{
					CU::Vector4f horizontal_modifier = cam_pos - position;
					CU::Math::Normalize(horizontal_modifier);
					float angle = 0.f;
					angle += ( ( delta_pos.x * horizontal_modifier.x ) ) / m_MouseDeltaModifier;
					translation.myOrientation.RotateAroundPointZ(position3, angle);
				}
			}

			translation.myOrientation.SetTranslation(position);

			debug.m_DirtyFlag = true;
		}
	}
	
	//UpdateOBBs();

}

bool DebugSystem::CheckGizmoCollision(const CU::Vector3f& cam_pos, const CU::Vector3f& ray_dir)
{
	if (m_CurrentEntity < 0)
		return false;

	DebugComponent& debug = GetComponent<DebugComponent>(m_CurrentEntity);
	for (float i = 0; i < 25.f; i += 0.001f)
	{
		CU::Vector3f step = (ray_dir * i);
		CU::Vector3f new_pos = cam_pos + step;

		PositionGizmo& pos_gizmo = debug.m_PositionGizmo;
		RotationGizmo& rot_gizmo = debug.m_RotationGizmo;
		RotationGizmo& scale_gizmo = debug.m_ScaleGizmo;

		if (pos_gizmo.IsActive())
		{
			if (debug.m_PositionGizmo.GetForward().Inside(new_pos))
			{
				m_Holding = true;
				m_Direction = &debug.m_PositionGizmo.GetForward();
				return true;
			}
			else if (debug.m_PositionGizmo.GetRight().Inside(new_pos))
			{
				m_Holding = true;
				m_Direction = &debug.m_PositionGizmo.GetRight();
				return true;
			}
			else if (debug.m_PositionGizmo.GetUp().Inside(new_pos))
			{
				m_Holding = true;
				m_Direction = &debug.m_PositionGizmo.GetUp();
				return true;
			}
		}
		else if (rot_gizmo.IsActive())
		{
			if (rot_gizmo.GetForward().Inside(new_pos))
			{
				m_Holding = true;
				m_Direction = &rot_gizmo.GetForward();
				return true;
			}
			else if (rot_gizmo.GetRight().Inside(new_pos))
			{
				m_Holding = true;
				m_Direction = &rot_gizmo.GetRight();
				return true;
			}
			else if (rot_gizmo.GetUp().Inside(new_pos))
			{
				m_Holding = true;
				m_Direction = &rot_gizmo.GetUp();
				return true;
			}
		}
		else if(scale_gizmo.IsActive())
		{
			
			if (debug.m_ScaleGizmo.GetForward().Inside(new_pos))
			{
				m_Holding = true;
				m_Direction = &debug.m_ScaleGizmo.GetForward();
				return true;
			}
			else if (debug.m_ScaleGizmo.GetRight().Inside(new_pos))
			{
				m_Holding = true;
				m_Direction = &debug.m_ScaleGizmo.GetRight();
				return true;
			}
			else if (debug.m_ScaleGizmo.GetUp().Inside(new_pos))
			{
				m_Holding = true;
				m_Direction = &debug.m_ScaleGizmo.GetUp();
				return true;
			}
		}
	}
	return false;
}

void DebugSystem::UpdateOBBs()
{
	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (Entity e : entities)
	{
		DebugComponent& debug = GetComponent<DebugComponent>(e);
	

		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		CU::Vector4f up = translation.myOrientation.GetUp();
		CU::Vector4f right = translation.myOrientation.GetRight();
		CU::Vector4f forward = translation.myOrientation.GetForward();


		CU::Vector4f position;

		//x
		position = translation.myOrientation.GetTranslation();
		position += right * debug.m_MaxPoint.x;
		debug.m_OBB.m_Planes[0].InitWithPointAndNormal(position, right);

		position = translation.myOrientation.GetTranslation();
		position += right * debug.m_MinPoint.x;
		right -= (right * 2.f);
		debug.m_OBB.m_Planes[1].InitWithPointAndNormal(position, right);

		//y
		position = translation.myOrientation.GetTranslation();
		position += up * debug.m_MaxPoint.y;
		debug.m_OBB.m_Planes[2].InitWithPointAndNormal(position, up);

		position = translation.myOrientation.GetTranslation();
		position += up * debug.m_MinPoint.y;
		up -= (up * 2.f);
		debug.m_OBB.m_Planes[3].InitWithPointAndNormal(position, up);

		//z
		position = translation.myOrientation.GetTranslation();
		position += forward * debug.m_MaxPoint.z;
		debug.m_OBB.m_Planes[4].InitWithPointAndNormal(position, forward);


		position = translation.myOrientation.GetTranslation();
		position += forward * debug.m_MinPoint.z;
		forward -= (forward * 2.f);
		debug.m_OBB.m_Planes[5].InitWithPointAndNormal(position, forward);

		debug.m_PositionGizmo.SetPosition(translation.myOrientation.GetPosition());
		debug.m_PositionGizmo.Update();
		debug.m_RotationGizmo.SetPosition(translation.myOrientation.GetPosition());

		CU::Vector3f up_pos = translation.myOrientation.GetPosition();
		up_pos.x += 0.5f;
		up_pos.y -= 0.5f;
		up_pos.z += 0.5f;
		debug.m_RotationGizmo.GetUp().SetPosition(up_pos);

		debug.m_RotationGizmo.Update();
	}

}

//This needs to be optimized as hell.
void DebugSystem::ReceiveMessage(const OnLeftClick& message)
{
	UpdateOBBs();



	CU::Vector3f cam_pos = CU::Vector3f(message.camera_pos_x, message.camera_pos_y, message.camera_pos_z);
	CU::Vector3f ray_dir = CU::Vector3f(message.ray_dir_x, message.ray_dir_y, message.ray_dir_z);
	
	
	if ( CheckGizmoCollision(cam_pos, ray_dir) )
		return;
	//Should be optimized for a quad/oct -tree solution to only retrieve the entities in THIS part
	NodeEntityManager& node_manager = message.m_Player->GetFirstNode()->GetManager();
	const EntityArray& entities = node_manager.GetEntities(myFilter);


	//const CU::GrowingArray<Entity>& entities = GetEntities();
	CU::GrowingArray<entity_collisions> collisions;
	for (Entity i = entities.Size() - 1; i >= 0; i--)
	{
		Entity e = entities[i];
		DebugComponent& debug = GetComponent<DebugComponent>(e);
		debug.debugColor = { 255.f,255.f,255.f,255.f };



		for (float j = 0; j < 25.f; j += 0.05f)
		{
			CU::Vector3f step = (ray_dir * j);
			CU::Vector3f new_pos = cam_pos + step;

			if (debug.m_OBB.Inside(new_pos))
			{
				entity_collisions collision;
				collision.m_ID = e;
				collision.m_Position = new_pos;
				collisions.Add(collision);
				break;
			}
		}
	}

	float prev_length = FLT_MAX;
	entity_collisions closest;
	Entity prev_entity = -1;
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
		Engine::GetInstance()->SelectEntity(closest.m_ID);
		m_PrevID = prev_entity;
		m_CurrentEntity = m_PrevID;
		bool has_render = node_manager.HasComponent(m_CurrentEntity, CreateFilter<Requires<RenderComponent>>());
		if ( has_render )
		{
			RenderComponent& r = node_manager.GetComponent<RenderComponent>(m_CurrentEntity);
			current_model = r.myModelID;
		}
	}
}

void DebugSystem::RenderBox(const DebugComponent& component, const CU::Matrix44f& orientation)
{
	SLinePoint p1, p2, p3, p4, p5, p6, p7, p8;
	p1.color = component.debugColor;
	p2.color = p1.color;
	p3.color = p1.color;
	p4.color = p1.color;
	p5.color = p1.color;
	p6.color = p1.color;
	p7.color = p1.color;
	p8.color = p1.color;


	//p1.position = orientation.GetTranslation();
	//p1.position.y += 2.f;
	//p1.color = { 255.f, 0.f, 255.f, 255.f };

	//p2.position = orientation.GetTranslation();
	//p2.position.y -= 2.f;
	//p2.color = { 0.f, 255.f, 255.f, 255.f };

	//m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, p1, p2));

	//return;

	p1.position = orientation.GetTranslation(); // translation.myOrientation.GetTranslation();
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


	/*if ( component.m_MinPoint.x < -0.f )
	{
		p1.color = CU::Vector4f(255.f,0.f,0.f,255.f);
		p2.color = p1.color;
		p3.color = p1.color;
		p4.color = p1.color;
		p5.color = p1.color;
		p6.color = p1.color;
		p7.color = p1.color;
		p8.color = p1.color;
	}*/

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

	/*p1.position -= orientation.GetRight() * component.m_WHD.x;
	p1.position -= orientation.GetUp() * component.m_WHD.y;
	p1.position -= orientation.GetForward() * component.m_WHD.z;

	p2.position += orientation.GetRight() * component.m_WHD.x;
	p2.position -= orientation.GetUp() * component.m_WHD.y;
	p2.position -= orientation.GetForward() * component.m_WHD.z;

	p3.position += orientation.GetRight() * component.m_WHD.x;
	p3.position -= orientation.GetUp() * component.m_WHD.y;
	p3.position += orientation.GetForward() * component.m_WHD.z;

	p4.position -= orientation.GetRight() * component.m_WHD.x;
	p4.position -= orientation.GetUp() * component.m_WHD.y;
	p4.position += orientation.GetForward() * component.m_WHD.z;

	p5.position -= orientation.GetRight() * component.m_WHD.x;
	p5.position += orientation.GetUp() * component.m_WHD.y;
	p5.position -= orientation.GetForward() * component.m_WHD.z;

	p6.position -= orientation.GetRight() * component.m_WHD.x;
	p6.position += orientation.GetUp() * component.m_WHD.y;
	p6.position += orientation.GetForward() * component.m_WHD.z;

	p7.position += orientation.GetRight() * component.m_WHD.x;
	p7.position += orientation.GetUp() * component.m_WHD.y;
	p7.position -= orientation.GetForward() * component.m_WHD.z;

	p8.position += orientation.GetRight() * component.m_WHD.x;
	p8.position += orientation.GetUp() * component.m_WHD.y;
	p8.position += orientation.GetForward() * component.m_WHD.z;*/


	
	m_Synchronizer->AddRenderCommand(LineCommand(p1, p2, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p2, p3, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p3, p4, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p4, p1, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p1, p5, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p5, p6, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p6, p8, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p8, p7, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p7, p5, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p6, p4, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p7, p2, true));
	m_Synchronizer->AddRenderCommand(LineCommand(p8, p3, true));
	



}

