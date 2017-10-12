#include "stdafx.h"
#include "DebugHandle.h"
#include <Engine/Texture.h>
#include <Engine/Engine.h>
#include <Engine/Synchronizer.h>


#if !defined(_PROFILE) && !defined(_FINAL)
#include "imgui.h"
namespace debug
{

	DebugHandle* DebugHandle::m_Instance = nullptr;
	static bool sDebugTextures = false;

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

			//ImGui::Text("Model Commands : %d", mySynchronizer->GetRenderCommands(eBufferType::MODEL_BUFFER).Size());
			ImGui::Text("Model Commands : %d", pEngine->m_SegmentHandle.CommandSize((s32)pEngine->m_Synchronizer->GetCurrentBufferIndex()));

			ImGui::Text("Spotlight Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::SPOTLIGHT_BUFFER).Size());
			ImGui::Text("Pointlight Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::POINTLIGHT_BUFFER).Size());
			ImGui::Text("Particle Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::PARTICLE_BUFFER).Size());
			ImGui::Text("Sprite Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::SPRITE_BUFFER).Size());
			ImGui::Text("Text Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::TEXT_BUFFER).Size());
			ImGui::Text("Line Commands : %d", pEngine->m_Synchronizer->GetRenderCommands(eBufferType::LINE_BUFFER).Size());

			ImGui::Separator();


			std::stringstream camera_pos;
			const auto& pos = Engine::GetInstance()->GetCamera()->GetPosition();
			camera_pos << "x:" << pos.x << "\ny:" << pos.y << "\nz:" << pos.z;
			ImGui::Text("%s", camera_pos.str().c_str());

			ImGui::Checkbox("Debug Textures", &sDebugTextures);

			if (sDebugTextures)
				DebugTextures();

			for (std::string str : m_Text)
			{
				ImGui::Text(str.c_str());
			}

			for (auto& obj : m_Values)
			{
				std::stringstream ss;
				ss << obj.m_Label << " : " << *obj.m_Value;
				ImGui::Text(ss.str().c_str());
			}

			if (ImGui::BeginChild("Region", ImVec2(ImGui::GetWindowSize().x- 25.f, 200.f), false))
			{
				for (s32* v : m_IntValuesToPrint)
				{
					ImGui::Text("%d", *v);
				}
				ImGui::EndChild();
			}

			ImGui::End();
		}
		ImGui::PopStyleVar();

	}

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

	void DebugHandle::DebugTextures()
	{
		static bool pOpen = false;
		//ImGui::PushStyleVar(AutoSize)
		if (ImGui::Begin("textures", &pOpen, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static s32 index = 0;
			ImGui::PushItemWidth(250.f);
			ListBox("", &index, m_Labels);
			ImGui::PopItemWidth();

			ImTextureID tex_id = m_DebugTextures[index];
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

	//  	s32 DebugHandle::RegisterMainWindow(DebugMainWindow window)
	//  	{
	//  		m_MainWindows.Add(window);
	//  	}
	//  
	//  	s32 DebugHandle::RegisterWindow(float width, float height, float x_pos, float y_pos, const std::string& label)
	//  	{
	//  
	//  	}



	// 
	// #if !defined(_PROFILE) && !defined(_FINAL)
	// 
	// 	bool Engine::SaveLevel()
	// 	{
	// 		static char file_name[512];
	// 		if (ImGui::Begin("Filename"))
	// 		{
	// 
	// 			ImGui::InputText("filename", file_name, 512);
	// 			if (ImGui::Button("Save", ImVec2(100, 30)))
	// 			{
	// 				if (!cl::substr(file_name, ".level"))
	// 				{
	// 					DL_WARNINGBOX("Incorrect filetype, no file created!");
	// 				}
	// 				else
	// 				{
	// 					std::ofstream f(file_name);
	// 					f << "Hello World!";
	// 					f.flush();
	// 					f.close();
	// 					m_PauseInput = false;
	// 					ImGui::End();
	// 					return true;
	// 				}
	// 			}
	// 		}
	// 		ImGui::End();
	// 
	// 		return false;
	// 	}
	// 





	// 
	// 	void Engine::AddFunction(const std::string& label, std::function<void()> function)
	// 	{
	// 		m_Functions.push_back(std::make_pair(label, function));
	// 	}
	// 
	// 
	// 	void Engine::AddCheckBox(bool* toggle, std::string label)
	// 	{
	// 		CheckBox box;
	// 		box.m_Name = label;
	// 		box.m_Toggle = toggle;
	// 		m_Checkboxes.Add(box);
	// 	}
	// 
	// 	void Engine::RegisterFloatSider(float* v, const char* label, float min, float max)
	// 	{
	// 		slider _slider;
	// 		_slider.current_value = v;
	// 		_slider.label = label;
	// 		_slider.max = max;
	// 		_slider.min = min;
	// 		m_Sliders.Add(_slider);
	// 	}
	// 
	// 
	// 
	// 	void Engine::UpdateDebugUI()
	// 	{
	// 		ImGui::SetNextWindowPos(ImVec2(0, 0));
	// 		ImGui::SetNextWindowSize(ImVec2(300, GetWindowSize().m_Height));
	// 
	// 		ImGuiWindowFlags flags = 0;
	// 
	// 		flags |= ImGuiWindowFlags_NoTitleBar;
	// 		flags |= ImGuiWindowFlags_NoResize;
	// 		//flags |= ImGuiWindowFlags_NoCollapse;
	// 		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	// 		static bool pOpen = false;
	// 		static bool new_window = false;
	// 		static bool save = false;
	// 		static bool debug_textures = false;
	// 
	// 		static bool entity_options = false;
	// 
	// 		if (ImGui::Begin("Information", &pOpen, flags))
	// 		{
	// 			ImGui::Text("Delta Time : %.3f", GetDeltaTime());
	// 			ImGui::Text("FPS : %.1f", GetFPS());
	// 			ImGui::Text("CPU Usage : %.1f", m_SystemMonitor.GetCPUUsage());
	// 			ImGui::Text("Memory Usage : %dmb", m_SystemMonitor.GetMemoryUsage());
	// 
	// 			//ImGui::Text("Model Commands : %d", mySynchronizer->GetRenderCommands(eBufferType::MODEL_BUFFER).Size());
	// 			ImGui::Text("Model Commands : %d", m_SegmentHandle.CommandSize((s32)mySynchronizer->GetCurrentBufferIndex()));
	// 
	// 			ImGui::Text("Spotlight Commands : %d", mySynchronizer->GetRenderCommands(eBufferType::SPOTLIGHT_BUFFER).Size());
	// 			ImGui::Text("Pointlight Commands : %d", mySynchronizer->GetRenderCommands(eBufferType::POINTLIGHT_BUFFER).Size());
	// 			ImGui::Text("Particle Commands : %d", mySynchronizer->GetRenderCommands(eBufferType::PARTICLE_BUFFER).Size());
	// 			ImGui::Text("Sprite Commands : %d", mySynchronizer->GetRenderCommands(eBufferType::SPRITE_BUFFER).Size());
	// 			ImGui::Text("Text Commands : %d", mySynchronizer->GetRenderCommands(eBufferType::TEXT_BUFFER).Size());
	// 			ImGui::Text("Line Commands : %d", mySynchronizer->GetRenderCommands(eBufferType::LINE_BUFFER).Size());
	// 
	// 			ImGui::Separator();
	// 
	// 
	// 			if (ImGui::TreeNodeEx("Entity", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen))
	// 			{
	// 				if (ImGui::Button("New Entity", ImVec2(100.f, 25.f)))
	// 				{
	// 					new_window = !new_window;
	// 				}
	// 				ImGui::Separator();
	// 			}
	// 
	// 			if (ImGui::TreeNodeEx("Post-Processing", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen))
	// 			{
	// 				static bool tonemapping_hdr = true;
	// 				static bool bloom = true;
	// 				static bool ssao = true;
	// 				static bool motion_blur = true;
	// 				static bool depth_of_field = true;
	// 
	// 				ImGui::Columns(2, "", true);
	// 				ImGui::Checkbox("Tonemapping/HDR", &tonemapping_hdr);
	// 				ImGui::Checkbox("Motion Blur", &motion_blur);
	// 				ImGui::Checkbox("Depth of Field", &depth_of_field);
	// 				ImGui::NextColumn();
	// 				ImGui::Checkbox("Bloom", &bloom);
	// 				ImGui::Checkbox("SSAO", &ssao);
	// 
	// 
	// 				ImGui::Columns(1);
	// 
	// 				if (tonemapping_hdr)
	// 					myRenderer->GetPostprocessManager().SetPassesToProcess(PostProcessManager::HDR);
	// 				else
	// 					myRenderer->GetPostprocessManager().RemovePassToProcess(PostProcessManager::HDR);
	// 
	// 				if (depth_of_field)
	// 				{
	// 					ImGui::Text("Depth of Field settings");
	// 					float fov_value = 0.f;
	// 					ImGui::SliderFloat("Strength", &fov_value, 0.f, 9999.f, "%.f");
	// 
	// 				}
	// 
	// 
	// 				ImGui::Separator();
	// 			}
	// 
	// 
	// 
	// 
	// 			/*if (ImGui::Button("Save Level", ImVec2(100.f, 25.f)))
	// 			{
	// 			save = !save;
	// 			m_PauseInput = true;
	// 			}*/
	// 
	// 			if (save) if (SaveLevel()) save = !save;
	// 
	// 
	// 			ImGui::Checkbox("Debug Textures", &debug_textures);
	// 			//ImGui::Checkbox("Debug Textures", &myRenderer->GetPostprocessManager().GetHDRPass().toggle_debug);
	// 			if (debug_textures)
	// 				DebugTextures();
	// 
	// 
	// 			static bool render_lines = false;
	// 			ImGui::Checkbox("Render Lines", &render_lines);
	// 			myRenderer->SetRenderLines(render_lines);
	// 			ImGui::SameLine();
	// 			ImGui::Checkbox("Instance Models", &m_RenderInstanced);
	// 
	// 
	// 			float fov_value = m_Camera->GetFOV();
	// 			ImGui::SliderFloat("FOV", &fov_value, 60.f, 120.f, "%.f");
	// 			m_Camera->SetFOV(fov_value);
	// 
	// 
	// 
	// 			if (ImGui::TreeNodeEx("Directional Light", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen))
	// 			{
	// 				/*
	// 				Need some kind of color picker / wheel instead
	// 
	// 				*/
	// 
	// 				/* Should be read from the level file */
	// 
	// 				static ImVec4 r_color;
	// 				static ImVec4 g_color;
	// 				static ImVec4 b_color;
	// 				static ImVec4 a_color;
	// 				static ImVec4 tot_color;
	// 
	// 				static float colors[4];
	// 
	// 				if (ImGui::ColorEdit4("colors", colors, true))
	// 				{
	// 
	// 				}
	// 
	// 				ImGui::Text("Directional Light Color");
	// 				//ImGui::SliderInt("R", &r, 0, 255);
	// 				//r_color = ImVec4((float)col / 255.f, 0, 0, 1);
	// 				//ImGui::SameLine();
	// 				//ImGui::ColorButton(r_color);
	// 
	// 
	// 				//ImGui::SliderInt("G", &g, 0, 255);
	// 				//g_color = ImVec4(0, (float)g / 255.f, 0, 1);
	// 				//ImGui::SameLine();
	// 				//ImGui::ColorButton(g_color);
	// 
	// 
	// 				//ImGui::SliderInt("B", &b, 0, 255);
	// 				//b_color = ImVec4(0, 0, (float)b / 255.f, 1);
	// 				//ImGui::SameLine();
	// 				//ImGui::ColorButton(b_color);
	// 
	// 				//ImGui::SliderInt("Intensity", &a, 0, 255);
	// 				//a_color = ImVec4((float)a / 255.f, (float)a / 255.f, (float)a / 255.f, 1);
	// 				//ImGui::SameLine(); 
	// 				//ImGui::ColorButton(a_color);
	// 
	// 
	// 				//tot_color = ImVec4((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)a / 255.f);
	// 				//ImGui::ColorButton(tot_color);
	// 
	// 
	// 				myRenderer->GetDeferredRenderer()->SetColor(CU::Vector4f(colors[0], colors[1], colors[2], colors[3]));
	// 
	// 
	// 
	// 				ImGui::Text("Directional Light Direction");
	// 				static float x = 0;
	// 				static float y = 0;
	// 				static float z = 0;
	// 				ImGui::SliderFloat("X", &x, -1.f, 1.f);
	// 				ImGui::SliderFloat("Y", &y, -1.f, 1.f);
	// 				ImGui::SliderFloat("Z", &z, -1.f, 1.f);
	// 
	// 
	// 
	// 				myRenderer->SetDirection(CU::Vector3f(x, y, z));
	// 				//myRenderer->GetDirectionalCamera()->SetAt(CU::Vector3f((float)x / 180.f, (float)y / 180.f, (float)z / 180.f));
	// 
	// 
	// 				/*ImGui::Text("Directional Shadow Position");
	// 				static float sx = 0;
	// 				static float sy = 0;
	// 				static float sz = 0;
	// 				ImGui::SliderFloat("sX", &sx, -30.f, 360.f);
	// 				ImGui::SliderFloat("sY", &sy, -30.f, 360.f);
	// 				ImGui::SliderFloat("sZ", &sz, -30.f, 360.f);
	// 				myRenderer->GetDirectionalCamera()->SetPosition(CU::Vector3f(sx, sy, sz));*/
	// 			}
	// 
	// 			for (CheckBox& box : m_Checkboxes)
	// 			{
	// 				ImGui::Checkbox(box.m_Name.c_str(), box.m_Toggle);
	// 			}
	// 
	// 
	// 			for (auto pair : m_Functions)
	// 			{
	// 				if (ImGui::Button(pair.first.c_str(), ImVec2(150, 15)))
	// 				{
	// 					pair.second();
	// 				}
	// 			}
	// 
	// 			for (slider& s : m_Sliders)
	// 			{
	// 				ImGui::SliderFloat(s.label, s.current_value, s.min, s.max);
	// 			}
	// 
	// 
	// 			/*static s32 index = 0;
	// 			s32 prev = index;
	// 			ListBox("", &index, m_Levels);
	// 			if (prev != index)
	// 			{
	// 			m_Functions[index].second();
	// 			}*/
	// 			ImGui::End();
	// 		}
	// 		ImGui::PopStyleVar();
	// 
	// 		if (new_window)
	// 		{
	// 			ImGui::SetNextWindowPos(ImVec2(300.f, 0.f));
	// 			//ImGui::SetNextWindowSize(ImVec2(300.f, 300.f));
	// 
	// 			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	// 			if (ImGui::Begin("New Window", &new_window, ImGuiWindowFlags_NoTitleBar))
	// 			{
	// 				ImGui::Text("Entity Creation");
	// 				ImGui::Separator();
	// 				static bool new_bp = false;
	// 				static bool new_entity = false;
	// 				if (ImGui::Button("New Blueprint", ImVec2(100.f, 25.f)))
	// 				{
	// 					new_bp = !new_bp;
	// 				}
	// 				ImGui::SameLine();
	// 				if (ImGui::Button("New Entity", ImVec2(100.f, 25.f)))
	// 				{
	// 					new_entity = !new_entity;
	// 				}
	// 
	// 
	// 
	// 
	// 				if (ImGui::IsItemHovered())
	// 				{
	// 					ImGui::BeginTooltip();
	// 					ImGui::Text("Create and Edit a new Blueprint.");
	// 					ImGui::EndTooltip();
	// 				}
	// 				ImGui::Separator();
	// 
	// 				if (new_bp)
	// 				{
	// 					static bool new_component = false;
	// 					static const int item_count = ARRAYSIZE(RegisteredComponentsStr) - 1;
	// 					static int components = 0;
	// 
	// 					static int selected[item_count] = { 0 };
	// 					static char* str = { "Translation\0Graphics\0Physics\0AI\0Network\0Input" };
	// 
	// 					ImGui::BeginChildFrame(0, ImVec2(ImGui::GetWindowSize().x, 200.f));
	// 					static bool edit_t = false;
	// 					if (new_component)
	// 					{
	// 						for (int i = 0; i < components; i++)
	// 						{
	// 							std::stringstream ss;
	// 							ss << "Box_" << i;
	// 							std::stringstream ss2;
	// 							ss2 << "X_" << i;
	// 							std::stringstream ss3;
	// 							ss3 << "Edit_" << i;
	// 							if (ImGui::Button(ss2.str().c_str()))
	// 							{
	// 								for (int j = i; j < components - 1; j++)
	// 								{
	// 									selected[i] = selected[j + 1];
	// 								}
	// 								components--;
	// 							}
	// 							ImGui::SameLine();
	// 							ImGui::Combo(ss.str().c_str(), &selected[i], str);
	// 							ImGui::SameLine();
	// 							if (ImGui::Button(ss3.str().c_str()))
	// 							{
	// 								switch (selected[i])
	// 								{
	// 									case RegisteredComponents::translation:
	// 									{
	// 										edit_t = !edit_t;
	// 									}break;
	// 								}
	// 							}
	// 						}
	// 
	// 						ImGui::Separator();
	// 					}
	// 
	// 					if (components < item_count)
	// 					{
	// 						if (ImGui::Button("Add Component"))
	// 						{
	// 							if (components < item_count)
	// 								components++;
	// 							new_component = true;
	// 						}
	// 					}
	// 
	// 					ImGui::EndChildFrame();
	// 
	// 					if (ImGui::Button("Save"))
	// 					{
	// 
	// 					}
	// 					ImGui::SameLine();
	// 					if (ImGui::Button("Cancel"))
	// 					{
	// 						new_bp = false;
	// 					}
	// 				}
	// 
	// 				if (new_entity)
	// 				{
	// 				}
	// 			}
	// 			ImGui::End();
	// 			ImGui::PopStyleVar();
	// 
	// 		}
	// 		EditEntity();
	// 	}
	// 
	// 	bool Engine::GetLineRendering()
	// 	{
	// 		return myRenderer->GetRenderLines();
	// 	}
	// 
	// 	void Engine::EditEntity()
	// 	{
	// 		if (!m_IsEditingEntity)
	// 			return;
	// 
	// 		DebugComponent& debug = m_EntityManager.GetComponent<DebugComponent>(m_EntityToEdit);
	// 		EditObject& to_edit = debug.m_EditObject;
	// 
	// 		std::stringstream ss;
	// 		ss << "Entity : " << m_EntityToEdit;
	// 		ImGui::SetNextWindowPos(ImVec2(300.f, 0));
	// 		ImGui::SetNextWindowSize(ImVec2(300.f, 600.f));
	// 		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	// 		if (ImGui::Begin(ss.str().c_str(), &m_IsEditingEntity, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	// 		{
	// 			to_edit.Update();
	// 			ImGui::End();
	// 		}
	// 		ImGui::PopStyleVar();
	// 	}
	// #endif
};
#endif
