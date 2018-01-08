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
	m_LineAmount = 2048;
	m_Vertices.Init(m_LineAmount * 2);
	m_Effect = Engine::GetInstance()->GetEffect("Shaders/line.json");
	m_LineBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(CU::Matrix44f), "Line Constant Buffer");
	CreateBuffer();
}

void Line3D::Render(const graphics::RenderContext& rc)
{
	PROFILE_FUNCTION(profiler::colors::Green);

	if (m_Vertices.Empty())
		return;
	auto& ctx = rc.GetContext();
	m_VertexWrapper.SetVertexCount(m_Vertices.Size());
	IBuffer* pBuffer = m_VertexWrapper.GetVertexBuffer();
	ctx.UpdateBuffer(pBuffer,
					 &m_Vertices[0],
					 sizeof(LinePoint) * m_Vertices.Size(),
					 graphics::MAP_WRITE_DISCARD);


	ctx.SetBlendState(rc.GetAPI().GetBlendState(graphics::NO_BLEND));
	ctx.Draw(this);
	m_Vertices.RemoveAll();
}

void Line3D::AddLine(LinePoint points[2])
{
	if (m_Vertices.Size() >= (m_LineAmount*2))
		return;

	m_Vertices.Add(points[0]);
	m_Vertices.Add(points[1]);
}

void Line3D::CreateBuffer()
{
	graphics::InputElementDesc layout[] = {
		{ "POSITION", 0, graphics::_16BYTE_RGBA, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, graphics::_16BYTE_RGBA, 0, 16, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};
	auto& device = Engine::GetAPI()->GetDevice();
	IInputLayout* input_layout = device.CreateInputLayout(m_Effect->GetVertexShader(), layout, ARRSIZE(layout));

	graphics::BufferDesc buffer_desc;
	buffer_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	buffer_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	buffer_desc.m_CPUAccessFlag = graphics::WRITE;
	buffer_desc.m_Size = sizeof(LinePoint) * (m_LineAmount * 2);
	buffer_desc.m_ByteWidth = buffer_desc.m_Size;

	IBuffer* vertex_buffer = device.CreateBuffer(buffer_desc, "Line3D VertexBuffer");

	m_VertexWrapper.SetStart(0);
	m_VertexWrapper.SetStride(sizeof(LinePoint));
	m_VertexWrapper.SetByteOffset(0);
	m_VertexWrapper.SetVertexCount(1);
	m_VertexWrapper.SetSize(buffer_desc.m_ByteWidth);
	m_VertexWrapper.SetInputLayout(input_layout);
	m_VertexWrapper.SetTopology(graphics::LINE_LIST);
	m_VertexWrapper.SetBuffer(vertex_buffer);

#ifdef _DEBUG
	m_VertexWrapper.m_DebugName = DEBUG_NAME("Line", Line3D);
#endif
}
