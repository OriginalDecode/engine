#include "stdafx.h"
#include "Inspector.h"

#include <Engine/Engine.h>
#include <Engine/imgui.h>

#include <Engine/InspectorView.h>
#include <Engine/PhysicsView.h>
#include <Engine/GraphicsView.h>
#include <Engine/TransformView.h>
#include <Engine/TreeDweller.h>
#include <Engine/Octree.h>
#include <Engine/NodeEntityManager.h>

#include <EntitySystem/EntityManager.h>
#include <EntitySystem/DebugComponent.h>
#include <EntitySystem/TranslationComponent.h>
#include <EntitySystem/GraphicsComponent.h>
#include <EntitySystem/NetworkComponent.h>
#include <EntitySystem/PhysicsComponent.h>
#include <EntitySystem/AIComponent.h>
#include <EntitySystem/InputComponent.h>
#include <EntitySystem/LightComponent.h>

#include <Physics/PhysicsManager.h>
#include <Physics/RigidBody.h>




Inspector::Inspector()
	: m_Manager(Engine::GetInstance()->GetEntityManager())
{
	m_Views.push_back(new TransformView(m_Manager));
	m_Views.push_back(new GraphicsView(m_Manager));
	m_Views.push_back(new PhysicsView(m_Manager));
}


Inspector::~Inspector()
{
	for (InspectorView* view : m_Views)
	{
		delete view;
	}
}


void Inspector::HandleAdd()
{
	DebugComponent& c = m_Manager.GetComponent<DebugComponent>(m_CurrentEntity);
	TreeDweller* pDweller = static_cast<TreeDweller*>(c.m_Dweller);

 	if (!(c.m_ComponentFlags & TreeDweller::GRAPHICS) && ImGui::Button("Add Graphics"))
 	{
 		c.m_ComponentFlags |= TreeDweller::GRAPHICS;
		GraphicsComponent& g = m_Manager.AddComponent<GraphicsComponent>(m_CurrentEntity);
		pDweller->AddComponent(&g, TreeDweller::LIGHT);
		pDweller->GetFirstNode()->GetManager()->AddEntity(pDweller);
 	}
 
 	if (!(c.m_ComponentFlags & TreeDweller::LIGHT) && ImGui::Button("Add Light"))
 	{
 		c.m_ComponentFlags |= TreeDweller::LIGHT;
 		LightComponent& l = m_Manager.AddComponent<LightComponent>(m_CurrentEntity);
 		pDweller->AddComponent(&l, TreeDweller::LIGHT);
 		pDweller->GetFirstNode()->GetManager()->AddEntity(pDweller);
 	}

	if(ImGui::Button("Add Physics"))
	{
		if (!(c.m_ComponentFlags & TreeDweller::PHYSICS))
		{
			c.m_ComponentFlags |= TreeDweller::PHYSICS;
			PhysicsComponent& phys = m_Manager.AddComponent<PhysicsComponent>(m_CurrentEntity);

			GraphicsComponent& g = m_Manager.GetComponent<GraphicsComponent>(m_CurrentEntity);
			TranslationComponent& translation = m_Manager.GetComponent<TranslationComponent>(m_CurrentEntity);

			phys.m_Body = Engine::GetInstance()->GetPhysicsManager()->CreateBody();


			btRigidBody* body = phys.m_Body->InitAsBox(g.m_Instances[0].m_Scale * 2.f, { 0.f,0.f,0.f });
			Engine::GetInstance()->GetPhysicsManager()->Add(body);

			phys.m_Body->SetPosition(translation.GetOrientation().GetPosition());

			pDweller->AddComponent(&phys, TreeDweller::PHYSICS);
			pDweller->GetFirstNode()->GetManager()->AddEntity(pDweller);
		}
	}
}

void Inspector::Update(float dt)
{
	const WindowSize& window_size = Engine::GetInstance()->GetInnerSize();

	ImVec2 inspector_size;
	inspector_size.x = 350;
	inspector_size.y = window_size.m_Height;

	ImGui::SetNextWindowPos(ImVec2(window_size.m_Width - 350, 0));
	ImGui::SetNextWindowSize(inspector_size);

	if (ImGui::Begin("Entity Inspector"))
	{
		if (m_CurrentEntity <= 0)
		{
			ImGui::End();
			return;
		}

		for (InspectorView* view : m_Views)
			view->Update();

		HandleAdd();

		ImGui::End();
	}

}

void Inspector::SetEntity(Entity e)
{
	m_CurrentEntity = e;
	const s32 flags = m_Manager.GetComponent<DebugComponent>(e).m_ComponentFlags;
	for (InspectorView* view : m_Views)
		view->SetEntity(m_CurrentEntity, flags);
}