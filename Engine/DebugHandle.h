#pragma once

#if !defined(_PROFILE) && !defined(_FINAL)
#include <CommonLib/DataStructures/GrowingArray.h>
#include <CommonLib/Math/Matrix/Matrix.h>
#include <CommonLib/EventManager/Subscriber.h>

#include <Engine/Inspector.h>

class Texture;
class Material;

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

		void AddButton(const char* button_name);

		void Information();

		void WriteChildren(const CU::GrowingArray<Model *>& children);

		void ChildRecursive(const TimingObjectDisplay &root);

		void DebugTextures();

		void AddText(std::string str);
		void SetEntity(Entity e);
		Entity GetHoveredEntity() const;
		Entity GetSelectedEntity() const;

		//void RegisterCheckbox(DebugCheckbox checkbox);

		void RegisterMaterial(Material* pMaterial, std::string lable);
		void ConfirmEntity();
		int32 GetDebugTextureIndex() const;

		void SetDebugTexture(Texture* tex);

		void RegisterTexture(Texture* texture, const char* name, const char* category = "null");
		void UnregisterTexture(Texture* t, const char* name);
		Texture* GetTexture(int32 index);

		void SetObjectMatrix(CU::Matrix44f* mat);

		void HandleEvent(uint64 event, void* data = nullptr);

		void AddLUT(const char* lable, Texture* tex);
		void AddTimingObject(const std::string& view_tree_and_time_string);

		float m_ControllerLookSens = 0.005f;
		float m_MouseLookSense = 0.005f;
		float m_CameraSpeed = 50.f;


		void SetHoveredModel(HashType hash) { m_HoveredModel = hash; }
		const HashType& GetModelHash() const { return m_SelectedModel; }
		void SetTextureToDraw(Texture* tex) { m_TextureToDraw = tex; }



		void RegisterVariable(const char* lable, bool* boolean);
		void RegisterVariable(const char* lable, float* floating);
		void RegisterVariable(const char* lable, int* integer);

		
		bool IsActive() const { return m_IsActive; }
	private:
		bool m_IsActive = true;
		enum eVariableType
		{
			eBool,
			eInteger,
			eFloat,
			nof_var_types
		};

	struct DebugVariable
	{
		DebugVariable(void* var, const char* lable)
			: m_Variable(var)
			, m_Lable(lable)
		{
		}

		void* m_Variable = nullptr;
		std::string m_Lable;
	};


		std::vector<DebugVariable> m_DebugVariables[nof_var_types];


		Texture* m_TextureToDraw = nullptr;
		void ConfirmModel();
		HashType m_SelectedModel;
		HashType m_HoveredModel;

		std::vector<std::string> m_LutLables;
		std::vector<Texture*> m_LutTextures;
		Inspector m_Inspector;
		CU::Matrix44f* m_ObjectMatrix = nullptr;
		Entity m_CurrEntity = 0;
		Entity m_PrevEntity = 0;
		Entity m_EditEntity = 0;


		CU::GrowingArray<TimingObjectDisplay> m_TimingObjects;
		CU::GrowingArray<std::string> m_Text;

		//______________________________________________

		void* m_DebugTexture = nullptr;
		CU::GrowingArray<Texture*> m_RegisteredSampleTextures; //used for the debug textures
		struct DebugTextureCategory
		{
			DebugTextureCategory() = default;
			DebugTextureCategory(std::string name) : category(name){}
			std::string category;
			std::vector<std::string> labels;
		};
		std::vector<DebugTextureCategory> m_Categories;

		//______________________________________________

		//These are the materials that you can choose from
		std::vector<Material*> m_Materials;
		std::vector<std::string> m_MaterialLabels;

		//this is my current modelinstances
		std::vector<ModelInstanceCmpt*> m_ModelInstances;
		std::vector<std::string> m_InstanceLabels;


		int32 m_TextureIndex = 0;

		float m_Range = 0.f;
		float m_CamPos[3];
		float m_CamRot[3];
		float s_ShadowDir[3];


		static DebugHandle* m_Instance;

		DebugHandle();
		~DebugHandle();
	};
};
#endif