#include "stdafx.h"
#if !defined(_PROFILE) && !defined(_FINAL)
#include "DebugHandle.h"

#include <Engine/Texture.h>
#include <Engine/Engine.h>
#include <Engine/Synchronizer.h>
#include <Engine/LevelFactory.h>
#include <Engine/Renderer.h>
#include <Engine/PostProcessManager.h>
#include <Engine/HDRPass.h>

#include <Input/InputHandle.h>

#include <EntitySystem/EntityManager.h>
#include <EntitySystem/ComponentFilter.h>

#include <Physics/RigidBody.h>
#include <Physics/PhysicsManager.h>

#include "NodeEntityManager.h"

#include <vector>

#include "imgui.h"
#include "ImGuizmo.h"
#include <PostMaster/EventManager.h>
#include <hash/DebugEvents.h>
#include <network/NetworkManager.h>

namespace debug
{
	static s32 s_MaterialIndex = 0;

	static auto GetVector = [](void* vec, int index, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (index < 0 || index >= static_cast<int>(vector.size()))
			return false;
		*out_text = vector.at(index).c_str();

		return true;
	};

	bool ListBox(const char* label, int* current_index, std::vector<std::string>& values)
	{
		if (values.empty())
			return false;

		return ImGui::ListBox(label, current_index, GetVector, static_cast<void*>(&values), values.size(), values.size());
	}



	DebugHandle* DebugHandle::m_Instance = nullptr;
	bool DebugHandle::s_PausePhysics = false;
	DebugHandle::DebugHandle()
	{
		EventManager* mgr = EventManager::GetInstance();
		mgr->Subscribe("create_entity", this);
		mgr->Subscribe(DebugEvents_OnRightClick, this);
		mgr->Subscribe("copy_selected", this);
		mgr->Subscribe("paste_new", this);
		m_RegisteredSampleTextures.Init(32);
	}



	static bool sDebugTextures = false;
	static bool s_RightClicked = false;
	static bool s_OpenMenu = false;
	static CU::Vector3f s_CreatePosition = CU::Vector3f(0, 0, 0);



	void HandleWorldContextMenu(Engine* pEngine);
	void CopyEntity(Entity m_EditEntity);
	void PasteEntity();

