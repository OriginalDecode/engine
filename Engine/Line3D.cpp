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
	Engine::GetAPI()->ReleasePtr(m_LineBuffer);
	m_LineBuffer = nullptr;
}

void Line3D::Initiate(int aLineAmount /*= 256*/)
{
	m_LineAmount = aLineAmount;
	m_Effect =  Engine::GetInstance()->GetEffect("Shaders/line.json");
	CreateBuffer();
	m_LineBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(m_cbLine), "Line3D ConstantBuffer");
}

void Line3D::Update(const LinePoint& first_point, const LinePoint& second_point, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Purple);
	m_FirstPoint = first_point;
	m_SecondPoint = second_point;
	m_Vertices.RemoveAll();
	m_Vertices.Add(m_FirstPoint);
	m_Vertices.Add(m_SecondPoint);

	IBuffer* pBuffer = m_VertexWrapper.GetVertexBuffer();
	render_context.GetContext().UpdateBuffer(pBuffer,
											 &m_Vertices[0], 
											 sizeof(LinePoint) * m_Vertices.Size(), 
											 graphics::MAP_WRITE_DISCARD);
}

void Line3D::Render(const CU::Matrix44f& prevOrientation, const CU::Matrix44f& projection, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Green);
	UpdateConstantBuffer(prevOrientation, projection, render_context);
	auto& ctx = render_context.GetContext();
	ctx.VSSetConstantBuffer(0, 1, &m_LineBuffer);
	ctx.Draw(this);
}

void Line3D::AddCube(const CU::Vector3f& min, const CU::Vector3f& max)
{
	float depth = max.z - min.z;
	float width = max.x - min.x;
	float height = max.y - min.y;

	CU::Vector3f point;
	CU::Vector3f nextPoint;

	LinePoint p1;
	LinePoint p2;

	point.x = min.x;
	point.y = min.y;
	point.z = min.z;

	point.x -= (width * 0.5f);
	point.y -= (height * 0.5f);
	point.z -= (depth * 0.5f);

	nextPoint = point;
	nextPoint.x += width;

	p1.position = point;
	p1.color = CU::Vector4f(1, 0, 1, 1);

	p2.position = nextPoint;
	p2.color = CU::Vector4f(1, 1, 1, 1);

	m_Vertices.Add(p2);
	m_Vertices.Add(p2);

	point = nextPoint;
	point.z += depth;

	nextPoint = point;
	nextPoint.x -= width;

	p1.position = point;
	p1.color = CU::Vector4f(0, 1, 1, 1);

	p2.position = nextPoint;
	p2.color = CU::Vector4f(0, 0, 1, 1);

	m_Vertices.Add(p2);
	m_Vertices.Add(p2);

	point = nextPoint;
	point.y += height;

	nextPoint = point;
	nextPoint.z -= depth;

	p1.position = point;
	p1.color = CU::Vector4f(1.f, 150.f/255.f, 0, 1);

	p2.position = nextPoint;
	p2.color = CU::Vector4f(1, 0, 0, 1);

	m_Vertices.Add(p2);
	m_Vertices.Add(p2);

	point = nextPoint;
	point.x += width;

	nextPoint = point;
	nextPoint.z += depth;


	p1.position = point;
	p1.color = CU::Vector4f(1.f, 1.f, 0, 1);

	p2.position = nextPoint;
	p2.color = CU::Vector4f(0, 1, 0, 1);

	m_Vertices.Add(p2);
	m_Vertices.Add(p2);

}



void Line3D::CreateBuffer()
{

	graphics::InputElementDesc desc;
	desc.m_SemanicIndex = 0;
	desc.m_ByteOffset = 0;
	desc.m_ElementSpecification = graphics::INPUT_PER_VERTEX_DATA;
	desc.m_InputSlot = 0;
	desc.m_InstanceDataStepRate = 0;

	graphics::InputElementDesc layout[] = {
		{ "POSITION", 0, graphics::_16BYTE_RGBA, 1, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, graphics::_16BYTE_RGBA, 1, 16, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};
	auto& pDevice = Engine::GetAPI()->GetDevice();
	IInputLayout* input_layout = pDevice.CreateInputLayout(m_Effect->GetVertexShader(), layout, ARRSIZE(layout));

	graphics::BufferDesc buffer_desc;
	buffer_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	buffer_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	buffer_desc.m_CPUAccessFlag = graphics::WRITE;
	buffer_desc.m_ByteWidth = sizeof(LinePoint) * m_LineAmount;

	IBuffer* vertex_buffer = pDevice.CreateBuffer(buffer_desc, "Line3D VertexBuffer");

	m_VertexWrapper.SetStart(0);
	m_VertexWrapper.SetStride(sizeof(m_cbLine));
	m_VertexWrapper.SetByteOffset(0);
	m_VertexWrapper.SetVertexCount(2);
	m_VertexWrapper.SetSize(buffer_desc.m_ByteWidth);
	m_VertexWrapper.SetInputLayout(input_layout);
	m_VertexWrapper.SetTopology(graphics::POINT_LIST);
	m_VertexWrapper.SetBuffer(vertex_buffer);

#ifdef _DEBUG
	m_VertexWrapper.m_DebugName = DEBUG_NAME("Line", Line3D);
#endif
}


void Line3D::UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	auto& ctx = render_context.GetContext();
	m_cbLine.m_World = m_Orientation;
 	m_cbLine.m_View = CU::Math::Inverse(camera_orientation);
 	m_cbLine.m_Projection = camera_projection;

	//ctx.UpdateConstantBuffer(m_LineBuffer, nullptr, sizeof(VertexBaseStruct));
}
