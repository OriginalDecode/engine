#include "stdafx.h"
#include "Line3D.h"
#include <DL_Debug.h>

Line3D::Line3D()
	: m_FirstPoint({ 0, 0, 0, 1 }, { 1, 1, 1, 1 })
	, m_SecondPoint({ 0, 0, 0, 1 }, { 1, 1, 1, 1 })
{
}

Line3D::~Line3D()
{
}

void Line3D::Initiate(int aLineAmount /*= 256*/)
{
	m_LineAmount = aLineAmount;
	m_Effect = Engine::GetInstance()->GetEffect("Shaders/line.json");
	m_LineBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(CU::Matrix44f), "Line Constant Buffer");
	CreateBuffer();
}

void Line3D::Render(LinePoint points[2], const graphics::RenderContext& rc)
{
	PROFILE_FUNCTION(profiler::colors::Green);

	m_FirstPoint = points[0];
	m_SecondPoint = points[1];
	m_Vertices.RemoveAll();
	m_Vertices.Add(m_FirstPoint);
	m_Vertices.Add(m_SecondPoint);

	auto& ctx = rc.GetContext();
	IBuffer* pBuffer = m_VertexWrapper.GetVertexBuffer();
	ctx.UpdateBuffer(pBuffer,
					 &m_Vertices[0],
					 sizeof(LinePoint) * m_Vertices.Size(),
					 graphics::MAP_WRITE_DISCARD);

 	ctx.UpdateConstantBuffer(m_LineBuffer, &m_Orientation[0]);

	ctx.SetBlendState(rc.GetAPI().GetBlendState(graphics::NO_BLEND));
	ctx.Draw(this);
}

void Line3D::CreateBuffer()
{
	graphics::InputElementDesc layout[] = {
		{ "POSITION", 0, graphics::_16BYTE_RGBA, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, graphics::_16BYTE_RGBA, 0, 16, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};
	auto& pDevice = Engine::GetAPI()->GetDevice();
	IInputLayout* input_layout = pDevice.CreateInputLayout(m_Effect->GetVertexShader(), layout, ARRSIZE(layout));

	graphics::BufferDesc buffer_desc;
	buffer_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	buffer_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	buffer_desc.m_CPUAccessFlag = graphics::WRITE;
	buffer_desc.m_Size = sizeof(LinePoint) * m_LineAmount;
	buffer_desc.m_ByteWidth = buffer_desc.m_Size;

	IBuffer* vertex_buffer = pDevice.CreateBuffer(buffer_desc, "Line3D VertexBuffer");

	m_VertexWrapper.SetStart(0);
	m_VertexWrapper.SetStride(sizeof(LinePoint));
	m_VertexWrapper.SetByteOffset(0);
	m_VertexWrapper.SetVertexCount(2);
	m_VertexWrapper.SetSize(buffer_desc.m_ByteWidth);
	m_VertexWrapper.SetInputLayout(input_layout);
	m_VertexWrapper.SetTopology(graphics::LINE_LIST);
	m_VertexWrapper.SetBuffer(vertex_buffer);

#ifdef _DEBUG
	m_VertexWrapper.m_DebugName = DEBUG_NAME("Line", Line3D);
#endif
}
