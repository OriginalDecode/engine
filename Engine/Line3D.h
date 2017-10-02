#pragma once
#include "engine_shared.h"
#include <Math/Matrix/Matrix44.h>
#include <DataStructures/GrowingArray.h>

#include <Engine/VertexWrapper.h>

struct LinePoint
{
	LinePoint(const CU::Vector4f& pos, const CU::Vector4f& col)
		: position(pos)
		, color(col)
	{

	}
	LinePoint() { color = { 255.f, 255.f, 255.f, 255.f }; }
	CU::Vector4f position;
	CU::Vector4f color;
};

struct Line
{
	LinePoint first;
	LinePoint second;
};


namespace graphics
{
	class RenderContext;
};
class Effect;
struct VertexBaseStruct;
class Line3D
{
public:
	Line3D();
	~Line3D();

	void Initiate(int aLineAmount = 256);


	void Update(const LinePoint& firstPoint, const LinePoint& secondPoint, const graphics::RenderContext& render_context);
	void Render(const CU::Matrix44f& prevOrientation, const CU::Matrix44f& projection, const graphics::RenderContext& render_context);
	void AddCube(const CU::Vector3f& min, const CU::Vector3f& max);

	VertexWrapper& GetVertexWrapper() { return m_VertexWrapper; }
	Effect* GetEffect() const { return m_Effect; }
private:
	void CreateBuffer();

	void UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context);

	LinePoint m_FirstPoint;
	LinePoint m_SecondPoint;

	CU::Matrix44f m_Orientation;
	Effect* m_Effect = nullptr;
	VertexWrapper m_VertexWrapper;

	IBuffer* m_LineBuffer;
	struct cbLine
	{
		CU::Matrix44f m_World;
		CU::Matrix44f m_View;
		CU::Matrix44f m_Projection;
	} m_cbLine;


	CU::GrowingArray<LinePoint> m_Vertices;
	int m_LineAmount = 0;
};

