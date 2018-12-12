#pragma once
#include <Engine/engine_shared.h>
#include <DataStructures/GrowingArray.h>
#include <Math/Matrix/Matrix44.h>
#include <map>
#include <Engine/Model.h>
#include <RefPointer.h>

class Material;
namespace graphics
{
	class RenderContext;
}
struct InstanceObject
{
	InstanceObject()
	{
		m_GPUData.Init(600);
	}
	RefPointer<Model> m_Model;
	Material* m_Material = nullptr;
	bool m_Shadowed = false;
	CU::GrowingArray<GPUModelData> m_GPUData;

	UINT64 m_Start = 0;
	UINT64 m_End = 0;
};

class InstancingManager
{
public:
	InstancingManager();
	bool FindInstanceObject(uint64 key);
	bool FindModel(uint64 key) const;
	void AddInstanceObject(InstanceObject instance_object);
//	const InstanceObject& GetInstanceObject(u64 key, u64 model_key);

	void AddGPUDataToInstance(uint64 material_key, uint64 model_key, GPUModelData data);

	void DoInstancing(const graphics::RenderContext& rc, bool shadowing);
	void EndFrame();
private:
	std::map<uint64, std::map<uint64, InstanceObject>> m_InstanceObjects; /* the key is the material */


};

