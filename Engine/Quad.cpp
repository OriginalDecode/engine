#include "stdafx.h"
#include "Quad.h"
#include <Engine/Engine.h>
#include <Engine/IGraphicsAPI.h>
#include <Engine/IGraphicsContext.h>

void Quad::Initiate()
{
	m_ScreenpassShader = Engine::GetInstance()->GetEffect("Shaders/render_to_texture.json");

	const s32 vtx_count = 4;
	const s32 stride = sizeof(VertexTypePosUV);
	const s32 size = vtx_count * stride;
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

// 	D3D11_INPUT_ELEMENT_DESC desc[] = {
// 		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
// 		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 16,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
// 	};
// 
// 	void* shader = m_ScreenpassShader->GetVertexShader()->compiledShader;
// 	int size = m_ScreenpassShader->GetVertexShader()->shaderSize;
// 
// 	DirectX11* dx = Engine::GetInstance()->GetAPI();
// 
// 	HRESULT hr = dx->GetDevice()->CreateInputLayout(&desc[0], ARRAYSIZE(desc), shader, size, &m_InputLayout);
// 
// 	dx->SetDebugName(m_InputLayout, "Quad Vertex Layout");
// 	dx->HandleErrors(hr, "Failed to create VertexLayout");

	m_VertexWrapper = VertexWrapper(data, 0, 1, stride, 0, vtx_count, size, /*vertex buffer*/nullptr, /*input layout*/nullptr, graphics::TRIANGLE_LIST);

	
	const graphics::eVertexFormat format = graphics::_4BYTE_R;
	const s32 index_count = 6;
	const s32 index_size = index_count * 4; //why *4?
	s8* index_data = new s8[index_size];

	s32 indices[index_count];
	indices[0] = 1;
	indices[1] = 0;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 1;


	memcpy(index_data, &indices[0], index_size);

	m_IndexWrapper = IndexWrapper(index_data, index_count, 0, index_size, format, 0, nullptr /*index buffer*/);



	


	//_____________________________

// 	m_VertexBuffer.myVertexBuffer = dx->CreateVertexBuffer(m_VertexData.mySize, m_VertexData.myVertexData);
// 	dx->SetDebugName(m_VertexBuffer.myVertexBuffer, "Quad VertexBuffer");
// 
// 	m_VertexBuffer.myStride = m_VertexData.myStride;
// 	m_VertexBuffer.myByteOffset = 0;
// 	m_VertexBuffer.myStartSlot = 0;
// 	m_VertexBuffer.myNrOfBuffers = 1;
// 
// 
// 	//______________________________
// 
// 	m_IndexBuffer.myIndexBuffer = dx->CreateIndexBuffer(m_IndexData.mySize, m_IndexData.myIndexData);
// 	dx->SetDebugName(m_VertexBuffer.myVertexBuffer, "Quad IndexBuffer");
// 	m_IndexBuffer.myIndexBufferFormat = m_IndexData.myFormat;
// 	m_IndexBuffer.myByteOffset = 0;

}

void Quad::Render()
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	//Engine::GetAPI()->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 1);
	ctx.DrawIndexed(this);
	//Engine::GetAPI()->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
}
