#include "stdafx.h"
#include "Quad.h"


void Quad::Initiate()
{
	m_VertexData.myNrOfVertexes = 4;
	m_VertexData.myStride = sizeof(VertexTypePosUV);
	m_VertexData.mySize = m_VertexData.myNrOfVertexes * m_VertexData.myStride;
	m_VertexData.myVertexData = new s8[m_VertexData.mySize];

	CU::GrowingArray<VertexTypePosUV> vertices;
	CU::GrowingArray<s32> indices;

	VertexTypePosUV vertex;
	vertex.myPosition = { -1, -1, 0 };
	vertex.myUV = { 0, 1 };
	vertices.Add(vertex);

	vertex.myPosition = { -1, 1, 0 };
	vertex.myUV = { 0, 0 };
	vertices.Add(vertex);


	vertex.myPosition = { 1, -1, 0 };
	vertex.myUV = { 1, 1 };
	vertices.Add(vertex);


	vertex.myPosition = { 1, 1, 0 };
	vertex.myUV = { 1, 0 };
	vertices.Add(vertex);

	memcpy(m_VertexData.myVertexData, &vertices[0], m_VertexData.mySize);

	indices.Add(0);
	indices.Add(1);
	indices.Add(2);

	indices.Add(3);
	indices.Add(2);
	indices.Add(1);

	m_IndexData.myFormat = DXGI_FORMAT_R32_UINT;
	m_IndexData.myIndexCount = 6;
	m_IndexData.mySize = m_IndexData.myIndexCount * 4;
	m_IndexData.myIndexData = new s8[m_IndexData.mySize];
	memcpy(m_IndexData.myIndexData, &indices[0], m_IndexData.mySize);


	D3D11_INPUT_ELEMENT_DESC desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_ScreenpassShader = Engine::GetInstance()->GetEffect("Shaders/render_to_texture.json");


	void* shader = m_ScreenpassShader->GetVertexShader()->compiledShader;
	int size = m_ScreenpassShader->GetVertexShader()->shaderSize;

	DirectX11* dx = Engine::GetInstance()->GetAPI();

	HRESULT hr = dx->GetDevice()->CreateInputLayout(&desc[0], ARRAYSIZE(desc), shader, size, &m_InputLayout);

	dx->SetDebugName(m_InputLayout, "Quad Vertex Layout");
	dx->HandleErrors(hr, "Failed to create VertexLayout");


	//_____________________________

	m_VertexBuffer.myVertexBuffer = dx->CreateVertexBuffer(m_VertexData.mySize, m_VertexData.myVertexData);
	dx->SetDebugName(m_VertexBuffer.myVertexBuffer, "Quad VertexBuffer");

	m_VertexBuffer.myStride = m_VertexData.myStride;
	m_VertexBuffer.myByteOffset = 0;
	m_VertexBuffer.myStartSlot = 0;
	m_VertexBuffer.myNrOfBuffers = 1;


	//______________________________

	m_IndexBuffer.myIndexBuffer = dx->CreateIndexBuffer(m_IndexData.mySize, m_IndexData.myIndexData);
	dx->SetDebugName(m_VertexBuffer.myVertexBuffer, "Quad IndexBuffer");
	m_IndexBuffer.myIndexBufferFormat = m_IndexData.myFormat;
	m_IndexBuffer.myByteOffset = 0;

}

void Quad::CleanUp()
{
	SAFE_RELEASE(m_InputLayout);
	m_ScreenpassShader = nullptr;
}

void Quad::SetBuffers()
{
	IDevContext* ctx = Engine::GetInstance()->GetAPI()->GetContext();
	ctx->IASetInputLayout(m_InputLayout);

	ctx->IASetVertexBuffers(m_VertexBuffer.myStartSlot
		, m_VertexBuffer.myNrOfBuffers
		, &m_VertexBuffer.myVertexBuffer
		, &m_VertexBuffer.myStride
		, &m_VertexBuffer.myByteOffset);

	ctx->IASetIndexBuffer(m_IndexBuffer.myIndexBuffer
		, m_IndexBuffer.myIndexBufferFormat
		, m_IndexBuffer.myByteOffset);

	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Quad::Render()
{
	IDevContext* ctx = Engine::GetInstance()->GetAPI()->GetContext();
	ctx->DrawIndexed(6, 0, 0);
}
