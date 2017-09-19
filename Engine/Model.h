#pragma once

#include <vector>

#include "BaseModel.h"
#include "engine_shared.h"
#include "Effect.h"
struct D3D11_INPUT_ELEMENT_DESC;

static Ticket_Mutex g_ModelMutex;
class Model : public BaseModel
{
	friend class CModelImporter;
public:
	Model() = default;
	~Model();
	virtual void Initiate(const std::string& filename);

	void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const graphics::RenderContext& render_context) override;
	void RenderInstanced(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context);
	void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context) override;
	void ShadowRenderInstanced(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context);





	void AddChild(Model* aChild);

	void SetPosition(const CU::Vector3f& aPosition);
	CU::Matrix44f& GetOrientation();
	void SetOrientation(CU::Matrix44f orientation);


	virtual void SetMaxPoint(CU::Vector3f max_point);
	virtual CU::Vector3f GetMaxPoint() const { return m_MaxPoint; }

	virtual void SetMinPoint(CU::Vector3f min_point);
	virtual CU::Vector3f GetMinPoint() const { return m_MinPoint; }

	std::vector<float> GetVertices();
	std::vector<s32> GetIndices();

	CU::GrowingArray<Surface*>& GetSurfaces() { return m_Surfaces; }
	CU::GrowingArray<Model*> GetChildModels() { return m_Children; }

	void AddTexture(const std::string& path, Effect::TextureSlot slot);


	void AddOrientation(CU::Matrix44f orientation);
	void CreateCube();

private:
	void RenderCube(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context);
	void RemoveOrientation();
	CU::GrowingArray<Model*> m_Children;

protected:
	void InitConstantBuffer();
	void UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& rc) override;
	
	CU::GrowingArray<Surface*> m_Surfaces;
	CU::GrowingArray<CU::Matrix44f> m_Orientations;

};