	void HandleInspector(Entity m_EditEntity, CU::Matrix44f* m_ObjectMatrix, EntityManager &em)
	{
		const WindowSize& window_size = Engine::GetInstance()->GetInnerSize();

		ImVec2 inspector_size;
		inspector_size.x = 350;
		inspector_size.y = window_size.m_Height;



		ImGui::SetNextWindowPos(ImVec2(window_size.m_Width - 350, 0));
		ImGui::SetNextWindowSize(inspector_size);

		if (ImGui::Begin("Entity Inspector"))
		{

			ImGui::Text("Entity %d", m_EditEntity);
			static bool cam_attach = false;

			if (!cam_attach && ImGui::Button("Attatch Camera"))
				cam_attach = true;
			else if (ImGui::Button("Detach Camera"))
				cam_attach = false;



			Camera* cam = Engine::GetInstance()->GetCamera();
			if (cam_attach)
			{

				TranslationComponent& t = Engine::GetInstance()->GetEntityManager().GetComponent<TranslationComponent>(m_EditEntity);
				cam->SetPosition2(t.GetOrientation().GetPosition() + CU::Vector3f(10.f, 10.f, 10.f));

			}


			CU::Matrix44f& orientation = CU::Math::Inverse(cam->GetOrientation());
			CU::Matrix44f& perspective = cam->GetPerspective();
			CU::Matrix44f& object_matrix = *m_ObjectMatrix;

			GraphicsComponent& g = em.GetComponent<GraphicsComponent>(m_EditEntity);


			constexpr int t_key = 'T';
			static bool bToggle = false;
			if (ImGui::IsKeyPressed(t_key))
				bToggle = !bToggle;

// 			if (m_ObjectMatrix && !bToggle)
// 				EditTransform(orientation.myMatrix, perspective.myMatrix, m_ObjectMatrix->myMatrix);
// 
// 			if (bToggle)
// 				EditTransform(orientation.myMatrix, perspective.myMatrix, g.m_Instances[0].m_Orientation.myMatrix);

// 			if (em.HasComponent<PhysicsComponent>(m_EditEntity))
// 			{
// 				PhysicsComponent& phys = em.GetComponent<PhysicsComponent>(m_EditEntity);
// 				if (ImGuizmo::IsUsing())
// 					phys.m_Body->SetPosition(m_ObjectMatrix->GetPosition());
// 
// 				const CU::Vector3f linVel = phys.m_Body->GetLinearVelocity();
// 				ImGui::Text("Linear Velocity\nx:%.1f\ny:%.1f\nz:%.1f", linVel.x, linVel.y, linVel.z);
// 			}

			ImGui::Separator();

			DebugComponent& c = em.GetComponent<DebugComponent>(m_EditEntity);


			if (c.m_ComponentFlags & TreeDweller::LIGHT)
			{
				if (ImGui::CollapsingHeader("Edit Light"))
				{
					ImGui::BulletText("text");
					LightComponent& l = em.GetComponent<LightComponent>(m_EditEntity);
					static int type = (int)l.myType;
					ImGui::InputInt("lighttype", &type);
					l.myType = (eLightType)type;

					ImGui::InputFloat("Intensity", &l.intensity);
					ImGui::InputFloat("Range", &l.range);
					static float half_angle = cl::RadToDegree(l.angle * 2);
					ImGui::InputFloat("Angle", &half_angle);
					l.angle = cl::DegreeToRad(half_angle * 0.5f);

					static float rgb[3];
					rgb[0] = l.color.x;
					rgb[1] = l.color.y;
					rgb[2] = l.color.z;

					ImGui::ColorEdit3("Color", rgb);
					l.color = { rgb[0], rgb[1], rgb[2] };

				}
			}
			if (c.m_ComponentFlags & TreeDweller::GRAPHICS)
			{
				if (ImGui::CollapsingHeader("Edit Graphics"))
				{

				}
			}

			if (c.m_ComponentFlags & TreeDweller::PHYSICS)
			{
				if (ImGui::CollapsingHeader("Physics"))
				{
					PhysicsComponent& phys = em.GetComponent<PhysicsComponent>(m_EditEntity);
					RigidBody* body = phys.m_Body;
					static bool is_static = body->IsStatic();
					ImGui::Checkbox("Static", &is_static);
					body->SetStatic(is_static);
				}
			}




			static bool _open = false;
			if (ImGui::Button("Add Component"))
				_open = !_open;

			if (_open && ImGui::Begin(""))
			{

				if (c.m_ComponentFlags & (~TreeDweller::GRAPHICS) && ImGui::Button("Add Graphics"))
				{
					c.m_ComponentFlags |= TreeDweller::GRAPHICS;
					em.AddComponent<GraphicsComponent>(m_EditEntity);
				}

				if (!(c.m_ComponentFlags & TreeDweller::LIGHT) && ImGui::Button("Add Light"))
				{
					c.m_ComponentFlags |= TreeDweller::LIGHT;
					LightComponent& l = em.AddComponent<LightComponent>(m_EditEntity);
					TreeDweller* pDweller = static_cast<TreeDweller*>(c.m_Dweller);
					pDweller->AddComponent(&l, TreeDweller::LIGHT);
					pDweller->GetFirstNode()->GetManager()->AddEntity(pDweller);
				}

				if (c.m_ComponentFlags & (~TreeDweller::PHYSICS) && ImGui::Button("Add Physics"))
				{
					c.m_ComponentFlags |= TreeDweller::PHYSICS;
					PhysicsComponent& phys = em.AddComponent<PhysicsComponent>(m_EditEntity);
					phys.m_Body = Engine::GetInstance()->GetPhysicsManager()->CreateBody();

					Model* pModel = Engine::GetInstance()->GetModel<Model>(g_DefaultModel).GetData();
					pModel = pModel->GetChildModels()[0];
					btRigidBody* body = phys.m_Body->InitAsBox(0.5, 0.5, 0.5, { 0.f,0.f,0.f });
					Engine::GetInstance()->GetPhysicsManager()->Add(body);

					TranslationComponent& translation = em.GetComponent<TranslationComponent>(m_EditEntity);
					phys.m_Body->SetPosition(translation.GetOrientation().GetPosition());

					TreeDweller* pDweller = static_cast<TreeDweller*>(c.m_Dweller);
					pDweller->AddComponent(&phys, TreeDweller::PHYSICS);
					pDweller->GetFirstNode()->GetManager()->AddEntity(pDweller);

				}

				if (ImGui::Button("Add AI"))
				{
					c.m_ComponentFlags |= TreeDweller::AI;
					em.AddComponent<AIComponent>(m_EditEntity);
				}

				if (ImGui::Button("Add Network")) //This will just indicate that the object will be synchronized
				{
					c.m_ComponentFlags |= TreeDweller::NETWORK;
					em.AddComponent<NetworkComponent>(m_EditEntity);
				}

				ImGui::End();
			}


			ImGui::End();
		}
	}

