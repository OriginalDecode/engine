#include "stdafx.h"
#include "DeferredRenderer.h"
#include "PointLight.h"
#include <DL_Debug.h>
#include "GBuffer.h"

#include <Engine/Quad.h>
#include <Engine/IGraphicsContext.h>


DeferredRenderer::~DeferredRenderer()
{
	SAFE_DELETE(m_RenderQuad);
// 	SAFE_DELETE(myFinishedSceneTexture);
// 	SAFE_DELETE(myDepthStencil);
// 	SAFE_DELETE(m_VertexBuffer);
// 	SAFE_DELETE(myVertexData);
// 	SAFE_DELETE(m_IndexBuffer);
// 	SAFE_DELETE(myIndexData);
// 	SAFE_DELETE(m_SampleTexture);
// 
// 	SAFE_RELEASE(myInputLayout);
// 	SAFE_RELEASE(myConstantBuffer);
}

bool DeferredRenderer::Initiate(Texture* shadow_texture)
{

	const graphics::IGraphicsAPI* api = Engine::GetAPI();
	WindowSize window_size;
	window_size.m_Height = api->GetInfo().m_WindowHeight;
	window_size.m_Width = api->GetInfo().m_WindowWidth;

	m_FinishedSceneTexture = new Texture;
// 	m_FinishedSceneTexture->Initiate(window_size.m_Width, window_size.m_Height
// 		, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
// 		, DXGI_FORMAT_R16G16B16A16_FLOAT
// 		, "Texture : FinishedScene");

	m_SampleTexture = new Texture;
// 	m_SampleTexture->Initiate(window_size.m_Width, window_size.m_Height
// 		, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
// 		, DXGI_FORMAT_R16G16B16A16_FLOAT
// 		, "DeferredRenderer : SampleTexture");

	TextureDesc depth_desc;
	depth_desc.m_Usage = graphics::DEFAULT_USAGE;
	depth_desc.m_ResourceTypeBinding = graphics::SHADERRESOURCE | graphics::DEPTHSTENCIL | graphics::RENDERTARGET;
	depth_desc.m_TextureFormat = graphics::RGBA16_FLOAT;
	depth_desc.m_DepthTextureFormat = graphics::DEPTH_32_FLOAT;
	depth_desc.m_RenderTargetFormat = graphics::R32_FLOAT;
	depth_desc.m_Width = window_size.m_Width;
	depth_desc.m_Height = window_size.m_Height;

	m_DepthStencilTexture = new Texture;
	m_DepthStencilTexture->Initiate(depth_desc, "DeferredRenderer - DSV");


	m_CubeMap = Engine::GetInstance()->GetTexture("Data/Textures/church_horizontal_cross_cube_specular_pow2.dds");

	m_ScreenPassShader = Engine::GetInstance()->GetEffect("Shaders/render_to_texture.json");

	m_AmbientPassShader = Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json");
	m_AmbientPassShader->AddShaderResource(m_CubeMap, Effect::CUBEMAP);


	m_RenderQuad = new Quad(m_AmbientPassShader);

	InitConstantBuffer();
	return true;
}

// void DeferredRenderer::SetGBufferAsTarget()
// {
//  	m_GBuffer.Clear(clearcolor::black, render_context);
//  	render_context.m_Context->ClearDepthStencilView(myDepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f 0);
//  	m_GBuffer.SetAsRenderTarget(myDepthStencil, render_context);
// }

