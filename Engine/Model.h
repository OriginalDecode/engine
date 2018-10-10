#pragma once

#include <vector>

#include "BaseModel.h"
#include "engine_shared.h"
#include "Effect.h"
struct D3D11_INPUT_ELEMENT_DESC;

struct GPUModelData
{
	CU::Matrix44f m_Orientation;
	CU::Vector4f dummy;
	u32 m_ID;
	u32 m_Hovering = false;
};


class Model : public BaseModel
{
	friend class CModelImporter;
	friend class InstancingManager;
public:
	Model() = default;
	~Model();
	virtual void Initiate(const std::string& filename);

	void Render(const graphics::RenderContext& rc) override;
	void RenderInstanced(const graphics::RenderContext& rc);
	void RenderInstanced(const graphics::RenderContext& rc, Effect* override_effect);
	void ShadowRender(const graphics::RenderContext& rc) override;
	void ShadowRenderInstanced(const graphics::RenderContext& rc);



	void AddChild(Model* aChild);

	void AddSurface(Surface* surface);

	void SetPosition(const CU::Vector4f& aPosition) override;
	void SetForward(const CU::Vector4f& d) override;
	const CU::Matrix44f& GetOrientation() const override;
	void SetOrientation(const CU::Matrix44f orientation) override;

	virtual void SetMaxPoint(CU::Vector3f max_point);
	virtual CU::Vector3f GetMaxPoint() const { return m_MaxPoint; }

	virtual void SetMinPoint(CU::Vector3f min_point);
	virtual CU::Vector3f GetMinPoint() const { return m_MinPoint; }

	CU::GrowingArray<Surface*>& GetSurfaces() { return m_Surfaces; }
	CU::GrowingArray<Model*> GetChildModels() { return m_Children; }

	void AddTexture(const std::string& path, TextureSlot slot);

	s32 GetInstanceCount() const { return m_GPUData.Size(); }
	Effect* GetEffect() { return m_Effect; }
	void AddOrientation(CU::Matrix44f orientation);
	void AddInstanceData(GPUModelData data);

	void SetMaterial(Material* pMaterial) override;
	Material* GetMaterial();
	void CreateCube();

	void SetKey(u64 key) { m_Key = key; }
	const u64 GetKey() const { return m_Key; }

	void SetEntityID(int id);

	void SetIsInstanced(bool is_instanced) override;
	bool IsInstanced() const { return m_IsInstanced; }

	void SetSurface0(Surface* surface) override;
	Surface* GetSurface();
private:
	void RenderCube(const graphics::RenderContext& rc);
	void RemoveGPUData();
	CU::GrowingArray<Model*> m_Children;

	IBuffer* m_ModelID = nullptr;
	int m_ModelIDHash = 0;
protected:
	u64 m_Key = 0;
	void UpdateConstantBuffer(const graphics::RenderContext& rc) override;
	CU::GrowingArray<GPUModelData> m_GPUData;
};
