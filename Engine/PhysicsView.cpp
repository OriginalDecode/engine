#include "stdafx.h"
#include "PhysicsView.h"


#include <Engine/imgui.h>
#include <Engine/TreeDweller.h>

#include <EntitySystem/PhysicsComponent.h>
#include <EntitySystem/EntityManager.h>

#include <Physics/RigidBody.h>


PhysicsView::PhysicsView(EntityManager& em)
	: InspectorView(em)
{
}

PhysicsView::~PhysicsView()
{
}

void PhysicsView::Update()
{
	if (m_ComponentFlags & (~TreeDweller::PHYSICS))
		return;

	if (ImGui::CollapsingHeader("Physics"))
	{
		PhysicsComponent& phys = m_Manager.GetComponent<PhysicsComponent>(m_CurrentEntity);
		RigidBody* body = phys.m_Body;
		static bool is_static = body->IsStatic();
		ImGui::Checkbox("Static", &is_static);
		body->SetStatic(is_static);
	}


}
