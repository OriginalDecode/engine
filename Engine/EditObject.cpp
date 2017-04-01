#include "stdafx.h"
#include "EditObject.h"

#include <LightComponent.h>
#include <RenderComponent.h>
#include <AIComponent.h>
#include <NetworkComponent.h>
#include <InputComponent.h>
#include <PhysicsComponent.h>

#include "imgui.h"

void EditObject::Initiate(Entity entity_id, s32 component_flags)
{
	m_ID = entity_id;
	m_Components = component_flags;
	m_EntityManager = &Engine::GetInstance()->GetEntityManager();
}

void EditObject::Update()
{
	if (m_Components & LIGHT)
	{
		if (ImGui::Button("Edit Light", ImVec2(m_ButtonX, m_ButtonY)))
		{
			SetComponentToEdit(EditObject::LIGHT);
		}
		EditLight();
	}

	if (m_Components & GRAPHICS)
	{
		if (ImGui::Button("Edit Graphics", ImVec2(m_ButtonX, m_ButtonY)))
		{
			SetComponentToEdit(EditObject::GRAPHICS);
		}
		EditGraphics();
	}
	
	if (m_Components & PHYSICS)
	{
		if (ImGui::Button("Edit Physics", ImVec2(m_ButtonX, m_ButtonY)))
		{
			SetComponentToEdit(EditObject::PHYSICS);
		}
		EditPhysics();
	}

	if (m_Components & AI)
	{
		if (ImGui::Button("Edit AI", ImVec2(m_ButtonX, m_ButtonY)))
		{
			SetComponentToEdit(EditObject::AI);
		}
		EditAI();
	}

	if (m_Components & INPUT)
	{
		if (ImGui::Button("Edit Input", ImVec2(m_ButtonX, m_ButtonY)))
		{
			SetComponentToEdit(EditObject::INPUT);
		}
		EditInput();
	}

	if (m_Components & NETWORK)
	{
		if (ImGui::Button("Edit Network", ImVec2(m_ButtonX, m_ButtonY)))
		{
			SetComponentToEdit(EditObject::NETWORK);
		}
		EditNetwork();
	}

}

void EditObject::SetComponentToEdit(s32 component_flag)
{
	m_EditComponent = component_flag;
}

void EditObject::EditGraphics()
{
	if (m_EditComponent & GRAPHICS)
	{
		
	}
}

void EditObject::EditLight()
{
	if (m_EditComponent & LIGHT)
	{
		std::stringstream ss;
		ss << "LightComponent" << m_ID;
		ImGui::SetNextWindowPos(ImVec2(300.f, ImGui::GetWindowSize().y));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		if (ImGui::BeginChildFrame(0, ImVec2(300.f, ImGui::GetWindowSize().y / 2)))
		{
			static float rgb[3];

			static ImVec4 r_color;
			static ImVec4 g_color;
			static ImVec4 b_color;
			static ImVec4 tot_color;

			LightComponent& l = m_EntityManager->GetComponent<LightComponent>(m_ID);
			rgb[0] = l.color.x;
			rgb[1] = l.color.y;
			rgb[2] = l.color.z;

			{
				ImGui::ColorButton(r_color);
				ImGui::SameLine();
				ImGui::SliderFloat("R", &rgb[0], 0, 1);
				r_color = ImVec4(rgb[0], 0, 0, 1);
			}

			{
				ImGui::ColorButton(g_color);
				ImGui::SameLine();
				ImGui::SliderFloat("G", &rgb[1], 0, 1);
				g_color = ImVec4(0, rgb[1], 0, 1);
			}

			{
				ImGui::ColorButton(b_color);
				ImGui::SameLine();
				ImGui::SliderFloat("B", &rgb[2], 0, 1);
				b_color = ImVec4(0, 0, rgb[2], 1);
			}

			tot_color = ImVec4(rgb[0], rgb[1], rgb[2], 1);
			ImGui::ColorButton(tot_color);


			l.color.x = rgb[0];
			l.color.y = rgb[1];
			l.color.z = rgb[2];

			ImGui::Separator();

			if (l.myType == eLightType::eSPOTLIGHT)
			{
				static float angle = 0.f;
				angle = l.angle;
				ImGui::SliderFloat("Angle", &angle, 0, 90);
				l.angle = angle;
			}

			static float range = 0.f;
			range = l.range;
			ImGui::SliderFloat("Range", &range, 0, 100);
			l.range = range;


			//ImGui::Separator();

			/*if (ImGui::Button("Done"))
			{
				m_EditLight = !m_EditLight;
			}*/

			//ImGui::End();
			ImGui::EndChildFrame();
		}
		ImGui::PopStyleVar();
	}
}

void EditObject::EditAI()
{
	if (m_EditComponent & AI)
	{

	}
}

void EditObject::EditNetwork()
{
	if (m_EditComponent & NETWORK)
	{

	}
}

void EditObject::EditInput()
{
	if (m_EditComponent & INPUT)
	{

	}
}

void EditObject::EditPhysics()
{
	if (m_EditComponent & PHYSICS)
	{

	}
}
