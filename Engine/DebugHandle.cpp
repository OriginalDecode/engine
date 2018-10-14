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
#include <Engine/TerrainSystem.h>

#include <Input/InputHandle.h>

#include <EntitySystem/EntityManager.h>
#include <EntitySystem/ComponentFilter.h>

#include <Physics/RigidBody.h>
#include <Physics/PhysicsManager.h>

#include "NodeEntityManager.h"

#include <vector>

#include <imgui/imgui.h>
#include <imgui/ImGuizmo.h>

#include <PostMaster/EventManager.h>
#include <hash/DebugEvents.h>
#include <network/NetworkManager.h>
#include <CommonLib/Utilities.h>
namespace debug
{
	const char* TEXTURE_SLOT_NAMES[] = {
		"ALBEDO/DIFFUSE",
		"NORMAL",
		"ROUGHNESS",
		"METALNESS",
		"EMISSIVE",
		"OPACTIY",
		"AO",
		"DEPTH",
		"SHADOWMAP",
		"CUBEMAP",
	};


	static s32 widget_id = 0;
	static float light_dir[3];
	static s32 s_MaterialIndex = 0;

	static auto GetVector = [](void* vec, int index, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (index < 0 || index >= static_cast<int>(vector.size()))
			return false;
		*out_text = vector.at(index).c_str();

		return true;
	};