void DeferredRenderer::DeferredRender(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir, const graphics::RenderContext& render_context)
{
	//UpdateConstantBuffer(previousOrientation, aProjection, shadow_mvp, light_dir);
 
	//render_context.m_API->ResetViewport();
 
	IRenderTargetView* rtv[] = {
		m_FinishedSceneTexture->GetRenderTargetView()
	};
	//ID3D11DepthStencilView* depth = render_context.m_API->GetDepthView(); 
	//Replace above depth thing
 
	auto& ctx = render_context.GetContext();

	ctx.ClearRenderTarget(rtv, clearcolor::black);
	ctx.OMSetRenderTargets(ARRSIZE(rtv), rtv, nullptr);

	ctx.PSSetConstantBuffer(0, 1, m_ConstantBuffer);
	ctx.PSSetSamplerState(0, 1, render_context.GetAPI().GetSamplerState(graphics::LINEAR));

	render_context.m_API->SetRasterizer(eRasterizer::CULL_NONE);

	//render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 1);
	m_AmbientPassShader->Use();
	render_context.GetContext().DrawIndexed(m_RenderQuad);
	m_AmbientPassShader->Clear();
	//render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
 


	/*depth = myDepthStencil->GetDepthView();
 
	myContext->OMSetRenderTargets(1, &rtv[0], depth);*/
}

void DeferredRenderer::SetRenderTarget()
{
// 	ID3D11DepthStencilView* depth = myDepthStencil->GetDepthView();
// 	myContext->OMSetRenderTargets(1, myFinishedSceneTexture->GetRenderTargetRef(), depth);
}

void DeferredRenderer::Finalize()
{
	m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 0);
	m_API->SetBlendState(eBlendStates::NO_BLEND);
	m_API->SetRasterizer(m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_NONE);

	SetBuffers(render_contexts);

	ID3D11ShaderResourceView* srv[] =
	{
		myFinishedSceneTexture->GetShaderView(),
		myDepthStencil->GetShaderView(),
	};

	s32 num_srv = ARRAYSIZE(srv);

	myContext->PSSetShaderResources(0, num_srv, &srv[0]);
	m_API->SetSamplerState(eSamplerStates::POINT_CLAMP);

	myScreenPassShader->Use();
	myContext->DrawIndexed(6, 0, 0);
	myScreenPassShader->Clear();

	m_API->SetRasterizer(eRasterizer::CULL_BACK);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
}

void DeferredRenderer::InitConstantBuffer()
{

// 	D3D11_BUFFER_DESC cbDesc;
// 	ZeroMemory(&cbDesc, sizeof(cbDesc));
// 	cbDesc.ByteWidth = sizeof(SConstantStruct);
// 	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
// 	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
// 	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
// 	cbDesc.MiscFlags = 0;
// 	cbDesc.StructureByteStride = 0;
// 
// 	HRESULT hr = m_API->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
// 	m_API->SetDebugName(myConstantBuffer, "Deferred Ambient Constant Buffer");
// 	m_API->HandleErrors(hr, "[DeferredRenderer] : Failed to Create Constant Buffer, ");
}

void DeferredRenderer::UpdateConstantBuffer(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir)
{
// 	m_ConstantStruct.camPosition = previousOrientation.GetPosition();
// 	m_ConstantStruct.invertedProjection = CU::Math::InverseReal(aProjection);
// 	m_ConstantStruct.view = previousOrientation;
// 	m_ConstantStruct.m_ShadowMVP = shadow_mvp;
// 	m_ConstantStruct.m_Direction = light_dir;
// 
// 	m_API->UpdateConstantBuffer(myConstantBuffer, &m_ConstantStruct);
}

void DeferredRenderer::OnResize()
{
	//Recreate the quad
	SAFE_DELETE(m_RenderQuad);
	m_RenderQuad = new Quad(Engine::GetInstance()->GetEffect(""));
}

