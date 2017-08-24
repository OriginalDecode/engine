#include "stdafx.h"
#include "WaterPlane.h"
#include <Engine/Texture.h>

void WaterPlane::Initiate(const CU::Vector3f& position)
{
	m_Orientation.SetPosition(position);

	myEffect = Engine::GetInstance()->GetEffect("Shaders/water.json");

	const WindowSize& window_size = Engine::GetInstance()->GetInnerSize();

	m_Refraction = new Texture;
	m_Refraction->Initiate(window_size.m_Width, window_size.m_Height
		, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_RENDER_TARGET
		, DXGI_FORMAT_R16G16B16A16_FLOAT
		, DXGI_FORMAT_R32_TYPELESS
		, DXGI_FORMAT_R32_FLOAT
		, DXGI_FORMAT_D32_FLOAT
		, "Water : RefractionDepth");

	m_Reflection = new Texture;
	m_Reflection-> Initiate(window_size.m_Width, window_size.m_Height
		, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_RENDER_TARGET
		, DXGI_FORMAT_R16G16B16A16_FLOAT
		, DXGI_FORMAT_R32_TYPELESS
		, DXGI_FORMAT_R32_FLOAT
		, DXGI_FORMAT_D32_FLOAT
		, "Water : ReflectionDepth");

	m_RefractionG.Initiate();
	m_ReflectionG.Initiate();

	myEffect->AddShaderResource(m_RefractionG.GetDiffuse(), Effect::REFRACTION);
	myEffect->AddShaderResource(m_ReflectionG.GetDiffuse(), Effect::REFLECTION);

	Engine* engine = Engine::GetInstance();

	myEffect->AddShaderResource(engine->GetTexture("Data/Textures/T_cubemap_level01.dds"), Effect::CUBEMAP);
	myEffect->AddShaderResource(engine->GetTexture("Data/Textures/water_normal.dds"), Effect::NORMAL);
	myEffect->AddShaderResource(engine->GetTexture("Data/Textures/water_dudv.dds"), Effect::DUDV);


//	engine->AddTexture(m_RefractionG.GetDiffuse(), "Refraction");
//	engine->AddTexture(m_ReflectionG.GetDiffuse(), "Reflection");
	CreatePlane();
	m_cbPixel = Engine::GetAPI()->CreateConstantBuffer(sizeof(cbPixel));
	
	//engine->RegisterCheckBox(&m_RenderWireframe, "Waterplane : Wireframe");

}

void WaterPlane::CleanUp()
{
	SAFE_DELETE(m_Reflection);
	SAFE_DELETE(m_Refraction);
	SAFE_RELEASE(m_cbPixel);
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
	m_VertexMatrices.m_CameraPos = camera_orientation.GetPosition();
	m_VertexMatrices.m_Time = Engine::GetInstance()->GetTotalTime();
	render_context.m_API->UpdateConstantBuffer((myConstantBuffer), &m_VertexMatrices);
}

void WaterPlane::Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context)
{
	render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	render_context.m_API->SetBlendState(eBlendStates::BLEND_FALSE);

	render_context.m_API->SetRasterizer(m_RenderWireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_NONE);
	ID3D11SamplerState* sampler = render_context.m_API->GetSampler((s32)eSamplerStates::LINEAR_WRAP);
	render_context.m_Context->PSSetSamplers(1, 1, &sampler);

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

void WaterPlane::SetupRefractionRender(const RenderContext& render_context)
{
	m_RefractionG.Clear(m_ClearColor, render_context);
	render_context.m_Context->ClearDepthStencilView(m_Refraction->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_RefractionG.SetAsRenderTarget(m_Refraction, render_context);
}

void WaterPlane::SetupReflectionRender(const RenderContext& render_context)
{
	m_ReflectionG.Clear(m_ClearColor, render_context);
	render_context.m_Context->ClearDepthStencilView(m_Reflection->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_ReflectionG.SetAsRenderTarget(m_Reflection, render_context);
}

void WaterPlane::SetClipPlane(const CU::Vector4f& plane, const RenderContext& render_context)
{
	m_PixelStruct.m_CompareValue = plane;
	render_context.m_API->UpdateConstantBuffer(m_cbPixel, &m_PixelStruct);
	render_context.m_Context->PSSetConstantBuffers(0, 1, &m_cbPixel);
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
	vert.uv = { 0, 0 };
	m_Vertices.Add(vert);

	vert.position = { -half_width, 0, half_width };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 0, 1 };

	m_Vertices.Add(vert);

	vert.position = { half_width, 0, -half_width };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 1, 0 };
	m_Vertices.Add(vert);

	vert.position = { half_width, 0, half_width };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 1, 1 };
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
