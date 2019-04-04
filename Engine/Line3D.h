#pragma once
#include "engine_shared.h"
#include <Math/Matrix/Matrix44.h>
#include <DataStructures/GrowingArray.h>

#include <Engine/VertexWrapper.h>



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
	void Render(const graphics::RenderContext& render_context);

	VertexWrapper& GetVertexWrapper() { return m_VertexWrapper; }
	Effect* GetEffect() const { return m_Effect; }
	void AddLine(LinePoint points[2]);
	void AddLine(LinePoint first, LinePoint second);
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

