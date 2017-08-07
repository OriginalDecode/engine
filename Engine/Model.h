#pragma once

#include <vector>

#include "BaseModel.h"
#include "engine_shared.h"

struct D3D11_INPUT_ELEMENT_DESC;

static Ticket_Mutex g_ModelMutex;
class Model : public BaseModel
{
	friend class CModelImporter;
public:
	Model() = default;
	void CleanUp();
	virtual void Initiate(const std::string& filename);

	void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const RenderContext& render_context) override;
	void RenderInstanced(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context);
	void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context) override;
	void ShadowRenderInstanced(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context);

	void AddChild(Model* aChild);

	void SetPosition(const CU::Vector3f& aPosition);
	CU::Matrix44f& GetOrientation();
	void SetOrientation(CU::Matrix44f orientation);
	CU::Vector3f GetWHD() const { return m_WHD; }
	void SetWHD(CU::Vector3f whd);


	virtual void SetMaxPoint(CU::Vector3f max_point);
	virtual CU::Vector3f GetMaxPoint() const { return m_MaxPoint; }

	virtual void SetMinPoint(CU::Vector3f min_point);
	virtual CU::Vector3f GetMinPoint() const { return m_MinPoint; }

	std::vector<float> GetVertices();
	std::vector<s32> GetIndices();

	CU::GrowingArray<Surface*>& GetSurfaces() { return mySurfaces; }
	CU::GrowingArray<Model*> GetChildModels() { return myChildren; }

	void AddOrientation(CU::Matrix44f orientation);
	void ClearOrientation();
	void CreateCube();
private:
	void RenderCube(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context);

	void RemoveOrientation();
	CU::GrowingArray<Model*> myChildren;
	CU::GrowingArray<CU::Matrix44f> m_Orientations;
	CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> m_InputLayoutDesc;
	IInputLayout* m_InstanceInputLayout = nullptr;


protected:
	void InitInstanceBuffer();
	void InitConstantBuffer();
	void UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context) override;

	CU::GrowingArray<SVertexTypePosCol> myVertices;
	CU::GrowingArray<s32> m_Indices;

	CU::GrowingArray<Surface*> mySurfaces;

	CU::Matrix44f myOrientation;

	IBuffer* m_InstanceBuffer = nullptr;
	D3D11_SUBRESOURCE_DATA m_InstanceData;
	const s32 m_InstanceCount = 500;
};