// //This could be in the engine and return a quad object?
// void DeferredRenderer::CreateFullscreenQuad()
// {
// 	myVertexFormat.ReInit(2);
// 	myVertexFormat.Add(VertexLayoutPosUV[0]);
// 	myVertexFormat.Add(VertexLayoutPosUV[1]);
// 
// 	CU::GrowingArray<VertexTypePosUV> vertices;
// 	CU::GrowingArray<int> indices;
// 	VertexTypePosUV v;
// 	v.myPosition = { -1, -1, 0, 1 };
// 	v.myUV = { 0, 1 };
// 	vertices.Add(v);
// 
// 	v.myPosition = { -1, 1, 0, 1 };
// 	v.myUV = { 0, 0 };
// 	vertices.Add(v);
// 
// 	v.myPosition = { 1, -1, 0, 1 };
// 	v.myUV = { 1, 1 };
// 	vertices.Add(v);
// 
// 	v.myPosition = { 1, 1, 0, 1 };
// 	v.myUV = { 1, 0 };
// 	vertices.Add(v);
// 
// 
// 	indices.Add(0);
// 	indices.Add(1);
// 	indices.Add(2);
// 
// 	indices.Add(3);
// 	indices.Add(2);
// 	indices.Add(1);
// 
// 	m_VertexBuffer = new VertexBufferWrapper;
// 	myVertexData = new VertexDataWrapper;
// 	m_IndexBuffer = new IndexBufferWrapper;
// 	myIndexData = new IndexDataWrapper;
// 
// 	myVertexData->myNrOfVertexes = vertices.Size();
// 	myVertexData->myStride = sizeof(VertexTypePosUV);
// 	myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
// 	myVertexData->myVertexData = new char[myVertexData->mySize]();
// 	memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);
// 
// 	myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
// 	myIndexData->myIndexCount = 6;
// 	myIndexData->mySize = myIndexData->myIndexCount * 4;
// 
// 	myIndexData->myIndexData = new char[myIndexData->mySize];
// 	memcpy(myIndexData->myIndexData, &indices[0], myIndexData->mySize);
// 
// 	CreateBuffer();
// 	CreateIndexBuffer();
// }
// 
// void DeferredRenderer::CreateBuffer()
// {
// 	void* shader = myScreenPassShader->GetVertexShader()->compiledShader;
// 	int size = myScreenPassShader->GetVertexShader()->shaderSize;
// 
// 	HRESULT hr = m_API->GetDevice()->CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size(), shader, size, &myInputLayout);
// 	m_API->SetDebugName(myInputLayout, "DeferredQuad Vertex Layout");
// 	m_API->HandleErrors(hr, "Failed to create VertexLayout");
// 	D3D11_BUFFER_DESC vertexBufferDesc;
// 	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
// 	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
// 	vertexBufferDesc.ByteWidth = myVertexData->mySize;
// 	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
// 	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
// 	vertexBufferDesc.MiscFlags = 0;
// 
// 	D3D11_SUBRESOURCE_DATA vertexData;
// 	vertexData.pSysMem = static_cast<void*>(myVertexData->myVertexData);
// 
// 	hr = m_API->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer->myVertexBuffer);
// 	m_API->HandleErrors(hr, "Failed to Create VertexBuffer!");
// 
// 	m_VertexBuffer->myStride = myVertexData->myStride;
// 	m_VertexBuffer->myByteOffset = 0;
// 	m_VertexBuffer->myStartSlot = 0;
// 	m_VertexBuffer->myNrOfBuffers = 1;
// }
// 
// void DeferredRenderer::CreateIndexBuffer()
// {
// 	D3D11_BUFFER_DESC indexDesc;
// 	ZeroMemory(&indexDesc, sizeof(indexDesc));
// 	indexDesc.Usage = D3D11_USAGE_DEFAULT;
// 	indexDesc.ByteWidth = myIndexData->mySize;
// 	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
// 	indexDesc.CPUAccessFlags = 0;
// 	indexDesc.MiscFlags = 0;
// 	indexDesc.StructureByteStride = 0;
// 
// 	D3D11_SUBRESOURCE_DATA indexData;
// 	ZeroMemory(&indexData, sizeof(indexData)), indexData.pSysMem = myIndexData->myIndexData;
// 	HRESULT hr = m_API->GetDevice()->CreateBuffer(&indexDesc, &indexData, &m_IndexBuffer->myIndexBuffer);
// 	m_API->HandleErrors(hr, "Failed to Create IndexBuffer");
// 
// 	m_IndexBuffer->myIndexBufferFormat = myIndexData->myFormat;
// 	m_IndexBuffer->myByteOffset = 0;
// }
