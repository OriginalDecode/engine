#include "stdafx.h"
#include "DeferredRenderer.h"
#include "PointLight.h"
#include "LightPass.h"
#include <DL_Debug.h>
#include "GBuffer.h"
#define BLACK_CLEAR(v) v[0] = 0.f; v[1] = 0.f; v[2] = 0.f; v[3] = 0.f;
namespace Snowblind
{
	CDeferredRenderer::CDeferredRenderer()
	{
#ifdef SNOWBLIND_DX11
		myDirectX = CEngine::GetAPI();
		myContext = myDirectX->GetContext();
		myEngine = CEngine::GetInstance();
		BLACK_CLEAR(myClearColor);


		SWindowSize windowSize = myEngine->GetWindowSize();

		myFinishedSceneTexture = new CTexture(windowSize.myWidth, windowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		myFinishedSceneTexture->SetDebugName("DeferredFinishedTexture");

		myDepthStencil = new CTexture();
		myDepthStencil->InitStencil(windowSize.myWidth, windowSize.myHeight);
		myDepthStencil->SetDebugName("DeferredDepthStencil");

		myCubeMap = myEngine->GetTexture("Data/Textures/church_horizontal_cross_cube_specular_pow2.dds");

		myScreenPassShader = myEngine->GetEffect("Data/Shaders/T_Render_To_Texture.json");
		myAmbientPassShader = myEngine->GetEffect("Data/Shaders/T_Deferred_Ambient.json");

		myGBuffer = new CGBuffer();

		myAmbientPassShader->AddShaderResource(myGBuffer->myAlbedo->GetShaderView());
		myAmbientPassShader->AddShaderResource(myGBuffer->myNormal->GetShaderView());
		myAmbientPassShader->AddShaderResource(myGBuffer->myDepth->GetShaderView());
		myAmbientPassShader->AddShaderResource(myCubeMap->GetShaderView());

		CreateFullscreenQuad();
		InitConstantBuffer();
#endif
	}

	CDeferredRenderer::~CDeferredRenderer()
	{
#ifdef SNOWBLIND_DX11
		SAFE_DELETE(myFinishedSceneTexture);
		SAFE_DELETE(myDepthStencil);
		SAFE_DELETE(myConstantStruct);
		SAFE_DELETE(myGBuffer);
		SAFE_DELETE(myVertexBuffer);
		SAFE_DELETE(myIndexBuffer);
		SAFE_DELETE(myIndexData);
		SAFE_DELETE(myVertexData);

		SAFE_RELEASE(myConstantBuffer);
		SAFE_RELEASE(myInputLayout);
#endif
	}

	void CDeferredRenderer::SetTargets()
	{
#ifdef SNOWBLIND_DX11
		myGBuffer->Clear(myClearColor);
		myContext->ClearDepthStencilView(myDepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		myGBuffer->SetAsRenderTarget(myDepthStencil);
#endif
	}

	void CDeferredRenderer::SetBuffers()
	{
#ifdef SNOWBLIND_DX11
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
#endif
	}

	void CDeferredRenderer::DeferredRender(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection)
	{
#ifdef SNOWBLIND_DX11
		UpdateConstantBuffer(previousOrientation, aProjection);
		myDirectX->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		SetBuffers();

		myDirectX->ResetViewport();

		ID3D11RenderTargetView* backbuffer = myFinishedSceneTexture->GetRenderTargetView(); 
		ID3D11DepthStencilView* depth = myDirectX->GetDepthView(); 

		myContext->ClearRenderTargetView(backbuffer, myClearColor);
		myContext->OMSetRenderTargets(1, &backbuffer, depth);

		myAmbientPassShader->Activate();
		myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
		myDirectX->SetSamplerState(eSamplerStates::POINT_CLAMP); 

		myContext->DrawIndexed(6, 0, 0);

		myAmbientPassShader->Deactivate();
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);


		depth = myDepthStencil->GetDepthView();
		myContext->OMSetRenderTargets(1, &backbuffer, depth);
		myDirectX->SetSamplerState(eSamplerStates::POINT_CLAMP); 
#endif
	}

	void CDeferredRenderer::Finalize()
	{
#ifdef SNOWBLIND_DX11
		myDirectX->SetDepthBufferState(eDepthStencil::MASK_TEST);
		myDirectX->SetBlendState(eBlendStates::NO_BLEND);
		SetBuffers();

		myDirectX->SetVertexShader(myScreenPassShader->GetVertexShader()->vertexShader);
		myDirectX->SetPixelShader(myScreenPassShader->GetPixelShader()->pixelShader);

		ID3D11ShaderResourceView* srv[2];
		srv[0] = myFinishedSceneTexture->GetShaderView();
		srv[1] = myDepthStencil->GetShaderView();
		myContext->PSSetShaderResources(0, 2, &srv[0]);
		myDirectX->SetSamplerState(eSamplerStates::POINT_CLAMP);
		myContext->DrawIndexed(6, 0, 0);
		srv[0] = nullptr;
		srv[1] = nullptr;
		myContext->PSSetShaderResources(0, 2, &srv[0]);

		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
#endif
	}

	void CDeferredRenderer::InitConstantBuffer()
	{
#ifdef SNOWBLIND_DX11
		myConstantStruct = new SConstantStruct;

		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SConstantStruct);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = myDirectX->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
		myDirectX->SetDebugName(myConstantBuffer, "Deferred Ambient Constant Buffer");
		myDirectX->HandleErrors(hr, "[DeferredRenderer] : Failed to Create Constant Buffer, ");
#endif
	}

	void CDeferredRenderer::UpdateConstantBuffer(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection)
	{
#ifdef SNOWBLIND_DX11
		DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");
		myConstantStruct->camPosition = previousOrientation.GetPosition();
		myConstantStruct->invertedProjection = CU::Math::InverseReal(aProjection);
		myConstantStruct->view = previousOrientation;
		D3D11_MAPPED_SUBRESOURCE msr;
		myDirectX->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SConstantStruct* ptr = (SConstantStruct*)msr.pData;
			memcpy(ptr, &myConstantStruct->camPosition, sizeof(SConstantStruct));
		}

		myDirectX->GetContext()->Unmap(myConstantBuffer, 0);
#endif
	}

	CGBuffer* CDeferredRenderer::GetGBuffer()
	{
		return myGBuffer;
	}

	void CDeferredRenderer::CreateFullscreenQuad()
	{

#ifdef SNOWBLIND_DX11
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
#endif
	}

	void CDeferredRenderer::CreateVertexBuffer()
	{
#ifdef SNOWBLIND_DX11
		void* shader = myScreenPassShader->GetVertexShader()->compiledShader;
		int size = myScreenPassShader->GetVertexShader()->shaderSize;

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
#endif
	}

	void CDeferredRenderer::CreateIndexBuffer()
	{
#ifdef SNOWBLIND_DX11
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
#endif
	}
};