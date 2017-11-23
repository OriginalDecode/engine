#pragma once
#include <Engine/engine_shared.h>
#include <DataStructures/GrowingArray.h>
#include <Math/Matrix/Matrix44.h>
#include <map>
#include <Engine/Model.h>

class Material;
namespace graphics
{
	class RenderContext;
}
struct InstanceObject
{
	InstanceObject() = default;
	Model* m_Model = nullptr;
	Material* m_Material = nullptr;
	bool m_Shadowed = false;
	CU::GrowingArray<GPUModelData> m_GPUData;
};

class InstancingManager
{
public:
	InstancingManager() = default;
	bool FindInstanceObject(u64 key);
	InstanceObject& AddInstanceObject(InstanceObject instance_object);
	const InstanceObject& GetInstanceObject(u64 key);

	void AddGPUDataToInstance(u64 key, GPUModelData data);

	void DoInstancing(const graphics::RenderContext& rc, bool shadowing);
	void EndFrame();

private:
	std::map<u64, InstanceObject> m_InstanceObjects; /* the key is the material */


};