	bool ListBox(const char* label, int* current_index, std::vector<std::string>& values, size_t height_in_items = -1)
	{
		if (values.empty())
			return false;

		return ImGui::ListBox(label, current_index, GetVector, static_cast<void*>(&values), values.size(), height_in_items);
	}

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty())
			return false;

		return ImGui::Combo(label, currIndex, GetVector, static_cast<void*>(&values), values.size());
	}




	void DebugHandle::ConfirmModel()
	{
		if (m_HoveredModel.m_Hash <= 0)
			return;

		m_SelectedModel = m_HoveredModel;

	}

	DebugHandle* DebugHandle::m_Instance = nullptr;




	void SplitString(const std::string& str, std::vector<std::string>& vec)
	{
		size_t pos = str.find('|');
		if (vec.size() > 0 && vec[vec.size() - 1].compare(str) == 0 && str.find("|", pos) == str.npos)
		{
			return;
		}

		std::string out = str.substr(0, pos);
		vec.push_back(out);

		SplitString(str.substr(pos + 1), vec);

	}


	void DebugHandle::AddTimingObject(const std::string& view_tree_and_time_string)
	{
		return;
		std::vector<std::string> list;

		SplitString(view_tree_and_time_string, list);

		TimingObjectDisplay* last = nullptr;


		bool found = false;
		for (TimingObjectDisplay& obj : m_TimingObjects)
		{
			if (obj.m_Text.compare(list[0]) == 0)
			{
				found = true;
				last = &obj;
				break;
			}
		}

		if (!found)
		{
			TimingObjectDisplay object;
			object.m_Type = TimingObjectDisplay::ROOT;
			object.m_Text = list[0];
			m_TimingObjects.Add(object);
			last = &m_TimingObjects.GetLast();
		}

		for (size_t i = 1; i < list.size(); i++)
		{

			if (!last)
				break;
			TimingObjectDisplay tod;
			tod.m_Type = (i != list.size() - 1) ? TimingObjectDisplay::CHILD : TimingObjectDisplay::END;
			tod.m_Text = list[i];

			bool _found = false;
			for (TimingObjectDisplay& child : last->m_Children)
			{
				if (child.m_Text.compare(list[i]) == 0)
				{
					_found = true;
					last = &child;
					break;
				}
			}

			if (!_found || (tod.m_Type == TimingObjectDisplay::END))
			{
				last->m_Children.push_back(tod);
				last = &last->m_Children[last->m_Children.size() - 1];
			}
		}

	}

	bool DebugHandle::s_PausePhysics = false;
	DebugHandle::DebugHandle()
	{
		light_dir[0] = 0.01f;
		light_dir[1] = 1.f;
		light_dir[2] = 0.01f;

		EventManager* mgr = EventManager::GetInstance();
		mgr->Subscribe("create_entity", this);
		mgr->Subscribe(DebugEvents_OnRightClick, this);
		mgr->Subscribe("copy_selected", this);
		mgr->Subscribe("paste_new", this);
		mgr->Subscribe("left_click", this);
		m_RegisteredSampleTextures.Init(32);

		ImGui::StyleColorsDark();

	}



	DebugHandle::~DebugHandle()
	{
		for (Texture* t : m_LutTextures)
		{
			delete t;
			t = nullptr;
		}
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

					Model* pModel = Engine::GetInstance()->GetModel<Model>(cl::Hash("default")).GetData();
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

	static char level_name[250];
	void DebugHandle::Update()
	{

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		if (ImGui::Begin("Window"))
		{
			const ImGuiStyle& style = ImGui::GetStyle();
			const ImVec4 active = style.Colors[ImGuiCol_ButtonHovered];
			const ImVec4 inactive = style.Colors[ImGuiCol_Button];

			static s32 button_idx = 1;

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.1f, 0.0f));

			ImGui::PushStyleColor(ImGuiCol_Button, (button_idx == 1) ? active : inactive);
			if (ImGui::Button("Information"))
				button_idx = 1;
			ImGui::PopStyleColor();

			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, (button_idx == 2) ? active : inactive);
			if (ImGui::Button("Light Direction"))
				button_idx = 2;
			ImGui::PopStyleColor();

			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, (button_idx == 3) ? active : inactive);
			if (ImGui::Button("Textures"))
				button_idx = 3;
			ImGui::PopStyleColor();











			/*	ImGui::PushStyleColor(ImGuiCol_Button, (button_idx == 2) ? active : inactive);
				if (ImGui::Button("Render Targets"))
					button_idx = 2;
				ImGui::PopStyleColor();

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, (button_idx == 3) ? active : inactive);
				if (ImGui::Button("Tab3"))
					button_idx = 3;
				ImGui::PopStyleColor();*/
			ImGui::PopStyleVar(2);


			switch (button_idx)
			{
				case 1: {
					Information();
				} break;
				case 2: {
					if (ImGui::BeginChildFrame(1, ImVec2(ImGui::GetWindowWidth() - 10, 0), 0))
					{
						ImGui::SliderFloat("X", &light_dir[0], -1.f, 1.f);
						ImGui::SliderFloat("Y", &light_dir[1], -1.f, 1.f);
						ImGui::SliderFloat("Z", &light_dir[2], -1.f, 1.f);
						Engine::GetInstance()->m_Renderer->SetDirection(light_dir);

						//ImGui::SliderFloat("Distance")



					}
					ImGui::EndChildFrame();
				} break;
				case 3: {
					static int _item = 1;
					ImGui::PushItemWidth(250.f);
					for (DebugTextureCategory& c : m_Categories)
					{
						Combo("", &m_TextureIndex, c.labels);
					}
					ImGui::PopItemWidth();

					ImVec2 w_size = ImGui::GetWindowSize();
					w_size.x = w_size.x - (style.WindowPadding.x * 2.f);
					//w_size.x *= 0.65f;
					w_size.y = w_size.x / 1.777777777777777777777777777777778; //Aspect ratio division.
					ImTextureID tex_id = m_DebugTexture;
					ImGui::Image(tex_id, w_size, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
				} break;


			}
		}
		ImGui::End();
		ImGui::PopStyleVar();

		/*	ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(300, Engine::GetInstance()->GetInnerSize().m_Height));*/


			/* New Window */
		/*static bool s_LightControls = false;
		if (ImGui::Begin("Light controls", &s_LightControls, 0))
		{

			if (ImGui::TreeNode("Light Direction"))
			{
				ImGui::SliderFloat("X", &light_dir[0], -1.f, 1.f);
				ImGui::SliderFloat("Y", &light_dir[1], 0.f, 1.f);
				ImGui::SliderFloat("Z", &light_dir[2], -1.f, 1.f);
				Engine::GetInstance()->m_Renderer->SetDirection(light_dir);
				ImGui::TreePop();
			}

			ImGui::Separator();
		}
		ImGui::End();*/
		/* End of new Window */

		//DebugTextures();


	}


	void DebugHandle::DebugTextures()
	{
		static bool open = false;
		if (ImGui::Begin("textures", &open, ImGuiWindowFlags_AlwaysAutoResize))
		{


			ImGui::PushItemWidth(250.f);
			for (DebugTextureCategory& c : m_Categories)
			{
				ListBox("", &m_TextureIndex, c.labels);
			}
			ImGui::PopItemWidth();

			ImVec2 w_size = ImGui::GetWindowSize();
			w_size.x *= 0.65f;
			w_size.y = w_size.x / 1.777777777777777777777777777777778; //Aspect ratio division.
			ImGui::SameLine();
			ImTextureID tex_id = m_DebugTexture;
			ImGui::Image(tex_id, w_size, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
		}
		ImGui::End();

	}



	void DebugHandle::AddText(std::string str)
	{
		m_Text.RemoveAll();
		m_Text.Add(str);
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
			m_EditEntity = m_CurrEntity;
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

	void DebugHandle::RegisterTexture(Texture* texture, const char* name, const char* category)
	{
		//if (std::find(m_Categories.begin(), m_Categories.end(), [&](DebugTextureCategory& t) {
		//	return t.category.compare(category) == 0;
		//}) == m_Categories.end())
		//	m_Categories.emplace_back(category);

		m_RegisteredSampleTextures.Add(texture);
		for (DebugTextureCategory& c : m_Categories)
		{
			if (c.category.compare(category) == 0)
			{
				c.labels.emplace_back(name);
				return;
			}
		}

		m_Categories.emplace_back(category);
		m_Categories.back().labels.emplace_back(name);
	}

	void DebugHandle::UnregisterTexture(Texture* t, const char* name)
	{

	}

	Texture* DebugHandle::GetTexture(s32 index)
	{
		return m_RegisteredSampleTextures[index];
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
		else if (event == cl::Hash("create_entity"))
		{
			Engine* engine = Engine::GetInstance();
			Entity e = engine->GetEntityManager().CreateEntity();
			LevelFactory::CreateEntity(e, engine->GetEntityManager());
			NetworkComponent& c = engine->GetEntityManager().GetComponent<NetworkComponent>(e);
			memcpy(&c.m_GUID, data, sizeof(GUID));

			TranslationComponent& t = engine->GetEntityManager().GetComponent<TranslationComponent>(e);
			engine->GetNetworkManager()->AddReplicant(c.m_GUID, &t);

		}
		else if (event == cl::Hash("copy_selected"))
		{
			CopyEntity(m_EditEntity);


		}
		else if (event == cl::Hash("paste_new"))
		{
			PasteEntity();
		}
		else if (event == cl::Hash("left_click"))
		{
			ConfirmModel();
		}
	}

	void DebugHandle::AddLUT(const char* lable, Texture* tex)
	{
		m_LutLables.push_back(lable);
		m_LutTextures.push_back(tex);
	}

	void DebugHandle::ChildRecursive(const TimingObjectDisplay &root)
	{
		for (const TimingObjectDisplay& child : root.m_Children)
		{
			if (child.m_Type == TimingObjectDisplay::END)
				ImGui::Text("%s", child.m_Text.c_str());

			if (child.m_Type == TimingObjectDisplay::CHILD)
			{

				if (ImGui::TreeNode(child.m_Text.c_str()))
				{
					ChildRecursive(child);
					ImGui::TreePop();
				}
			}
		}


	}
	static int selected_index = -1;
	static Model* model = nullptr;
	static bool model_info = false;
	static Model* selected_child = nullptr;
	void DebugHandle::Information()
	{

		Engine* pEngine = Engine::GetInstance();
		ImGuiWindowFlags flags = 0;
		//flags |= ImGuiWindowFlags_NoTitleBar;
		//flags |= ImGuiWindowFlags_NoResize;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		static bool open = false;

		if (ImGui::BeginChildFrame(1, ImVec2(ImGui::GetWindowWidth() - 10, 0), 0))
		{
			ImGui::Text("Delta Time : %.3f", pEngine->GetDeltaTime());
			ImGui::Text("FPS : %.1f", pEngine->GetFPS());
			ImGui::Text("CPU Usage : %.1f", pEngine->m_SystemMonitor.GetCPUUsage()); /* does not show individual cores */
			ImGui::Text("Memory Usage : %dmb", pEngine->m_SystemMonitor.GetMemoryUsage());/*
			ImGui::Text("Model Commands : %d", pEngine->m_SegmentHandle.CommandSize((s32)pEngine->m_Synchronizer->GetCurrentBufferIndex()));
			ImGui::Text("Spotlight Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::SPOTLIGHT_BUFFER).Size());
			ImGui::Text("Pointlight Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::POINTLIGHT_BUFFER).Size());
			ImGui::Text("Particle Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::PARTICLE_BUFFER).Size());
			ImGui::Text("Sprite Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::SPRITE_BUFFER).Size());
			ImGui::Text("Text Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::TEXT_BUFFER).Size());
			ImGui::Text("Line Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::LINE_BUFFER).Size());*/

			ImGui::Separator();

			std::stringstream camera_pos;
			const auto& pos = Engine::GetInstance()->GetCamera()->GetPosition();
			camera_pos << "x:" << pos.x << "\ny:" << pos.y << "\nz:" << pos.z;
			ImGui::Text("%s", camera_pos.str().c_str());
			ImGui::Separator();

			if (m_SelectedModel.m_Hash > 0)
			{
				ImGui::Text("Current Model (HASH): %llu", m_SelectedModel.m_Hash);

				if (model)
					model->SetSelected(false);

				model = Engine::GetInstance()->GetModelDirect(m_SelectedModel.m_Lower);

				if (model)
				{
					ImGui::Text("Current Model (NAME): %s", model->GetFileName().c_str());
					model->SetSelected(true);
					auto& children = model->GetChildren();

					char temp[100];
					sprintf_s(temp, "Model\tChildren (%d)", children.Size());

					if (ImGui::TreeNode(temp))
					{

						//ImGui::Text("Children (%d)", children.Size());

						WriteChildren(children);


						ImGui::TreePop();
					}


				}

				if (selected_index >= 0)
				{

					std::vector<std::string> labels;
					std::vector<IShaderResourceView*> shader_resources;
					int text_width = 0;
					if (selected_child->GetSurface())
					{
						const Material& mat = selected_child->GetSurface()->GetMaterial();
						auto& resources = mat.GetResourceBindings();
						
						for (auto& resource : resources)
						{
							char temp[100];
							size_t pos = resource.m_ResourceName.rfind('/');
							std::string simplified = resource.m_ResourceName.substr(pos);

							sprintf_s(temp, "Resource %s\tSlot %s(%d)", simplified.c_str(), TEXTURE_SLOT_NAMES[resource.m_Slot], resource.m_Slot);

							const int length = strlen(temp);

							if (text_width < length)
								text_width = length;

							labels.push_back(temp);
							shader_resources.push_back(resource.m_Resource);


						}
					}

					ImVec2 pos = ImGui::GetWindowPos();
					float width = ImGui::GetWindowWidth();
					ImGui::SetNextWindowPos(ImVec2(pos.x + width, pos.y));
					if (ImGui::Begin("Model Info", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("child %d", selected_index);



						static int selected_resource = -1;
						ImGui::PushItemWidth((ImGui::GetFontSize() * text_width) * 0.65f) ;
						ListBox("", &selected_resource, labels);
						ImGui::PopItemWidth();

						if (selected_resource >= 0)
						{
							static float x = 512.f;
							static float y = 512.f;

							ImGui::Text("Width");
							ImGui::SameLine(110.f);
							ImGui::Text("Height");
							ImGui::PushItemWidth(100.f);
							ImGui::InputFloat("", &x);
							ImGui::SameLine();
							ImGui::InputFloat("", &y);
							ImGui::PopItemWidth();


							ImTextureID tex_id = shader_resources[selected_resource];

							ImGui::Image(tex_id, ImVec2(x, y), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
						}

						ImGui::End();
					}
				}




			}

			//ImGui::Text("Hovering : %d", m_CurrEntity);

			if (ImGui::TreeNode("Lut Textures"))
			{
				if (!m_LutLables.empty())
				{
					static int index = 0;
					ListBox("", &index, m_LutLables);
					Engine::GetInstance()->m_Renderer->m_PostProcessManager.GetHDRPass().SetLUT(m_LutTextures[index]);
				}
				ImGui::TreePop();
			}


			/*static Window* win = nullptr;
			if (ImGui::Button("click me") && !win)
			{

				WindowCreateInfo wci;
				wci.window_height = 720.f;
				wci.window_width = 1280.f;
				wci.window_process = (WNDPROC)Engine::GetInstance()->wndproc;
				wci.instance = (HINSTANCE)Engine::GetInstance()->hinstance;

				win = new Window;
				win->Initiate(wci, WS_BORDER);
				win->ShowWindow();
			}*/


		}
		ImGui::EndChildFrame();
		ImGui::PopStyleVar();
	}

	void DebugHandle::WriteChildren(const CU::GrowingArray<Model *>& parent_list)
	{
		std::vector<std::string> labels;
		std::vector<Model*> child_ptrs;
		for (int i = 0; i < parent_list.Size(); ++i)
		{
			static bool selected = false;
			Model* child = parent_list[i];


			char temp[100];
			sprintf_s(temp, "Child %d", i);
			labels.push_back(temp);
			child_ptrs.push_back(child);



			//ImGui::Text("Child %d", i);
			//ImGui::Selectable("Child", &selected);

			auto& children = child->GetChildren();
			if (children.Empty())
				continue;

			/*if(ImGui::TreeNode("Children"))
			{
				WriteChildren(children);
				ImGui::TreePop();
			}*/

		}

		ListBox("", &selected_index, labels);
		if (selected_index >= 0)
			selected_child = child_ptrs[selected_index];


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
