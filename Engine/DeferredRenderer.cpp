#include "stdafx.h"
#include "DeferredRenderer.h"
#include "AssetsContainer.h"
#include "VertexStructs.h"
#include "VertexTypes.h"

#include "IndexWrapper.h"
#include "VertexWrapper.h"


namespace Snowblind
{
	CDeferredRenderer::CDeferredRenderer()
	{
		myContext = CEngine::GetDirectX()->GetContext();
		myEngine = CEngine::GetInstance();
		myWindowSize = myEngine->GetWindowSize();
		myAlbedo = new CTexture(myWindowSize.myWidth, myWindowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		myNormal = new CTexture(myWindowSize.myWidth, myWindowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		myDepth = new CTexture(myWindowSize.myWidth, myWindowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R32G32B32A32_FLOAT);

		myDepthStencil = new CTexture();
		myDepthStencil->CreateDepthStencilView(myWindowSize.myWidth, myWindowSize.myHeight);

		myClearColor[0] = 0.f;
		myClearColor[1] = 0.f;
		myClearColor[2] = 0.f;
		myClearColor[3] = 0.f;

		myScreenData.myEffect = CAssetsContainer::GetInstance()->GetEffect("Data/Shaders/RenderToTexture.fx");
		myScreenData.mySource = myScreenData.myEffect->GetVariableByName("DiffuseTexture")->AsShaderResource();

		CreateAmbientData();
		//CreateLightData();

		CreateFullscreenQuad();

	}


	CDeferredRenderer::~CDeferredRenderer()
	{
		SAFE_DELETE(myAlbedo);
		SAFE_DELETE(myNormal);
		SAFE_DELETE(myDepth);
		SAFE_DELETE(myDepthStencil);
	}

	void CDeferredRenderer::Render()
	{
		

	}

	void CDeferredRenderer::Render(CEffect* anEffect)
	{
		D3DX11_TECHNIQUE_DESC techDesc;
		anEffect->GetTechnique()->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; p++)
		{
			anEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, myContext);
			myContext->DrawIndexed(6, 0, 0);
		}

	}

	ID3D11ShaderResourceView* CDeferredRenderer::GetDeferredTexture(const eDeferredType& aDeferredType)
	{
		DL_ASSERT("Not Implemented!");
		return 0;
	}

	void CDeferredRenderer::DeferredRender()
	{

	}

	void CDeferredRenderer::CreateLightData()
	{
		myLightPass.myEffect = CAssetsContainer::GetInstance()->GetEffect("Data/Shaders/DeferredLightMesh.fx");
		myLightPass.myAlbedo = myLightPass.myEffect->GetVariableByName("AlbedoTexture")->AsShaderResource();
		myLightPass.myNormal = myLightPass.myEffect->GetVariableByName("NormalTexture")->AsShaderResource();
		myLightPass.myDepth = myLightPass.myEffect->GetVariableByName("DepthTexture;")->AsShaderResource();
		myLightPass.myPointLightVariable = myLightPass.myEffect->GetVariableByName("PointLights");
	}

	void CDeferredRenderer::CreateAmbientData()
	{
		myAmbientPass.myEffect = CAssetsContainer::GetInstance()->GetEffect("Data/Shaders/DeferredAmbient.fx");
		myAmbientPass.myAlbedo = myAmbientPass.myEffect->GetVariableByName("AlbedoTexture")->AsShaderResource();
		myAmbientPass.myNormal = myAmbientPass.myEffect->GetVariableByName("NormalTexture")->AsShaderResource();
		myAmbientPass.myDepth = myAmbientPass.myEffect->GetVariableByName("DepthTexture;")->AsShaderResource();
	}

	void CDeferredRenderer::CreateFullscreenQuad()
	{
		myVertexFormat.Init(2);
		myVertexFormat.Add(VertexLayoutPosUV[0]);
		myVertexFormat.Add(VertexLayoutPosUV[1]);

		CU::GrowingArray<SVertexTypePosUV> vertices;
		CU::GrowingArray<int> indices;
		SVertexTypePosUV v;
		v.myPosition = { -1, -1, 1 };
		v.myUV = { 0, 1 };
		vertices.Add(v);

		v.myPosition = { -1, 1, 1 };
		v.myUV = { 0, 0 };
		vertices.Add(v);

		v.myPosition = { 1, -1, 1 };
		v.myUV = { 1, 1 };
		vertices.Add(v);

		v.myPosition = { 1, 1, 1 };
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
		HRESULT hr;

		D3DX11_PASS_DESC passDesc;
		hr = myScreenData.myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to get description from EffectPass!");

		hr = CEngine::GetDirectX()->GetDevice()->
			CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size(), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myInputLayout);
		CEngine::GetDirectX()->SetDebugName(myInputLayout, "DeferredQuad Vertex Layout");
		BAD_VALUE(hr != S_OK, hr);
		//CEngine::GetDirectX()->HandleErrors(hr, "Failed to create VertexLayout");

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = myVertexData->mySize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = static_cast<void*>(myVertexData->myVertexData);

		hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer);
		BAD_VALUE(hr != S_OK, hr);
		//CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create VertexBuffer!");

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
		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&indexDesc, &indexData, &myIndexBuffer->myIndexBuffer);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create IndexBuffer");

		myIndexBuffer->myIndexBufferFormat = myIndexData->myFormat;
		myIndexBuffer->myByteOffset = 0;
	}
};