#include "stdafx.h"
#include "BaseModel.h"
#include <stdio.h>
#ifdef _PROFILE
#include <easy/profiler.h>
#endif
#include <Engine/IGraphicsContext.h>

BaseModel::~BaseModel() = default;

void BaseModel::SetEffect(Effect* anEffect)
{
	m_Effect = anEffect;
}

void BaseModel::SetSurface0(Surface* surface)
{
	m_Surfaces[0] = surface;
}

void BaseModel::CreatePlane(float half_width)
{
	auto& device = Engine::GetAPI()->GetDevice();

	graphics::InputElementDesc desc[] =
	{
		{ "POSITION", 0, graphics::_16BYTE_RGBA, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, graphics::_12BYTE_RGB, 0, 16, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, graphics::_8BYTE_RG, 0, 28, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, graphics::_12BYTE_RGB, 0, 36, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, graphics::_12BYTE_RGB, 0, 48, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};

	IInputLayout* pInputLayout = device.CreateInputLayout(m_Effect->GetVertexShader(), desc, ARRSIZE(desc));

	CU::GrowingArray<SVertexPosNormUVBiTang> vertices;

	SVertexPosNormUVBiTang vert;
	vert.position = { -half_width, 0, half_width, 1};
	vert.normal = { 0, 1, 0 };
	vert.uv = { 0, 0 };
	vertices.Add(vert);

	vert.position = { -half_width, 0, -half_width, 1 };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 0, 1 };

	vertices.Add(vert);

	vert.position = { half_width, 0, half_width, 1 };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 1, 0 };
	vertices.Add(vert);

	vert.position = { half_width, 0, -half_width, 1 };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 1, 1 };
	vertices.Add(vert);

	const s32 vtx_stride = sizeof(SVertexPosNormUVBiTang);
	const s32 vtx_count = vertices.Size();
	const s32 vtx_size = vtx_count * vtx_stride;
	const s32 vtx_buff_count = 1;
	const s32 vtx_start = 0;
	const s32 vtx_byte_offset = 0;
	s8* vtx_data = new s8[vtx_size];
	memcpy(vtx_data, &vertices[0], vtx_size);


	graphics::BufferDesc vtx_desc;
	vtx_desc.m_Size = vtx_size;
	vtx_desc.m_Data = vtx_data;
	vtx_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	vtx_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	vtx_desc.m_CPUAccessFlag = graphics::WRITE;
	vtx_desc.m_ByteWidth = vtx_size;

	char temp[100];
	sprintf_s(temp, 100, "%s Vertex Buffer", typeid(this).name());
	IBuffer* vtx_buffer = device.CreateBuffer(vtx_desc, temp);



	m_VertexWrapper.SetData(vtx_data);
	m_VertexWrapper.SetStart(vtx_start);
	m_VertexWrapper.SetStride(vtx_stride);
	m_VertexWrapper.SetByteOffset(vtx_byte_offset);
	m_VertexWrapper.SetVertexCount(vtx_count);
	m_VertexWrapper.SetSize(vtx_size);
	m_VertexWrapper.SetBuffer(vtx_buffer);
	m_VertexWrapper.SetInputLayout(pInputLayout);
	m_VertexWrapper.SetTopology(graphics::TRIANGLE_LIST);

	CU::GrowingArray<u32> indices;
	indices.Add(0);
	indices.Add(1);
	indices.Add(2);

	indices.Add(3);
	indices.Add(2);
	indices.Add(1);

	const s32 idx_count = indices.Size();
	const s32 idx_stride = sizeof(u32);
	const s32 idx_size = idx_count * idx_stride;
	const s32 idx_start = 0;
	const s32 idx_byte_offset = 0;

	s8* idx_data = new s8[idx_size];
	memcpy(idx_data, &indices[0], idx_size);

	graphics::BufferDesc idx_desc;
	idx_desc.m_Size = idx_size;
	idx_desc.m_Data = idx_data;
	idx_desc.m_BindFlag = graphics::BIND_INDEX_BUFFER;
	idx_desc.m_UsageFlag = graphics::IMMUTABLE_USAGE;
	idx_desc.m_StructuredByteStride = 0;
	idx_desc.m_CPUAccessFlag = graphics::NO_ACCESS_FLAG;
	idx_desc.m_MiscFlags = 0;
	idx_desc.m_ByteWidth = idx_desc.m_Size;


	sprintf_s(temp, 100, "%s Index Buffer", typeid(this).name());
	IBuffer* idx_buffer = Engine::GetAPI()->GetDevice().CreateBuffer(idx_desc, temp);

	m_IndexWrapper.SetData(idx_data);
	m_IndexWrapper.SetIndexCount(idx_count);
	m_IndexWrapper.SetStart(idx_start);
	m_IndexWrapper.SetSize(idx_size);
	m_IndexWrapper.SetFormat(graphics::R32_UINT);
	m_IndexWrapper.SetByteOffset(idx_byte_offset);
	m_IndexWrapper.SetBuffer(idx_buffer);

}
