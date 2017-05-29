#include "stdafx.h"
#include "DeferredRenderer.h"
#include "PointLight.h"
#include <DL_Debug.h>
#include "GBuffer.h"
#define BLACK_CLEAR(v) v[0] = 0.f; v[1] = 0.f; v[2] = 0.f; v[3] = 1.f;

bool DeferredRenderer::Initiate(Texture* shadow_texture)
{
	m_API = Engine::GetAPI();
	myContext = m_API->GetContext();
	myEngine = Engine::GetInstance();
	BLACK_CLEAR(myClearColor);

	//WindowSize windowSize = Engine::GetInstance()->GetAPI()->GetInfo().
	WindowSize window_size;
	window_size.m_Height = m_API->GetInfo().m_WindowHeight;
	window_size.m_Width = m_API->GetInfo().m_WindowWidth;

	myFinishedSceneTexture = new Texture;
	myFinishedSceneTexture->Initiate(window_size.m_Width, window_size.m_Height
		, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R16G16B16A16_FLOAT
		, "Texture : FinishedScene");

	myDepthStencil = new Texture;
	myDepthStencil->Initiate(window_size.m_Width, window_size.m_Height
		, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_RENDER_TARGET
		, DXGI_FORMAT_R16G16B16A16_FLOAT
		, DXGI_FORMAT_R32_TYPELESS
		, DXGI_FORMAT_R32_FLOAT
		, DXGI_FORMAT_D32_FLOAT
		, "DeferredRenderer : ");

	myCubeMap = myEngine->GetTexture("Data/Textures/T_cubemap_level01.dds");

	myScreenPassShader = myEngine->GetEffect("Shaders/T_Render_To_Texture.json");

	m_GBuffer.Initiate();
	myAmbientPassShader = myEngine->GetEffect("Shaders/T_Deferred_Ambient.json");
	myAmbientPassShader->AddShaderResource(m_GBuffer.GetDiffuse(), Effect::DIFFUSE);
	myAmbientPassShader->AddShaderResource(m_GBuffer.GetNormal(), Effect::NORMAL);
	myAmbientPassShader->AddShaderResource(m_GBuffer.GetDepth(), Effect::DEPTH);
	myAmbientPassShader->AddShaderResource(m_GBuffer.GetEmissive(), Effect::EMISSIVE);
	myAmbientPassShader->AddShaderResource(myCubeMap, Effect::CUBEMAP);


	Engine::GetInstance()->GetEffect("Shaders/T_SSAO.json")->AddShaderResource(m_GBuffer.GetDepth(), Effect::DEPTH);
	Engine::GetInstance()->GetEffect("Shaders/T_SSAO.json")->AddShaderResource(m_GBuffer.GetNormal(), Effect::NORMAL);




	CreateFullscreenQuad();
	InitConstantBuffer();
	return true;
}

bool DeferredRenderer::CleanUp()
{
	m_GBuffer.CleanUp();
	myFinishedSceneTexture->CleanUp();
	SAFE_DELETE(myFinishedSceneTexture);
	myDepthStencil->CleanUp();
	SAFE_DELETE(myDepthStencil);
	//SAFE_DELETE(myGBuffer);

	//SAFE_DELETE(myConstantStruct);
	SAFE_RELEASE(myConstantBuffer);

	SAFE_DELETE(m_VertexBuffer);
	SAFE_DELETE(myVertexData);

	SAFE_DELETE(m_IndexBuffer);
	SAFE_DELETE(myIndexData);

	SAFE_RELEASE(myInputLayout);

	return true;
}

void DeferredRenderer::SetGBufferAsTarget(const RenderContext& render_context)
{
	m_GBuffer.Clear(myClearColor, render_context);
	render_context.m_Context->ClearDepthStencilView(myDepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_GBuffer.SetAsRenderTarget(myDepthStencil, render_context);
}

void DeferredRenderer::SetBuffers(const RenderContext& render_context)
{
	render_context.m_Context->IASetInputLayout(myInputLayout);

	render_context.m_Context->IASetVertexBuffers(m_VertexBuffer->myStartSlot
		, m_VertexBuffer->myNrOfBuffers
		, &m_VertexBuffer->myVertexBuffer
		, &m_VertexBuffer->myStride
		, &m_VertexBuffer->myByteOffset);

	render_context.m_Context->IASetIndexBuffer(m_IndexBuffer->myIndexBuffer
		, m_IndexBuffer->myIndexBufferFormat
		, m_IndexBuffer->myByteOffset);

	render_context.m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DeferredRenderer::DeferredRender(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir, const RenderContext& render_context)
{
	UpdateConstantBuffer(previousOrientation, aProjection, shadow_mvp, light_dir);
	SetBuffers(render_context);

	render_context.m_API->ResetViewport();

	ID3D11RenderTargetView* render_target = myFinishedSceneTexture->GetRenderTargetView();
	ID3D11DepthStencilView* depth = render_context.m_API->GetDepthView();

	render_context.m_Context->ClearRenderTargetView(render_target, myClearColor);
	render_context.m_Context->OMSetRenderTargets(1, &render_target, depth);

	myAmbientPassShader->Use();
	render_context.m_Context->PSSetConstantBuffers(0, 1, &myConstantBuffer);

	/*if ( m_Wireframe )
		m_WireframeState.Use();
	else
		m_SamplerState.Use();*/

	render_context.m_API->SetSamplerState(eSamplerStates::POINT_CLAMP);
	render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 1);
	render_context.m_API->SetRasterizer(m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_NONE);
	render_context.m_Context->DrawIndexed(6, 0, 0);
	render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);

	myAmbientPassShader->Clear();


	depth = myDepthStencil->GetDepthView();
	myContext->OMSetRenderTargets(1, &render_target, depth);
}

