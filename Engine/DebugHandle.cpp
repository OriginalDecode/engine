#include "stdafx.h"
#include "DebugHandle.h"
#include <Engine/Texture.h>
#include <Engine/Engine.h>
#include <Engine/Synchronizer.h>
#include <Engine/LevelFactory.h>

#include <EntitySystem/LightComponent.h>
#include <EntitySystem/TranslationComponent.h>
#include <EntitySystem/GraphicsComponent.h>
#include <EntitySystem/EntityManager.h>
#include <EntitySystem/ComponentFilter.h>
#include <vector>
#if !defined(_PROFILE) && !defined(_FINAL)
#include "imgui.h"
#include "ImGuizmo.h"
namespace debug
{
	void EditTransform(const float *cameraView, float *cameraProjection, float* matrix)
	{
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		static bool useSnap = false;
		static float snap[3] = { 1.f, 1.f, 1.f };

		constexpr int w_key = 'W';
		constexpr int r_key = 'R';
		constexpr int e_key = 'E';
		constexpr int s_key = 'S';


		if (ImGui::IsKeyPressed(w_key))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(e_key))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(r_key)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation, 3);
		ImGui::InputFloat3("Rt", matrixRotation, 3);
		ImGui::InputFloat3("Sc", matrixScale, 3);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		if (mCurrentGizmoOperation == ImGuizmo::SCALE)
			mCurrentGizmoMode = ImGuizmo::LOCAL;




		if (ImGui::IsKeyPressed(s_key))
			useSnap = !useSnap;
		ImGui::Checkbox("", &useSnap);
		ImGui::SameLine();

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL);
	}


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


			for (DebugCheckbox& cb : m_Checkboxes)
			{
				ImGui::Checkbox(cb.m_Label.c_str(), cb.m_Toggleable);
			}

			ImGui::Checkbox("Debug Textures", &sDebugTextures);
			if (sDebugTextures)
				DebugTextures();

			ImGui::Separator();
			for (DebugSlider<float>& s : m_Sliders)
			{
				ImGui::SliderFloat(s.m_Label.c_str(), s.m_Value, s.m_Min, s.m_Max);
			}

			auto& em = Engine::GetInstance()->GetEntityManager();


			if (em.HasComponents(m_EditEntity, CreateFilter<Requires<LightComponent>>()))
			{
				if (ImGui::Begin("Light"))
				{

					static float angle = 0;
					ImGui::SliderAngle("Light Angle", &angle);

					static float rot[3];
					ImGui::SliderFloat3("Create rotation", rot, 0.f, 1.f);

					static float intensity = 0;
					ImGui::SliderFloat("Light Intensity", &intensity, 1.f, 1000.f);
					static float col[3];
					ImGui::ColorEdit3("Color", col);

					LightComponent& comp = Engine::GetInstance()->GetEntityManager().GetComponent<LightComponent>(m_EditEntity);
					TranslationComponent& t = Engine::GetInstance()->GetEntityManager().GetComponent<TranslationComponent>(m_EditEntity);
					comp.angle = angle;
					comp.color.x = col[0];
					comp.color.y = col[1];
					comp.color.z = col[2];
					comp.intensity = intensity;

					CU::Vector3f pos = t.myOrientation.GetPosition();
					t.myOrientation.SetRotation3dX(cl::DegreeToRad(rot[0]));// = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(rot[0]));
					t.myOrientation.SetRotation3dY(cl::DegreeToRad(rot[1]));// = CU::Matrix44f::CreateRotateAroundY(cl::DegreeToRad(rot[1]));
					t.myOrientation.SetRotation3dZ(cl::DegreeToRad(rot[2]));// = CU::Matrix44f::CreateRotateAroundZ(cl::DegreeToRad(rot[2]));

					//t.myOrientation.SetForward({ rot[0], rot[1], rot[2], 1.f });


					ImGui::End();
				}
			}

			ImGui::Separator();
			ImGui::Text("Hovering : %d", m_CurrEntity);
			if (em.HasComponents(m_EditEntity, CreateFilter<Requires<GraphicsComponent>>()))
			{
				ModelInstance* instance = nullptr;
				static bool material_prompt = false;


				GraphicsComponent& g = Engine::GetInstance()->GetEntityManager().GetComponent<GraphicsComponent>(m_EditEntity);
				TranslationComponent& t = Engine::GetInstance()->GetEntityManager().GetComponent<TranslationComponent>(m_EditEntity);

				if (m_ModelInstances.empty())
				{
					for (s32 i = 0; i < g.m_Instances.Size(); i++)
					{
						ModelInstance* instance = &g.m_Instances[i];
						m_ModelInstances.push_back(instance);
						char buf[50];
						ZeroMemory(&buf, 50 * sizeof(char));
						sprintf_s(buf, "Instance : %d", i);
						m_InstanceLabels.push_back(buf);

					}
				}

				static s32 instance_index = -1;
				ListBox("", &instance_index, m_InstanceLabels);
				if (instance_index >= 0)
				{
					instance = m_ModelInstances[instance_index];

					ImGui::Text("Filename : %s\n", instance->m_Filename.c_str());
					//ImGui::Text("File Hash : %lu\n", instance->m_ModelID);
					ImGui::Text("Material : %s\n", instance->m_MaterialFile.c_str());


					for (s32 i = 0; i < m_MaterialLabels.size(); i++)
					{
						if (m_MaterialLabels[i].find(instance->m_MaterialFile.c_str()) != m_MaterialLabels[i].npos)
						{
							s_MaterialIndex = i;
						}
					}


					if (ImGui::Button("Edit Material", ImVec2(100, 25)))
						material_prompt = !material_prompt;

					//ImGui::Text("Material Hash : %lu\n", instance->m_MaterialKey);

				}



				if (ImGui::BeginChild("", ImVec2(250, 100)))
				{

					if (instance != nullptr)
					{
						ImGui::PushItemWidth(300.f);
						ListBox("", &s_MaterialIndex, m_MaterialLabels);

						u64 hash = m_Materials[s_MaterialIndex]->GetKey();
						std::string name = m_MaterialLabels[s_MaterialIndex];


						instance->m_MaterialFile = name;
						instance->m_MaterialKey = hash;
					}

					ImGui::EndChild();
				}


				if (ImGui::Button("save level", ImVec2(100, 25)))
				{
					LevelFactory::SaveLevel("data/pbr_level/", "pbr_level.level");
				}

				ImGui::SetNextWindowPos(ImVec2(1200, 10));
				ImGui::SetNextWindowSize(ImVec2(320, 300));
				
				if (ImGui::Begin("Entity Inspector"))
				{

					ImGui::Text("Entity %d", m_EditEntity);


					Camera* cam = Engine::GetInstance()->GetCamera();
					CU::Matrix44f& orientation = CU::Math::Inverse(cam->GetOrientation());
					CU::Matrix44f& perspective = cam->GetPerspective();
					CU::Matrix44f& object_matrix = *m_ObjectMatrix;

					if (m_ObjectMatrix)
						EditTransform(orientation.myMatrix, perspective.myMatrix, m_ObjectMatrix->myMatrix);

					ImGui::End();
				}
			}

			ImGui::End();
		}
		ImGui::PopStyleVar();

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

			ImTextureID tex_id = m_DebugTextures[m_TextureIndex];
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
		m_EditEntity = m_CurrEntity;
	}

	s32 DebugHandle::GetDebugTextureIndex() const
	{
		return m_TextureIndex;
	}

	void DebugHandle::SetObjectMatrix(CU::Matrix44f* mat)
	{
		m_ObjectMatrix = mat;
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
