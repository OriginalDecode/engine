#pragma once
#include "VertexStructs.h"
#include <Math/Matrix/Matrix44.h>
#include <DataStructures/GrowingArray.h>
#include "VertexWrapper.h"
#include "IndexWrapper.h"
struct D3D11_INPUT_ELEMENT_DESC;

class DirectX11;
class Effect;
class Engine;
class Surface;

struct IndexDataWrapper;
struct VertexBufferWrapper;
struct VertexDataWrapper;
struct IndexBufferWrapper;

class BaseModel
{
public:
	BaseModel() = default;
	virtual ~BaseModel() = 0;
	virtual void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) = 0;
	virtual void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection) = 0;

	void SetupLayoutsAndBuffers();

	void SetEffect(Effect* anEffect);

protected:
	void InitVertexBuffer();
	void InitInputLayout();
	void InitIndexBuffer();
	virtual void InitConstantBuffer() = 0;
	virtual void UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) = 0;

	std::string m_Filename;
	CU::Vector3f m_WHD; //to be removed
	CU::Vector3f m_MaxPoint;
	CU::Vector3f m_MinPoint;

	Effect* myEffect = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;
	CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;

	VertexDataWrapper m_VertexData;
	VertexBufferWrapper m_VertexBuffer;
	IndexDataWrapper m_IndexData;
	IndexBufferWrapper m_IndexBuffer;

	ID3D11Buffer* myConstantBuffer = nullptr;

	struct cbVertex
	{
		CU::Matrix44f m_World;
		CU::Matrix44f m_InvertedView;
		CU::Matrix44f m_Projection;
	} m_ConstantStruct;

	bool m_IsRoot = true;
};