void DeferredRenderer::SetRenderTarget(const RenderContext& render_context)
{
	ID3D11DepthStencilView* depth = myDepthStencil->GetDepthView();
	myContext->OMSetRenderTargets(1, myFinishedSceneTexture->GetRenderTargetRef(), depth);
}

void DeferredRenderer::Finalize(const RenderContext& render_contexts)
{
	m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 0);
	m_API->SetBlendState(eBlendStates::NO_BLEND);
	m_API->SetRasterizer(m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_NONE);

	SetBuffers(render_contexts);

	m_API->SetVertexShader(myScreenPassShader->GetVertexShader()->m_Shader);
	m_API->SetPixelShader(myScreenPassShader->GetPixelShader()->m_Shader);

	ID3D11ShaderResourceView* srv[] =
	{
		myFinishedSceneTexture->GetShaderView(),
		myDepthStencil->GetShaderView(),
	};

	s32 num_srv = ARRAYSIZE(srv);

	myContext->PSSetShaderResources(0, num_srv, &srv[0]);
	m_API->SetSamplerState(eSamplerStates::POINT_CLAMP);
	myContext->DrawIndexed(6, 0, 0);

	for (s32 i = 0; i < num_srv; i++)
	{
		srv[i] = nullptr;
	}

	myContext->PSSetShaderResources(0, num_srv, &srv[0]);

	m_API->SetRasterizer(eRasterizer::CULL_BACK);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
}

Texture* DeferredRenderer::GetFinalTexture()
{
	return myFinishedSceneTexture;
}

void DeferredRenderer::InitConstantBuffer()
{

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(SConstantStruct);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = m_API->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
	m_API->SetDebugName(myConstantBuffer, "Deferred Ambient Constant Buffer");
	m_API->HandleErrors(hr, "[DeferredRenderer] : Failed to Create Constant Buffer, ");
}

void DeferredRenderer::UpdateConstantBuffer(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir)
{
	m_ConstantStruct.camPosition = previousOrientation.GetPosition();
	m_ConstantStruct.invertedProjection = CU::Math::InverseReal(aProjection);
	m_ConstantStruct.view = previousOrientation;
	m_ConstantStruct.m_ShadowMVP = shadow_mvp;
	m_ConstantStruct.m_Direction = light_dir;

	m_API->UpdateConstantBuffer(myConstantBuffer, &m_ConstantStruct);
}

GBuffer& DeferredRenderer::GetGBuffer()
{
	return m_GBuffer;
}

//This could be in the engine and return a quad object?
void DeferredRenderer::CreateFullscreenQuad()
{

	myVertexFormat.ReInit(2);
	myVertexFormat.Add(VertexLayoutPosUV[0]);
	myVertexFormat.Add(VertexLayoutPosUV[1]);

	CU::GrowingArray<VertexTypePosUV> vertices;
	CU::GrowingArray<int> indices;
	VertexTypePosUV v;
	v.myPosition = { -1, -1, 0 };
	v.myUV = { 0, 1 };
	vertices.Add(v);

	v.myPosition = { -1, 1, 0 };
	v.myUV = { 0, 0 };
	vertices.Add(v);

	v.myPosition = { 1, -1, 0 };
	v.myUV = { 1, 1 };
	vertices.Add(v);

	v.myPosition = { 1, 1, 0 };
	v.myUV = { 1, 0 };
	vertices.Add(v);


	indices.Add(0);
	indices.Add(1);
	indices.Add(2);

	indices.Add(3);
	indices.Add(2);
	indices.Add(1);

	m_VertexBuffer = new VertexBufferWrapper;
	myVertexData = new VertexDataWrapper;
	m_IndexBuffer = new IndexBufferWrapper;
	myIndexData = new IndexDataWrapper;

	myVertexData->myNrOfVertexes = vertices.Size();
	myVertexData->myStride = sizeof(VertexTypePosUV);
	myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
	myVertexData->myVertexData = new char[myVertexData->mySize]();
	memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);

	myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
	myIndexData->myIndexCount = 6;
	myIndexData->mySize = myIndexData->myIndexCount * 4;

	myIndexData->myIndexData = new char[myIndexData->mySize];
	memcpy(myIndexData->myIndexData, &indices[0], myIndexData->mySize);

	CreateBuffer();
	CreateIndexBuffer();
}

void DeferredRenderer::CreateBuffer()
{
	void* shader = myScreenPassShader->GetVertexShader()->compiledShader;
	int size = myScreenPassShader->GetVertexShader()->shaderSize;

	HRESULT hr = m_API->GetDevice()->CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size(), shader, size, &myInputLayout);
	m_API->SetDebugName(myInputLayout, "DeferredQuad Vertex Layout");
	m_API->HandleErrors(hr, "Failed to create VertexLayout");
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = myVertexData->mySize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = static_cast<void*>(myVertexData->myVertexData);

	hr = m_API->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer->myVertexBuffer);
	m_API->HandleErrors(hr, "Failed to Create VertexBuffer!");

	m_VertexBuffer->myStride = myVertexData->myStride;
	m_VertexBuffer->myByteOffset = 0;
	m_VertexBuffer->myStartSlot = 0;
	m_VertexBuffer->myNrOfBuffers = 1;
}

void DeferredRenderer::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = myIndexData->mySize;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData)), indexData.pSysMem = myIndexData->myIndexData;
	HRESULT hr = m_API->GetDevice()->CreateBuffer(&indexDesc, &indexData, &m_IndexBuffer->myIndexBuffer);
	m_API->HandleErrors(hr, "Failed to Create IndexBuffer");

	m_IndexBuffer->myIndexBufferFormat = myIndexData->myFormat;
	m_IndexBuffer->myByteOffset = 0;
}
