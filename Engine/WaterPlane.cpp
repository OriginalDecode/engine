#include "stdafx.h"
#include "WaterPlane.h"

void WaterPlane::Initiate(const CU::Vector3f& position)
{
	m_Orientation.SetPosition(position);

	myEffect = Engine::GetInstance()->GetEffect("Shaders/T_water.json");

	CreatePlane();

}

void WaterPlane::SetPosition(const CU::Vector3f& position)
{
	m_Orientation.SetPosition(position);
}

void WaterPlane::UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context)
{
	m_VertexMatrices.m_World = m_Orientation;
	m_VertexMatrices.m_InvertedView = CU::Math::Inverse(camera_orientation);
	m_VertexMatrices.m_Projection = camera_projection;
	m_VertexMatrices.m_Time = Engine::GetInstance()->GetTotalTime();
	render_context.m_API->UpdateConstantBuffer((myConstantBuffer), &m_VertexMatrices);
}

void WaterPlane::Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context)
{
	render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	render_context.m_API->SetBlendState(eBlendStates::BLEND_FALSE);
	render_context.m_API->SetRasterizer(eRasterizer::CULL_FRONT);
	SetupLayoutsAndBuffers();
	render_context.m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	//render_context.m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
	render_context.m_Context->VSSetConstantBuffers(0, 1, &myConstantBuffer);
	render_context.m_Context->DSSetConstantBuffers(0, 1, &myConstantBuffer);
	myEffect->Use();
	render_context.m_Context->DrawIndexed(m_Indices.Size(),0,0);
	myEffect->Clear();
}

void WaterPlane::ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context)
{
	DL_ASSERT("water shadow?");
}

void WaterPlane::CreatePlane()
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	myVertexFormat.ReInit(5);
	myVertexFormat.Add(vertexDesc[0]);
	myVertexFormat.Add(vertexDesc[1]);
	myVertexFormat.Add(vertexDesc[2]);
	myVertexFormat.Add(vertexDesc[3]);
	myVertexFormat.Add(vertexDesc[4]);
	
	const float half_width = 2048.f;

	SVertexPosNormUVBiTang vert;
	vert.position = { -half_width, 0, -half_width };
	vert.normal = { 0, 1, 0 };
	m_Vertices.Add(vert);

	vert.position = { -half_width, 0, half_width };
	vert.normal = { 0, 1, 0 };
	m_Vertices.Add(vert);

	vert.position = { half_width, 0, -half_width };
	vert.normal = { 0, 1, 0 };
	m_Vertices.Add(vert);

	vert.position = { half_width, 0, half_width };
	vert.normal = { 0, 1, 0 };
	m_Vertices.Add(vert);

	m_Indices.Add(0);
	m_Indices.Add(1);
	m_Indices.Add(2);

	m_Indices.Add(3);
	m_Indices.Add(2);
	m_Indices.Add(1);

	m_VertexData.myNrOfVertexes = m_Vertices.Size();
	m_VertexData.myStride = sizeof(SVertexPosNormUVBiTang);
	m_VertexData.mySize = m_VertexData.myNrOfVertexes * m_VertexData.myStride;
	m_VertexData.myVertexData = new char[m_VertexData.mySize]();
	memcpy(m_VertexData.myVertexData, &m_Vertices[0], m_VertexData.mySize);

	m_IndexData.myFormat = DXGI_FORMAT_R32_UINT;
	m_IndexData.myIndexCount = m_Indices.Size();
	m_IndexData.mySize = m_IndexData.myIndexCount * 4;

	m_IndexData.myIndexData = new char[m_IndexData.mySize];
	memcpy(m_IndexData.myIndexData, &m_Indices[0], m_IndexData.mySize);

	InitVertexBuffer();
	InitInputLayout();
	InitIndexBuffer();
	InitConstantBuffer();
}

void WaterPlane::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(VertexBaseStruct);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	myConstantBuffer = Engine::GetAPI()->CreateBuffer(cbDesc);
}
