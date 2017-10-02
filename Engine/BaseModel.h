#pragma once
#include "VertexStructs.h"
#include <Math/Matrix/Matrix44.h>
#include <DataStructures/GrowingArray.h>
#include "engine_shared.h"
#include "VertexWrapper.h"
#include "IndexWrapper.h"
#include "InstanceWrapper.h"

class DirectX11;
class Effect;
class Engine;
class Surface;

namespace graphics
{
	class RenderContext;
};
class BaseModel
{
	friend class CModelImporter;
public:
	BaseModel() = default;
	virtual ~BaseModel() = 0;
	virtual void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const graphics::RenderContext& render_context) = 0;
	virtual void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context) = 0;

	void SetEffect(Effect* anEffect);
	const VertexWrapper& GetVertexWrapper() const { return m_VertexWrapper; }
	VertexWrapper& GetVertexWrapper() { return m_VertexWrapper; }
	const IndexWrapper& GetIndexWrapper() const { return m_IndexWrapper; }
	IndexWrapper& GetIndexWrapper() { return m_IndexWrapper; }
	const InstanceWrapper& GetInstanceWrapper() const { return m_InstanceWrapper; }
	InstanceWrapper& GetInstanceWrapper() { return m_InstanceWrapper; }

	CU::Vector3f GetWHD() const { return m_WHD; }

	virtual void AddSurface(Surface* surface) = 0;

protected:
	virtual void UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const graphics::RenderContext& rc) = 0;
	CU::Matrix44f m_Orientation;
	CU::GrowingArray<Surface*> m_Surfaces;

	CU::Vector3f m_WHD;
	CU::Vector3f m_MaxPoint;
	CU::Vector3f m_MinPoint;
	Effect* m_Effect = nullptr;

	VertexWrapper m_VertexWrapper;
	IndexWrapper m_IndexWrapper;
	InstanceWrapper m_InstanceWrapper;

	std::string m_FileName;
	IBuffer* m_ConstantBuffer = nullptr;
	struct cbVertex
	{
		CU::Matrix44f m_World;
		CU::Matrix44f m_InvertedView;
		CU::Matrix44f m_Projection;
	} m_ConstantStruct;

	bool m_IsRoot = true;
};
