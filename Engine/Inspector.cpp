#include "stdafx.h"
#include "Inspector.h"

#include <Engine/Engine.h>
#include <Engine/imgui.h>
#include <Engine/InspectorView.h>
#include <Engine/PhysicsView.h>
#include <Engine/GraphicsView.h>

#include <EntitySystem/EntityManager.h>
#include <EntitySystem/DebugComponent.h>



Inspector::Inspector()
	: m_Manager(Engine::GetInstance()->GetEntityManager())
{
	m_Views.push_back(new PhysicsView(m_Manager));
	m_Views.push_back(new GraphicsView(m_Manager));
}


Inspector::~Inspector()
{
	for (InspectorView* view : m_Views)
	{
		delete view;
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

		for (InspectorView* view : m_Views)
			view->Update();


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