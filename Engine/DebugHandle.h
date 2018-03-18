#pragma once

#if !defined(_PROFILE) && !defined(_FINAL)
#include <CommonLib/DataStructures/GrowingArray.h>
#include <CommonLib/Math/Matrix/Matrix.h>
#include <PostMaster/Subscriber.h>

#include <Engine/Inspector.h>

class Texture;
class Material;
struct ModelInstance;

namespace debug
{

	struct TimingObjectDisplay
	{
		enum Type
		{
			_NULL,
			ROOT,
			CHILD,
			END,
		};

		Type m_Type = _NULL;
		std::string m_Text;
		std::vector<TimingObjectDisplay> m_Children;
	};



	void EditTransform(const float *cameraView, float *cameraProjection, float* matrix);
	class DebugHandle : public Subscriber
	{
	public:
		static bool s_PausePhysics;
		static void Create() { m_Instance = new DebugHandle; }
		static void Destroy() { delete m_Instance; m_Instance = nullptr; }
		static DebugHandle* GetInstance() { return m_Instance; }
		void Update();

		void ChildRecursive(const TimingObjectDisplay &root);

		void DebugTextures();
		void AddTexture(void* srv, const std::string& debug_name);
		void AddTexture(Texture* texture, const std::string& debug_name);
		/*void RegisterFloatSlider(DebugSlider<float> slider);
		void RegisterIntValue(DebugTextValue<int> int_Value);*/

		void AddText(std::string str);
		void AddValueToPrint(s32* value);
		void SetEntity(Entity e);
		Entity GetHoveredEntity() const;
		Entity GetSelectedEntity() const;

		//void RegisterCheckbox(DebugCheckbox checkbox);

		void RegisterMaterial(Material* pMaterial, std::string lable);
		void ConfirmEntity();
		s32 GetDebugTextureIndex() const;

		void SetDebugTexture(Texture* tex);

		void RegisterTexture(Texture* texture, const char* name)
		{
			m_Labels.push_back(name);
			m_RegisteredSampleTextures.Add(texture);
		}
		Texture* GetTexture(s32 index) { return m_RegisteredSampleTextures[index]; }

		void SetObjectMatrix(CU::Matrix44f* mat);

		void HandleEvent(u64 event, void* data = nullptr);

		void AddLUT(const char* lable, Texture* tex);



		std::vector<std::string> m_LutLables;
		std::vector<Texture*> m_LutTextures;


		Inspector m_Inspector;

		CU::Matrix44f* m_ObjectMatrix = nullptr;

		Entity m_CurrEntity = 0;
		Entity m_PrevEntity = 0;
		Entity m_EditEntity = 0;
		static DebugHandle* m_Instance;

		void AddTimingObject(const std::string& view_tree_and_time_string);

		CU::GrowingArray<TimingObjectDisplay> m_TimingObjects;



		CU::GrowingArray<s32*> m_IntValuesToPrint;
		CU::GrowingArray<std::string> m_Text;/*
		CU::GrowingArray<DebugSlider<float>> m_Sliders;
		CU::GrowingArray<DebugTextValue<int>> m_Values;
		CU::GrowingArray<DebugCheckbox> m_Checkboxes;*/

		void* m_DebugTexture = nullptr;

		CU::GrowingArray<Texture*> m_RegisteredSampleTextures; //used for the debug textures
		CU::GrowingArray<void*> m_DebugTextures;
		std::vector<std::string> m_Labels;

		//These are the materials that you can choose from
		std::vector<Material*> m_Materials;
		std::vector<std::string> m_MaterialLabels;

		//this is my current modelinstances
		std::vector<ModelInstance*> m_ModelInstances;
		std::vector<std::string> m_InstanceLabels;


		s32 m_TextureIndex = 0;

		float m_CameraSpeed = 50.f;
		float m_ControllerLookSens = 0.005f;
		float m_MouseLookSense = 0.005f;
		float m_Range = 0.f;
		float m_CamPos[3];
		float m_CamRot[3];

		/**
#if !defined(_PROFILE) && !defined(_FINAL)
		bool SaveLevel();
		bool GetLineRendering();
		void EditEntity();
		void DebugTextures();
		void AddTexture(Texture* texture, const std::string& debug_name);
		void AddTexture(void* srv, const std::string& debug_name);

		void RegisterCheckBox(bool* pBool, const std::string& box_name)
		{
			CheckBox box;
			box.m_Name = box_name;
			box.m_Toggle = pBool;
			m_Checkboxes.Add(box);
		}

		void AddFunction(const std::string& label, std::function<void()> function);
		void AddCheckBox(bool* toggle, std::string label);
		void RegisterFloatSider(float* v, const char* label, float min, float max);
		CU::GrowingArray<void*>& GetDebugTextures() { return m_DebugTextures; }
	private:
		struct CheckBox
		{
			std::string m_Name;
			bool* m_Toggle = false;
		};
		CU::GrowingArray<CheckBox> m_Checkboxes;
		struct slider
		{
			float* current_value;
			const char* label;
			float min = 0.f;
			float max = 1.f;
		};
		CU::GrowingArray<slider> m_Sliders;
		void UpdateDebugUI();
		std::vector<std::string> m_Levels;
		typedef std::function<void()> callback;
		std::vector<std::pair<std::string, callback>> m_Functions;

#endif
		*/


	private:
		DebugHandle();
		~DebugHandle();
	};
};
#endif