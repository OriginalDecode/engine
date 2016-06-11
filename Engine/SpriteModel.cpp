#include "stdafx.h"
#include "SpriteModel.h"
#include "DirectX11.h"
#include "Texture.h"
#include "Effect.h"
#include "Engine.h"
#include "VertexTypes.h"
#include "VertexWrapper.h"
#include "IndexWrapper.h"
#include "EngineDefines.h"
#include "AssetsContainer.h"

namespace Snowblind
{
	CSpriteModel::CSpriteModel()
	{
	}


	CSpriteModel::~CSpriteModel()
	{
		SAFE_DELETE(myIndexBuffer);
		SAFE_DELETE(myVertexBuffer);
		SAFE_DELETE(myIndexData);
		SAFE_DELETE(myVertexData);

		SAFE_RELEASE(myTexture);
		SAFE_RELEASE(myVertexLayout);
		SAFE_RELEASE(myBlendState);

	}

	void CSpriteModel::Initiate(const std::wstring& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
	{
		//myWindowSize = CEngine::GetInstance()->GetWindowSize();
		//
		//myTexturePath = aTexturePath;
		//mySize = aSize;
		//myPosition = aPosition;
		//myTexture = CAssetsContainer::GetInstance()->GetTexture(myTexturePath)->GetShaderView();
		//myEffect = CAssetsContainer::GetInstance()->GetEffect("Data/Shaders/Sprite.fx");
		//myEffect->SetAlbedo(myTexture);
		//
		//myVertexFormat.Init(2);
		//myVertexFormat.Add(VertexLayoutPosUV[0]);
		//myVertexFormat.Add(VertexLayoutPosUV[1]);
		//
		//CU::GrowingArray<SVertexTypePosUV> vertices;
		//CU::GrowingArray<int> indices;
		//float halfWidth = mySize.x * 0.5f;
		//float halfHeight = mySize.y * 0.5f;
		//SVertexTypePosUV v;
		//v.myPosition = { -halfWidth, -halfHeight, 0 };
		//v.myUV = { 0, 1 };
		//vertices.Add(v);
		//
		//v.myPosition = { -halfWidth, halfHeight, 0 };
		//v.myUV = { 0, 0 };
		//vertices.Add(v);
		//
		//v.myPosition = { halfWidth, -halfHeight, 0 };
		//v.myUV = { 1, 1 };
		//vertices.Add(v);
		//
		//v.myPosition = { halfWidth, halfHeight, 0 };
		//v.myUV = { 1, 0 };
		//vertices.Add(v);
		//
		//
		//indices.Add(0);
		//indices.Add(1);
		//indices.Add(2);
		//
		//indices.Add(3);
		//indices.Add(2);
		//indices.Add(1);
		//
		//myVertexBuffer = new SVertexBufferWrapper;
		//myVertexData = new SVertexDataWrapper;
		//myIndexBuffer = new SIndexBufferWrapper;
		//myIndexData = new SVertexIndexWrapper;
		//
		//myVertexData->myNrOfVertexes = vertices.Size();
		//myVertexData->myStride = sizeof(SVertexTypePosUV);
		//myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		//myVertexData->myVertexData = new char[myVertexData->mySize]();
		//memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);
		//
		//myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
		//myIndexData->myIndexCount = 6;
		//myIndexData->mySize = myIndexData->myIndexCount * 4;
		//
		//myIndexData->myIndexData = new char[myIndexData->mySize];
		//memcpy(myIndexData->myIndexData, &indices[0], myIndexData->mySize);
		//
		//InitiateVertexBuffer();
		//InitiateIndexBuffer();
		//InitiateBlendState();
	}

	void CSpriteModel::Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
	{
	//myWindowSize = CEngine::GetInstance()->GetWindowSize();
	//
	//mySize = aSize;
	//myPosition = aPosition;
	//myEffect = CAssetsContainer::GetInstance()->GetEffect("Data/Shaders/Sprite.fx");
	//myTexture = aShaderResource;
	//myEffect->SetAlbedo(aShaderResource);
	//
	//myVertexFormat.Init(2);
	//myVertexFormat.Add(VertexLayoutPosUV[0]);
	//myVertexFormat.Add(VertexLayoutPosUV[1]);
	//
	//CU::GrowingArray<SVertexTypePosUV> vertices;
	//CU::GrowingArray<int> indices;
	//float halfWidth = mySize.x * 0.5f;
	//float halfHeight = mySize.y * 0.5f;
	//SVertexTypePosUV v;
	//v.myPosition = { -halfWidth, -halfHeight, 1 };
	//v.myUV = { 0, 1 };
	//vertices.Add(v);
	//
	//v.myPosition = { -halfWidth, halfHeight, 1 };
	//v.myUV = { 0, 0 };
	//vertices.Add(v);
	//
	//v.myPosition = { halfWidth, -halfHeight, 1 };
	//v.myUV = { 1, 1 };
	//vertices.Add(v);
	//
	//v.myPosition = { halfWidth, halfHeight, 1 };
	//v.myUV = { 1, 0 };
	//vertices.Add(v);
	//
	//
	//indices.Add(0);
	//indices.Add(1);
	//indices.Add(2);
	//
	//indices.Add(3);
	//indices.Add(2);
	//indices.Add(1);
	//
	//myVertexBuffer = new SVertexBufferWrapper;
	//myVertexData = new SVertexDataWrapper;
	//myIndexBuffer = new SIndexBufferWrapper;
	//myIndexData = new SVertexIndexWrapper;
	//
	//myVertexData->myNrOfVertexes = vertices.Size();
	//myVertexData->myStride = sizeof(SVertexTypePosUV);
	//myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
	//myVertexData->myVertexData = new char[myVertexData->mySize]();
	//memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);
	//
	//myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
	//myIndexData->myIndexCount = 6;
	//myIndexData->mySize = myIndexData->myIndexCount * 4;
	//
	//myIndexData->myIndexData = new char[myIndexData->mySize];
	//memcpy(myIndexData->myIndexData, &indices[0], myIndexData->mySize);
	//
	//InitiateVertexBuffer();
	//InitiateIndexBuffer();
	//InitiateBlendState();
	}

	void CSpriteModel::Render()
	{
	//	if (!myEffect)
	//		return;
	//	myEffect->SetAlbedo(myTexture);
	//
	//	float blendFactor[4];
	//	blendFactor[0] = 0.f;
	//	blendFactor[1] = 0.f;
	//	blendFactor[2] = 0.f;
	//	blendFactor[3] = 0.f;
	//
	//	myEffect->SetBlendState(myBlendState, blendFactor);
	//
	//
	//	ID3D11DeviceContext& context = *CEngine::GetDirectX()->GetContext();
	//	context.IASetInputLayout(myVertexLayout);
	//	context.IASetVertexBuffers(0, 1, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
	//	context.IASetIndexBuffer(myIndexBuffer->myIndexBuffer, DXGI_FORMAT_R32_UINT, myIndexBuffer->myByteOffset);
	//
	//	context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//	/*D3DX11_TECHNIQUE_DESC techDesc;
	//	myEffect->GetTechnique()->GetDesc(&techDesc);
	//
	//	for (UINT p = 0; p < techDesc.Passes; ++p)
	//	{
	//		HRESULT hr = myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, &context);
	//		CEngine::GetDirectX()->HandleErrors(hr, "Failed to apply pass to context!");
	//		context.DrawIndexed(myIndexData->myIndexCount, 0, 0);
	//	}*/
	}

	Snowblind::CEffect* CSpriteModel::GetEffect()
	{
		return myEffect;
	}

	CU::Math::Vector2<float> CSpriteModel::GetSize()
	{
		return mySize;
	}

	const CU::Math::Vector2<float>& CSpriteModel::GetPosition()
	{
		return myPosition;
	}

	void CSpriteModel::InitiateVertexBuffer()
	{
		//	HRESULT hr;
		//
		//	/*D3DX11_PASS_DESC passDesc;
		//	hr = myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);*/
		//	CEngine::GetDirectX()->HandleErrors(hr, "Failed to get description from EffectPass!");
		//
		//	/*hr = CEngine::GetDirectX()->GetDevice()->
		//		CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size(), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);*/
		//	CEngine::GetDirectX()->SetDebugName(myVertexLayout, "SpriteModel Vertex Layout");
		//	CEngine::GetDirectX()->HandleErrors(hr, "Failed to create VertexLayout");
		//
		//	D3D11_BUFFER_DESC vertexBufferDesc;
		//	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		//	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		//	vertexBufferDesc.ByteWidth = myVertexData->mySize;
		//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//	vertexBufferDesc.MiscFlags = 0;
		//
		//	D3D11_SUBRESOURCE_DATA vertexData;
		//	vertexData.pSysMem = static_cast<void*>(myVertexData->myVertexData);
		//
		//	hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer);
		//	CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create VertexBuffer!");
		//
		//	myVertexBuffer->myStride = myVertexData->myStride;
		//	myVertexBuffer->myByteOffset = 0;
		//	myVertexBuffer->myStartSlot = 0;
		//	myVertexBuffer->myNrOfBuffers = 1;
	}

	void CSpriteModel::InitiateIndexBuffer()
	{
	//	D3D11_BUFFER_DESC indexDesc;
	//	ZeroMemory(&indexDesc, sizeof(indexDesc));
	//	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	//	indexDesc.ByteWidth = myIndexData->mySize;
	//	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	indexDesc.CPUAccessFlags = 0;
	//	indexDesc.MiscFlags = 0;
	//	indexDesc.StructureByteStride = 0;
	//
	//	D3D11_SUBRESOURCE_DATA indexData;
	//	ZeroMemory(&indexData, sizeof(indexData)), indexData.pSysMem = myIndexData->myIndexData;
	//	HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&indexDesc, &indexData, &myIndexBuffer->myIndexBuffer);
	//	CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create IndexBuffer");
	//
	//	myIndexBuffer->myIndexBufferFormat = myIndexData->myFormat;
	//	myIndexBuffer->myByteOffset = 0;
	}

	void CSpriteModel::InitiateBlendState()
	{
		//D3D11_BLEND_DESC blendDesc;
		//blendDesc.AlphaToCoverageEnable = true;
		//blendDesc.IndependentBlendEnable = false;
		//blendDesc.RenderTarget[0].BlendEnable = TRUE;
		//blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		//blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		//blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		//blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		//blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		//blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		//
		//HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBlendState(&blendDesc, &myBlendState);
		//CEngine::GetDirectX()->HandleErrors(hr, "Failed to create blendstate.");
		//CEngine::GetDirectX()->SetDebugName(myBlendState, "SpriteModel : BlendState");
	}

	void CSpriteModel::ConvertToNormalSpace()
	{
		//myPosition.x /= myWindowSize.myWidth;
		//myPosition.y /= myWindowSize.myHeight;
	}

};