	void DebugHandle::Update()
	{
		Engine* pEngine = Engine::GetInstance();
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(300, Engine::GetInstance()->GetInnerSize().m_Height));
		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoTitleBar;
		flags |= ImGuiWindowFlags_NoResize;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		static bool s_Open = false;
		if (ImGui::Begin("Information", &s_Open, flags))
		{


			ImGui::Text("Delta Time : %.3f", pEngine->GetDeltaTime());
			ImGui::Text("FPS : %.1f", pEngine->GetFPS());
			ImGui::Text("CPU Usage : %.1f", pEngine->m_SystemMonitor.GetCPUUsage());
			ImGui::Text("Memory Usage : %dmb", pEngine->m_SystemMonitor.GetMemoryUsage());
			ImGui::Text("Model Commands : %d", pEngine->m_SegmentHandle.CommandSize((s32)pEngine->m_Synchronizer->GetCurrentBufferIndex()));
			ImGui::Text("Spotlight Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::SPOTLIGHT_BUFFER).Size());
			ImGui::Text("Pointlight Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::POINTLIGHT_BUFFER).Size());
			ImGui::Text("Particle Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::PARTICLE_BUFFER).Size());
			ImGui::Text("Sprite Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::SPRITE_BUFFER).Size());
			ImGui::Text("Text Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::TEXT_BUFFER).Size());
			ImGui::Text("Line Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::LINE_BUFFER).Size());

			ImGui::Separator();

			ImGui::Text("Light Direction");
			static float light_dir[3];
			ImGui::SliderFloat("X", &light_dir[0], 0.f, 180.f);
			ImGui::SliderFloat("Y", &light_dir[1], 0.f, 180.f);
			ImGui::SliderFloat("Z", &light_dir[2], 0.f, 180.f);
			//ImGui::DragFloat3("Light Direction", light_dir, 0.1, 0.f, 1.f);

			Engine::GetInstance()->m_Renderer->m_Direction = CU::Vector3f(light_dir[0], light_dir[1], light_dir[2]);

			std::stringstream camera_pos;
			const auto& pos = Engine::GetInstance()->GetCamera()->GetPosition();
			camera_pos << "x:" << pos.x << "\ny:" << pos.y << "\nz:" << pos.z;
			ImGui::Text("%s", camera_pos.str().c_str());

			ImGui::Separator();
			
			if (ImGui::Button("save level", ImVec2(100, 25)))
			{
				LevelFactory::SaveLevel("data/pbr_level/", "pbr_level.level");
			}

			DebugTextures();

			ImGui::Separator();
			ImGui::Text("Hovering : %d", m_CurrEntity);

			if (!m_LutLables.empty())
			{
				static int index = 0;
				ListBox("", &index, m_LutLables);
				Engine::GetInstance()->m_Renderer->m_PostProcessManager.GetHDRPass().SetLUT(m_LutTextures[index]);
			}

			ImGui::End();
		}
		ImGui::PopStyleVar();

		HandleWorldContextMenu(pEngine);

		m_Inspector.Update(Engine::GetInstance()->GetDeltaTime());
	}


	void DebugHandle::DebugTextures()
	{
		static bool pOpen = false;
		//ImGui::PushStyleVar(AutoSize)
		if (ImGui::Begin("textures", &pOpen, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::PushItemWidth(250.f);
			ListBox("", &m_TextureIndex, m_Labels);
			ImGui::PopItemWidth();

			ImTextureID tex_id = m_DebugTexture;
			ImVec2 w_size = ImGui::GetWindowSize();
			w_size.x *= 0.65f;
			w_size.y = w_size.x / 1.777777777777777777777777777777778;
			ImGui::SameLine();
			ImGui::Image(tex_id, w_size, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
		}
		ImGui::End();

	}

	void DebugHandle::AddTexture(Texture* texture, const std::string& debug_name)
	{
		AddTexture(texture->GetShaderView(), debug_name);
	}

	void DebugHandle::AddTexture(void* srv, const std::string& debug_name)
	{
		m_DebugTextures.Add(static_cast<ID3D11ShaderResourceView*>(srv));
		m_Labels.push_back(debug_name);
	}


	void DebugHandle::RegisterFloatSlider(DebugSlider<float> slider)
	{
		m_Sliders.Add(slider);
	}

	void DebugHandle::RegisterIntValue(DebugTextValue<int> int_Value)
	{
		m_Values.Add(int_Value);
	}

	void DebugHandle::AddText(std::string str)
	{
		m_Text.Add(str);
	}

	void DebugHandle::AddValueToPrint(s32* value)
	{
		m_IntValuesToPrint.Add(value);
	}

	void DebugHandle::SetEntity(Entity e)
	{
		if (e <= 0)
			return;
		m_PrevEntity = m_CurrEntity;
		m_CurrEntity = e;



		m_ModelInstances.clear();
		m_InstanceLabels.clear();

	}

	Entity DebugHandle::GetHoveredEntity() const
	{
		return m_CurrEntity;
	}

	Entity DebugHandle::GetSelectedEntity() const
	{
		return m_EditEntity;

	}

	void DebugHandle::RegisterCheckbox(DebugCheckbox checkbox)
	{
		m_Checkboxes.Add(checkbox);
	}

	void DebugHandle::RegisterMaterial(Material* pMaterial, std::string lable)
	{
		m_Materials.push_back(pMaterial);
		m_MaterialLabels.push_back(lable);
	}

	void DebugHandle::ConfirmEntity()
	{
		if (m_CurrEntity <= 0)
			return;

		if (!ImGui::IsAnyWindowHovered())
		{
			if (!ImGuizmo::IsUsing() && !ImGuizmo::IsOver())
			{
				m_EditEntity = m_CurrEntity;
				m_Inspector.SetEntity(m_EditEntity);
			}
		}
	}

	s32 DebugHandle::GetDebugTextureIndex() const
	{
		return m_TextureIndex;
	}

	void DebugHandle::SetDebugTexture(Texture* tex)
	{
		m_DebugTexture = static_cast<ID3D11ShaderResourceView*>(tex->GetShaderView());
	}

	void DebugHandle::SetObjectMatrix(CU::Matrix44f* mat)
	{
		m_ObjectMatrix = mat;
	}

	

	void DebugHandle::HandleEvent(u64 event, void* data /*= nullptr*/)
	{
		if (event == DebugEvents_OnRightClick)
		{
			s_RightClicked = true;

			CU::Vector3f intersection;
			memcpy(&intersection, data, sizeof(CU::Vector3f));

			s_CreatePosition = intersection;

		
		}
		else if (event == Hash("create_entity"))
		{
			Engine* engine = Engine::GetInstance();
			Entity e = engine->GetEntityManager().CreateEntity();
			LevelFactory::CreateEntity(e, engine->GetEntityManager());
			NetworkComponent& c = engine->GetEntityManager().GetComponent<NetworkComponent>(e);
			memcpy(&c.m_GUID, data, sizeof(GUID));

			TranslationComponent& t = engine->GetEntityManager().GetComponent<TranslationComponent>(e);
			engine->GetNetworkManager()->AddReplicant(c.m_GUID, &t);

		}
		else if (event == Hash("copy_selected"))
		{
			CopyEntity(m_EditEntity);


		}
		else if (event == Hash("paste_new"))
		{
			PasteEntity();


		}
	}

	void DebugHandle::AddLUT(const char* lable, Texture* tex)
	{
		m_LutLables.push_back(lable);
		m_LutTextures.push_back(tex);
	}

	void HandleWorldContextMenu(Engine* pEngine)
	{

		static ImVec2 menu_pos;
		if (s_RightClicked)
		{
			const CU::Vector2f& pos = pEngine->GetInputHandle()->GetCursorPos();
			menu_pos = ImVec2(pos.x, pos.y);
			s_RightClicked = !s_RightClicked;
			s_OpenMenu = !s_OpenMenu;
		}

		if (s_OpenMenu)
		{
			ImGui::SetNextWindowPos(menu_pos);
			ImGui::SetNextWindowSize(ImVec2(150, 200));
			if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize))
			{
				if (ImGui::Button("Create Entity"))
				{
					Entity e = pEngine->GetEntityManager().CreateEntity();
					LevelFactory::CreateEntity(e, pEngine->GetEntityManager());
					TranslationComponent& t = pEngine->GetEntityManager().GetComponent<TranslationComponent>(e);
					t.SetPosition(s_CreatePosition);
					s_OpenMenu = false;
				}

				ImGui::End();
			}
		}
	}


	static GraphicsComponent s_Graphics;
	static TranslationComponent s_Translation;
	static PhysicsComponent s_Physics;
	static LightComponent s_Light;
	bool s_HasNetwork = false;

	void PasteEntity()
	{
		Engine* engine = Engine::GetInstance();
		EntityManager& em = engine->GetEntityManager();
		Entity e = em.CreateEntity();

		TranslationComponent& t = em.AddComponent<TranslationComponent>(e);
		//t = s_Translation;

		GraphicsComponent& g = em.AddComponent<GraphicsComponent>(e);
		g = s_Graphics;
		//memcpy(&g, &s_Graphics, sizeof(GraphicsComponent));


		DebugComponent& d = em.AddComponent<DebugComponent>(e);
		d.m_ComponentFlags |= (TreeDweller::GRAPHICS | TreeDweller::TRANSLATION | TreeDweller::DEBUG);
		TreeDweller* dweller = new TreeDweller;
		dweller->AddComponent(&t, TreeDweller::TRANSLATION);
		dweller->AddComponent(&g, TreeDweller::GRAPHICS);
		dweller->AddComponent(&d, TreeDweller::DEBUG);

		dweller->Initiate(e, TreeDweller::DYNAMIC);
		d.m_Dweller = dweller;

		EventManager::GetInstance()->SendMessage(DebugEvents_AddEntity, dweller);
	}

	void CopyEntity(Entity m_EditEntity)
	{
		Engine* engine = Engine::GetInstance();
		EntityManager& em = engine->GetEntityManager();

		DebugComponent& _debug = em.GetComponent<DebugComponent>(m_EditEntity);
		s32 flags = _debug.m_ComponentFlags;

		if (flags & TreeDweller::TRANSLATION)
		{
			s_Translation = em.GetComponent<TranslationComponent>(m_EditEntity);
		}

		if (flags & TreeDweller::GRAPHICS)
		{
			s_Graphics = em.GetComponent<GraphicsComponent>(m_EditEntity);
		}
	}



};
#endif
