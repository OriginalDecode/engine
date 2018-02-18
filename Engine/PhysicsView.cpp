#include "stdafx.h"
#include "PhysicsView.h"


#include <imgui/imgui.h>
#include <Engine/TreeDweller.h>

#include <EntitySystem/PhysicsComponent.h>
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
	if (!(m_ComponentFlags & TreeDweller::PHYSICS))
		return;

	if (ImGui::CollapsingHeader("Physics"))
	{
		PhysicsComponent& phys = m_Manager.GetComponent<PhysicsComponent>(m_CurrentEntity);
		RigidBody* body = phys.m_Body;

		ImGui::Checkbox("Static", &body->IsStatic());
		if (body->IsStatic())
			body->SetStatic(true);


		ImGui::InputFloat("Mass", &body->GetMass(), 3);
		if (body->IsStatic() && body->GetMass() > 0.f)
			body->SetStatic(false);
	}
}
