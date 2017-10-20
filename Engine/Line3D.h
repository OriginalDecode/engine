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

	void Initiate(int aLineAmount = 1024);
	void Render(LinePoint points[2], const graphics::RenderContext& render_context);

	VertexWrapper& GetVertexWrapper() { return m_VertexWrapper; }
	Effect* GetEffect() const { return m_Effect; }
	void AddLine(LinePoint points[2]);
private:
	void CreateBuffer();
	CU::GrowingArray<LinePoint> m_Points;

	LinePoint m_FirstPoint;
	LinePoint m_SecondPoint;

	CU::Matrix44f m_Orientation;
	Effect* m_Effect = nullptr;
	VertexWrapper m_VertexWrapper;

	IBuffer* m_LineBuffer;

	CU::GrowingArray<LinePoint> m_Vertices;
	int m_LineAmount = 0;
};

