#include "stdafx.h"
#include "DeferredRenderer.h"
#include "PointLight.h"
namespace Snowblind
{
	CDeferredRenderer::CDeferredRenderer()
	{

		myDirectX = CEngine::GetDirectX();
		myContext = myDirectX->GetContext();
		myEngine = CEngine::GetInstance();
		SWindowSize windowSize = myEngine->GetWindowSize();
		myAlbedo = new CTexture(windowSize.myWidth, windowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		myAlbedo->SetDebugName("DeferredAlbedo");

		myNormal = new CTexture(windowSize.myWidth, windowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		myNormal->SetDebugName("DeferredNormal");

		myDepth = new CTexture(windowSize.myWidth, windowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R32G32B32A32_FLOAT);
		myDepth->SetDebugName("DeferredDepth");

		myFinishedTexture = new CTexture(windowSize.myWidth, windowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		myFinishedTexture->SetDebugName("DeferredFinishedTexture");


		myDepthStencil = new CTexture();
		myDepthStencil->InitAsDepthBuffer(windowSize.myWidth, windowSize.myHeight);
		myDepthStencil->SetDebugName("DeferredDepthStenci");

		myClearColor[0] = 0.f;
		myClearColor[1] = 0.f;
		myClearColor[2] = 0.f;
		myClearColor[3] = 0.f;

		myScreenData.myEffect = myEngine->GetAssetsContainer()->GetEffect("Data/Shaders/T_Render_To_Texture.json");
		myLightPass.myEffect = myEngine->GetAssetsContainer()->GetEffect("Data/Shaders/T_Deferred_LightMesh.json");
		myAmbientPass.myEffect = myEngine->GetAssetsContainer()->GetEffect("Data/Shaders/T_Deferred_Ambient.json");
		CreateLightConstantBuffers();
		CreateFullscreenQuad();
	}

	CDeferredRenderer::~CDeferredRenderer()
	{
		myDirectX = nullptr;
		SAFE_DELETE(myAlbedo);
		SAFE_DELETE(myNormal);
		SAFE_DELETE(myDepth);
		SAFE_DELETE(myFinishedTexture);

		SAFE_DELETE(myDepthStencil);
		SAFE_RELEASE(myInputLayout);
		SAFE_DELETE(myVertexBuffer);
		SAFE_DELETE(myIndexBuffer);
		SAFE_DELETE(myIndexData);
		SAFE_DELETE(myVertexData);

		SAFE_RELEASE(myLightPass.myVertexConstantBuffer);
		SAFE_RELEASE(myLightPass.myPixelConstantBuffer);
	}

	void CDeferredRenderer::SetTargets()
	{
		myContext->ClearRenderTargetView(myAlbedo->GetRenderTargetView(), myClearColor);
		myContext->ClearRenderTargetView(myNormal->GetRenderTargetView(), myClearColor);
		myContext->ClearRenderTargetView(myDepth->GetRenderTargetView(), myClearColor);
		myContext->ClearDepthStencilView(myDepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ID3D11RenderTargetView* target[3];
		target[0] = myAlbedo->GetRenderTargetView();
		target[1] = myNormal->GetRenderTargetView();
		target[2] = myDepth->GetRenderTargetView();

		myContext->OMSetRenderTargets(3, target, myDepthStencil->GetDepthView());
	}

	void CDeferredRenderer::SetBuffers()
	{
		SVertexBufferWrapper* buf = myVertexBuffer;
		myContext->IASetInputLayout(myInputLayout);
		myContext->IASetVertexBuffers(buf->myStartSlot
			, buf->myNrOfBuffers
			, &buf->myVertexBuffer
			, &buf->myStride
			, &buf->myByteOffset);
		SIndexBufferWrapper* inBuf = myIndexBuffer;
		myContext->IASetIndexBuffer(inBuf->myIndexBuffer, inBuf->myIndexBufferFormat, inBuf->myByteOffset);
		myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void CDeferredRenderer::CreateLightConstantBuffers()
	{
		//----------------------------------------
		// VertexShader Constant Buffer
		//----------------------------------------
		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SLightPass::SVertexConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&cbDesc, 0, &myLightPass.myVertexConstantBuffer);
		CEngine::GetDirectX()->SetDebugName(myLightPass.myVertexConstantBuffer, "LightPass Vertex Constant Buffer");
		CEngine::GetDirectX()->HandleErrors(hr, "[DeferredRenderer::LightPass] : Failed to Create Vertex Constant Buffer, ");


		//----------------------------------------
		// PixelShader Constant Buffer
		//----------------------------------------
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SLightPass::SPixelConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&cbDesc, 0, &myLightPass.myPixelConstantBuffer);
		CEngine::GetDirectX()->SetDebugName(myLightPass.myPixelConstantBuffer, "LightPass Pixel Constant Buffer");
		CEngine::GetDirectX()->HandleErrors(hr, "[DeferredRenderer::LightPass] : Failed to Create Pixel Constant Buffer, ");



	}

	void CDeferredRenderer::UpdateLightBuffers(CPointLight* pointlight, CCamera* aCamera, const CU::Matrix44f& previousOrientation)
	{
		//----------------------------------------
		// VertexShader Constant Buffer
		//----------------------------------------
		myLightPass.myVertexConstantStruct.myWorld = pointlight->GetOrientation();
		myLightPass.myVertexConstantStruct.myInvertedView = CU::Math::Inverse(previousOrientation);
		myLightPass.myVertexConstantStruct.myProjection = aCamera->GetProjection();
		myLightPass.myVertexConstantStruct.myScale.x = pointlight->GetRange();
		myLightPass.myVertexConstantStruct.myScale.y = pointlight->GetRange();
		myLightPass.myVertexConstantStruct.myScale.z = pointlight->GetRange();
		myLightPass.myVertexConstantStruct.myScale.w = pointlight->GetRange();

		D3D11_MAPPED_SUBRESOURCE msr;
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		CEngine::GetDirectX()->GetContext()->Map(myLightPass.myVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SLightPass::SVertexConstantBuffer* ptr = (SLightPass::SVertexConstantBuffer*)msr.pData;
			memcpy(ptr, &myLightPass.myVertexConstantStruct.myWorld.myMatrix[0], sizeof(SLightPass::SVertexConstantBuffer));
		}

		CEngine::GetDirectX()->GetContext()->Unmap(myLightPass.myVertexConstantBuffer, 0);

		//----------------------------------------
		// PixelShader Constant Buffer
		//----------------------------------------
		myLightPass.myPixelConstantStruct.myInvertedProjection = CU::Math::InverseReal(aCamera->GetProjection());
		myLightPass.myPixelConstantStruct.myView = previousOrientation;
		myLightPass.myPixelConstantStruct.myColor = pointlight->GetColor();
		myLightPass.myPixelConstantStruct.myPosition = pointlight->GetPosition();

		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		CEngine::GetDirectX()->GetContext()->Map(myLightPass.myPixelConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SLightPass::SPixelConstantBuffer* ptr = (SLightPass::SPixelConstantBuffer*)msr.pData;
			memcpy(ptr, &myLightPass.myPixelConstantStruct.myInvertedProjection.myMatrix[0], sizeof(SLightPass::SPixelConstantBuffer));
		}

		CEngine::GetDirectX()->GetContext()->Unmap(myLightPass.myPixelConstantBuffer, 0);

	}

	void CDeferredRenderer::RenderLight(CPointLight* pointlight, CCamera* aCamera, CU::Matrix44f& previousOrientation)
	{
		UpdateLightBuffers(pointlight, aCamera, previousOrientation);

		myContext->VSSetConstantBuffers(0, 1, &myLightPass.myVertexConstantBuffer);
		myContext->PSSetConstantBuffers(0, 1, &myLightPass.myPixelConstantBuffer);


		ID3D11ShaderResourceView* gbuffer[3];
		gbuffer[0] = myAlbedo->GetShaderView();
		gbuffer[1] = myNormal->GetShaderView();
		gbuffer[2] = myDepth->GetShaderView();
		myContext->PSSetShaderResources(0, 3, &gbuffer[0]);

		pointlight->Render(previousOrientation, aCamera);

		gbuffer[0] = nullptr;
		gbuffer[1] = nullptr;
		gbuffer[2] = nullptr;
		myContext->PSSetShaderResources(0, 3, &gbuffer[0]);
		
	}

	void CDeferredRenderer::SetNormalStates()
	{
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CDeferredRenderer::DeferredRender()
	{
		myDirectX->ResetViewport();

		ID3D11RenderTargetView* backbuffer = myFinishedTexture->GetRenderTargetView(); //myDirectX->GetBackbuffer();
		ID3D11DepthStencilView* depth = myDepthStencil->GetDepthView(); //myDirectX->GetDepthView();

		myContext->ClearRenderTargetView(backbuffer, myClearColor);
		myContext->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		myContext->OMSetRenderTargets(1, &backbuffer, depth);

		myDirectX->SetVertexShader(myAmbientPass.myEffect->GetVertexShader()->vertexShader);
		myDirectX->SetPixelShader(myAmbientPass.myEffect->GetPixelShader()->pixelShader);

		ID3D11ShaderResourceView* srv[3];
		srv[0] = myAlbedo->GetShaderView();
		srv[1] = myNormal->GetShaderView();
		srv[2] = myDepth->GetShaderView();
		myContext->PSSetShaderResources(0, 3, &srv[0]);
		myDirectX->SetSamplerState(eSamplerStates::POINT_CLAMP);

		myContext->DrawIndexed(6, 0, 0);

		srv[0] = nullptr;
		srv[1] = nullptr;
		srv[2] = nullptr;
		myContext->PSSetShaderResources(0, 3, &srv[0]);

	}

	void CDeferredRenderer::ResetBackbufferAndDepth()
	{
		ID3D11RenderTargetView* backbuffer = myDirectX->GetBackbuffer(); //myDirectX->GetBackbuffer();
		ID3D11DepthStencilView* depth = myDirectX->GetDepthView(); //myDirectX->GetDepthView();

		myContext->ClearRenderTargetView(backbuffer, myClearColor);
		myContext->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		myContext->OMSetRenderTargets(1, &backbuffer, depth);
	}

	void CDeferredRenderer::Finalize()
	{
		SetBuffers();
		myDirectX->SetVertexShader(myAmbientPass.myEffect->GetVertexShader()->vertexShader);
		myDirectX->SetPixelShader(myAmbientPass.myEffect->GetPixelShader()->pixelShader);

		ID3D11ShaderResourceView* srv[1];
		srv[0] = myFinishedTexture->GetShaderView();
		myContext->PSSetShaderResources(0, 1, &srv[0]);
		myDirectX->SetSamplerState(eSamplerStates::POINT_CLAMP);

		myContext->DrawIndexed(6, 0, 0);

		srv[0] = nullptr;
		myContext->PSSetShaderResources(0, 1, &srv[0]);
	}

	void CDeferredRenderer::SetLightStates()
	{
		ID3D11RenderTargetView* backbuffer = myFinishedTexture->GetRenderTargetView();
		myContext->OMSetRenderTargets(1, &backbuffer, myDepthStencil->GetDepthView());

		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		myDirectX->SetDepthBufferState(eDepthStencil::READ_NO_WRITE);

		myDirectX->SetVertexShader(myLightPass.myEffect->GetVertexShader()->vertexShader);
		myDirectX->SetPixelShader(myLightPass.myEffect->GetPixelShader()->pixelShader);
	}

	void CDeferredRenderer::CreateFullscreenQuad()
	{

		myVertexFormat.Init(2);
		myVertexFormat.Add(VertexLayoutPosUV[0]);
		myVertexFormat.Add(VertexLayoutPosUV[1]);

		CU::GrowingArray<SVertexTypePosUV> vertices;
		CU::GrowingArray<int> indices;
		SVertexTypePosUV v;
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

		myVertexBuffer = new SVertexBufferWrapper;
		myVertexData = new SVertexDataWrapper;
		myIndexBuffer = new SIndexBufferWrapper;
		myIndexData = new SVertexIndexWrapper;

		myVertexData->myNrOfVertexes = vertices.Size();
		myVertexData->myStride = sizeof(SVertexTypePosUV);
		myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		myVertexData->myVertexData = new char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);

		myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
		myIndexData->myIndexCount = 6;
		myIndexData->mySize = myIndexData->myIndexCount * 4;

		myIndexData->myIndexData = new char[myIndexData->mySize];
		memcpy(myIndexData->myIndexData, &indices[0], myIndexData->mySize);

		CreateVertexBuffer();
		CreateIndexBuffer();
	}

	void CDeferredRenderer::CreateVertexBuffer()
	{
		void* shader = myScreenData.myEffect->GetVertexShader()->compiledShader;
		int size = myScreenData.myEffect->GetVertexShader()->shaderSize;

		HRESULT hr = myDirectX->GetDevice()->CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size(), shader, size, &myInputLayout);
		myDirectX->SetDebugName(myInputLayout, "DeferredQuad Vertex Layout");
		myDirectX->HandleErrors(hr, "Failed to create VertexLayout");
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = myVertexData->mySize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = static_cast<void*>(myVertexData->myVertexData);

		hr = myDirectX->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer);
		myDirectX->HandleErrors(hr, "Failed to Create VertexBuffer!");

		myVertexBuffer->myStride = myVertexData->myStride;
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNrOfBuffers = 1;
	}

	void CDeferredRenderer::CreateIndexBuffer()
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
		HRESULT hr = myDirectX->GetDevice()->CreateBuffer(&indexDesc, &indexData, &myIndexBuffer->myIndexBuffer);
		myDirectX->HandleErrors(hr, "Failed to Create IndexBuffer");

		myIndexBuffer->myIndexBufferFormat = myIndexData->myFormat;
		myIndexBuffer->myByteOffset = 0;
	}

};