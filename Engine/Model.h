#pragma once

#include <vector>

#include "BaseModel.h"
#include "snowblind_shared.h"

struct D3D11_INPUT_ELEMENT_DESC;

enum eModelStates : int
{
	LIGHT_MESH,
	_COUNT
};

static Ticket_Mutex g_ModelMutex;
class CModel
{
	friend class CModelImporter;
public:
	void Initiate(const std::string& filename);
	virtual void CleanUp();
	virtual void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, bool render_shadows = false);
	virtual void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection);
	void AddChild(CModel* aChild);
	//void SetIsLightmesh();
	void SetPosition(const CU::Vector3f& aPosition);
	CU::Matrix44f& GetOrientation();
	void SetOrientation(CU::Matrix44f orientation);

	void SetMaxPoint(CU::Vector3f max_point);
	CU::Vector3f GetMaxPoint() const { return m_MaxPoint; }

	void SetMinPoint(CU::Vector3f min_point);
	CU::Vector3f GetMinPoint() const { return m_MinPoint; }

	std::vector<float> GetVertices();
	std::vector<s32> GetIndices();

	CU::GrowingArray<CSurface*>& GetSurfaces() { return mySurfaces; }
	CU::GrowingArray<CModel*> GetChildModels() { return myChildren; }

private:
	CU::GrowingArray<SVertexTypePosCol> myVertices;
	CU::GrowingArray<s32> m_Indices;

	CU::GrowingArray<CSurface*> mySurfaces;
	CU::GrowingArray<CModel*> myChildren;

	CU::Matrix44f myOrientation;
	std::bitset<eModelStates::_COUNT> myModelStates;

protected:
	std::string m_Filename;

	void SetupLayoutsAndBuffers();
	virtual void InitConstantBuffer();
	virtual void UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection);

	void InitVertexBuffer();
	void InitIndexBuffer();

	CU::Vector3f m_MaxPoint;
	CU::Vector3f m_MinPoint;


	Effect* myEffect = nullptr;
	IInputLayout* m_VertexLayout = nullptr;
	CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;

	IBuffer* myConstantBuffer= nullptr;

	struct cbVertex
	{
		CU::Matrix44f m_World;
		CU::Matrix44f m_InvertedView;
		CU::Matrix44f m_Projection;
	} m_ConstantStruct;

	bool m_IsRoot = true;


	IndexDataWrapper	m_IndexData;
	VertexDataWrapper	m_VertexData;

	VertexBufferWrapper m_VertexBuffer;
	IndexBufferWrapper	m_IndexBuffer;

};
