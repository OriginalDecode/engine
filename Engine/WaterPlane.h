#pragma once
#include <Engine/BaseModel.h>
class WaterPlane : public BaseModel
{
public:
	WaterPlane() = default;

	void Initiate(const CU::Vector3f& position);

	void SetPosition(const CU::Vector3f& position);

	void UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const RenderContext& render_context);
	void Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context) override;
	void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context) override;

	const CU::Vector3f& GetPosition() { return m_Orientation.GetPosition(); }

private:
	void CreatePlane();
	void InitConstantBuffer() override;

	struct Vertice
	{
		Vertice() = default;
		Vertice(const CU::Vector4f& pos, const CU::Vector3f& normal)
			: m_Position(pos)
			, m_Normal(normal)
		{
		}
		CU::Vector4f m_Position;
		CU::Vector3f m_Normal;
	};

	struct cbMatrices
	{
		CU::Matrix44f m_World;
		CU::Matrix44f m_InvertedView;
		CU::Matrix44f m_Projection;
		CU::Vector4f m_Time;
	} m_VertexMatrices;

	CU::Matrix44f m_Orientation;
	CU::GrowingArray<SVertexPosNormUVBiTang> m_Vertices;
	CU::GrowingArray<int> m_Indices;

};

