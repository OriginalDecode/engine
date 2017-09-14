#include "stdafx.h"
#include "Quad.h"
#include <Engine/Engine.h>
#include <Engine/IGraphicsAPI.h>
#include <Engine/IGraphicsDevice.h>
#include <Engine/IGraphicsContext.h>

Quad::Quad(Effect* effect) : m_Effect(effect)
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
	vertices[0].myPosition = { -1, -1, 0, 1 };
	vertices[0].myUV = { 0, 1 };

	vertices[1].myPosition = { -1, 1, 0 , 1 };
	vertices[1].myUV = { 0, 0 };

	vertices[2].myPosition = { 1, -1, 0 , 1 };
	vertices[2].myUV = { 1, 1 };

	vertices[3].myPosition = { 1, 1, 0 , 1 };
	vertices[3].myUV = { 1, 0 };

	memcpy(data, &vertices[0], size);


	graphics::BufferDesc buf_desc;
	buf_desc.m_Size = size;
	buf_desc.m_Data = data;
	buf_desc.m_CPUAccessFlag = graphics::WRITE;
	buf_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	buf_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;

	IBuffer* vertex_buffer = device.CreateBuffer(buf_desc);

	graphics::InputElementDesc desc[] = {
		{ "POSITION", 0, graphics::_16BYTE_RGBA, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, graphics::_8BYTE_RG, 0, 16, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};
	IInputLayout* layout = device.CreateInputLayout(m_Effect->GetVertexShader(), desc, ARRSIZE(desc));

	m_VertexWrapper = VertexWrapper(data, 
									vtx_start, 
									vtx_buffer_count,
									stride, vtx_byte_offset,
									vtx_count, 
									size, 
									vertex_buffer, 
									layout, 
									graphics::TRIANGLE_LIST);

	// Index
	const graphics::eVertexFormat format = graphics::_4BYTE_R;
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
	idx_desc.m_CPUAccessFlag = graphics::READ;
	idx_desc.m_BindFlag = graphics::BIND_INDEX_BUFFER;
	idx_desc.m_UsageFlag = graphics::DEFAULT_USAGE;

	IBuffer* index_buffer = device.CreateBuffer(idx_desc);

	m_IndexWrapper = IndexWrapper(index_data, 
								  index_count, 
								  index_start, 
								  index_size, 
								  format, 
								  index_byte_offset, 
								  index_buffer);
}

void Quad::Render(bool depth_on, Effect* override_effect /*= nullptr*/)
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	override_effect ? override_effect->Use() : m_Effect->Use();
	ctx.DrawIndexed(this, depth_on);
	override_effect ? override_effect->Use() : m_Effect->Clear();
}
