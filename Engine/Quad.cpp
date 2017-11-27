#include "stdafx.h"
#include "Quad.h"
#include <Engine/Engine.h>
#include <Engine/IGraphicsAPI.h>
#include <Engine/IGraphicsDevice.h>
#include <Engine/IGraphicsContext.h>

Quad::Quad(Effect* effect)
	: m_Effect(effect)
{
	FillVertexData(1.f, 1.f);
	FillIndexData();
}

Quad::Quad(Effect* effect, float half_width, float half_height)
	: m_Effect(effect)
{
	FillVertexData(half_width, half_height);
	FillIndexData();
}

Quad::Quad()
	: m_Effect(nullptr)
{
	FillVertexData(1.f, 1.f);
	FillIndexData();
}

void Quad::FillVertexData(float half_width, float half_height)
{
	auto& device = Engine::GetAPI()->GetDevice();
	// Vertex
	const s32 vtx_count = 4;
	const s32 stride = sizeof(VertexTypePosUV);
	const s32 size = vtx_count * stride;
	const s32 vtx_start = 0;
	const s32 vtx_byte_offset = 0;
	const s32 vtx_buffer_count = 1;
	s8* data = new s8[size];

	VertexTypePosUV vertices[vtx_count];
	vertices[0].myPosition = { -half_width, -half_height, 0, 1 };
	vertices[0].myUV = { 0, 1 };

	vertices[1].myPosition = { -half_width, half_height, 0 , 1 };
	vertices[1].myUV = { 0, 0 };

	vertices[2].myPosition = { half_width, -half_height, 0 , 1 };
	vertices[2].myUV = { 1, 1 };

	vertices[3].myPosition = { half_width, half_height, 0 , 1 };
	vertices[3].myUV = { 1, 0 };

	memcpy(data, &vertices[0], size);


	graphics::BufferDesc buf_desc;
	buf_desc.m_Size = size;
	buf_desc.m_Data = data;
	buf_desc.m_ByteWidth = size;
	buf_desc.m_CPUAccessFlag = graphics::WRITE;
	buf_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	buf_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;

	IBuffer* vertex_buffer = device.CreateBuffer(buf_desc, "Quad VertexBuffer");

	graphics::InputElementDesc desc[] = {
		{ "POSITION", 0, graphics::_16BYTE_RGBA, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, graphics::_8BYTE_RG, 0, 16, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};

	if (!m_Effect)
		m_Effect = Engine::GetInstance()->GetEffect("Shaders/render_to_texture.json");
	IInputLayout* layout = device.CreateInputLayout(m_Effect->GetVertexShader(), desc, ARRSIZE(desc));

	m_VertexWrapper.SetData(data);
	m_VertexWrapper.SetVertexCount(vtx_count);
	m_VertexWrapper.SetStart(vtx_start);
	m_VertexWrapper.SetStride(stride);
	m_VertexWrapper.SetByteOffset(vtx_byte_offset);
	m_VertexWrapper.SetSize(size);
	m_VertexWrapper.SetBuffer(vertex_buffer);
	m_VertexWrapper.SetInputLayout(layout);
	m_VertexWrapper.SetTopology(graphics::TRIANGLE_LIST);

#ifdef _DEBUG
	m_VertexWrapper.m_DebugName = DEBUG_NAME_A(m_Effect->GetFileName(), Quad);
#endif
}

void Quad::FillIndexData()
{
	auto& device = Engine::GetAPI()->GetDevice();
	const graphics::eTextureFormat format = graphics::R32_UINT;
	const s32 index_count = 6;
	const s32 index_size = index_count * 4; // there's 4 vertices, could that be the thing?
	s8* index_data = new s8[index_size];
	const s32 index_start = 0;
	const s32 index_byte_offset = 0;

	s32 indices[index_count];
	indices[0] = 1;
	indices[1] = 0;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 1;
	memcpy(index_data, &indices[0], index_size);

	graphics::BufferDesc idx_desc;
	idx_desc.m_Size = index_size;
	idx_desc.m_Data = index_data;
	idx_desc.m_ByteWidth = index_size;
	idx_desc.m_CPUAccessFlag = graphics::NO_ACCESS_FLAG;
	idx_desc.m_BindFlag = graphics::BIND_INDEX_BUFFER;
	idx_desc.m_UsageFlag = graphics::DEFAULT_USAGE;

	IBuffer* index_buffer = device.CreateBuffer(idx_desc, "Quad IndexBuffer");

	m_IndexWrapper.SetData(index_data);
	m_IndexWrapper.SetIndexCount(index_count);
	m_IndexWrapper.SetStart(index_start);
	m_IndexWrapper.SetSize(index_size);
	m_IndexWrapper.SetFormat(format);
	m_IndexWrapper.SetByteOffset(index_byte_offset);
	m_IndexWrapper.SetBuffer(index_buffer);
#ifdef _DEBUG
	m_IndexWrapper.m_DebugName = DEBUG_NAME_A(m_Effect->GetFileName(), Quad);
#endif


}

void Quad::Render(bool depth_on, Effect* override_effect /*= nullptr*/)
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	ctx.SetRasterizerState(Engine::GetAPI()->GetRasterizerState(graphics::CULL_NONE));
	override_effect ? override_effect->Use() : m_Effect->Use();
	ctx.DrawIndexed(this, depth_on);
	override_effect ? override_effect->Use() : m_Effect->Clear();
}
