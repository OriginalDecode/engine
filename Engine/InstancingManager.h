#pragma once
#include <Engine/engine_shared.h>
#include <DataStructures/GrowingArray.h>
#include <Math/Matrix/Matrix44.h>
#include <map>

class Model;
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
	CU::GrowingArray<CU::Matrix44f> m_Orientations;
};

class InstancingManager
{
public:
	InstancingManager() = default;
	bool FindInstanceObject(u64 key);
	InstanceObject& AddInstanceObject(InstanceObject instance_object);
	const InstanceObject& GetInstanceObject(u64 key);
	void AddOrientationToInstance(u64 key, const CU::Matrix44f& orientation);

	void DoInstancing(const graphics::RenderContext& rc);

private:
	std::map<u64, InstanceObject> m_InstanceObjects; /* the key is the material */


};

