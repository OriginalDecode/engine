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
		SAFE_DELETE(myConstantStruct);

#ifdef SNOWBLIND_DX11
		SAFE_RELEASE(myVertexLayout);
		SAFE_RELEASE(myConstantBuffer);
#endif
	}

	void CSpriteModel::Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
	{
#ifdef SNOWBLIND_DX11
		myWindowSize = CEngine::GetInstance()->GetWindowSize();

		myTexturePath = aTexturePath;
		mySize = aSize;
		myPosition = aPosition;
		CTexture* text = CEngine::GetInstance()->GetTexture(myTexturePath);
		myTexture = text->GetShaderView();
		myEffect = CEngine::GetInstance()->GetEffect("Data/Shaders/T_Sprite.json");

		myVertexFormat.Add(VertexLayoutPosUV[0]);
		myVertexFormat.Add(VertexLayoutPosUV[1]);

		CU::GrowingArray<SVertexTypePosUV> vertices;
		CU::GrowingArray<int> indices;
		float halfWidth = mySize.x * 0.5f;
		float halfHeight = mySize.y * 0.5f;
		SVertexTypePosUV v;
		v.myPosition = { -halfWidth, -halfHeight, 0 };
		v.myUV = { 0, 1 };
		vertices.Add(v);

		v.myPosition = { -halfWidth, halfHeight, 0 };
		v.myUV = { 0, 0 };
		vertices.Add(v);

		v.myPosition = { halfWidth, -halfHeight, 0 };
		v.myUV = { 1, 1 };
		vertices.Add(v);

		v.myPosition = { halfWidth, halfHeight, 0 };
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

		InitiateVertexBuffer();
		InitiateIndexBuffer();
		InitConstantBuffer();
		//InitiateBlendState();
#endif
	}

	void CSpriteModel::Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
	{
#ifdef SNOWBLIND_DX11
		myWindowSize = CEngine::GetInstance()->GetWindowSize();

		mySize = aSize;
		myPosition = aPosition;
		myEffect = CEngine::GetInstance()->GetEffect("Data/Shaders/T_Sprite.json");
		myTexture = aShaderResource;
		//myEffect->SetAlbedo(aShaderResource);

		myVertexFormat.Init(2);
		myVertexFormat.Add(VertexLayoutPosUV[0]);
		myVertexFormat.Add(VertexLayoutPosUV[1]);

		CU::GrowingArray<SVertexTypePosUV> vertices;
		CU::GrowingArray<int> indices;
		float halfWidth = mySize.x * 0.5f;
		float halfHeight = mySize.y * 0.5f;
		SVertexTypePosUV v;
		v.myPosition = { -halfWidth, -halfHeight, 1 };
		v.myUV = { 0, 1 };
		vertices.Add(v);

		v.myPosition = { -halfWidth, halfHeight, 1 };
		v.myUV = { 0, 0 };
		vertices.Add(v);

		v.myPosition = { halfWidth, -halfHeight, 1 };
		v.myUV = { 1, 1 };
		vertices.Add(v);

		v.myPosition = { halfWidth, halfHeight, 1 };
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

		InitiateVertexBuffer();
		InitiateIndexBuffer();
		InitConstantBuffer();
		//InitiateBlendState();
#endif
	}

	void CSpriteModel::Render(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix)
	{
#ifdef SNOWBLIND_DX11
		CEngine::GetAPI()->SetBlendState(eBlendStates::ALPHA_BLEND);
		if (!myEffect)
			return;
		CEngine::GetAPI()->SetSamplerState(eSamplerStates::LINEAR_CLAMP);

		SetMatrices(anOrientation, a2DCameraOrientation, anOrthogonalProjectionMatrix);

		ID3D11DeviceContext& context = *CEngine::GetAPI()->GetContext();
		context.IASetInputLayout(myVertexLayout);
		context.IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNrOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		context.IASetIndexBuffer(myIndexBuffer->myIndexBuffer, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		UpdateConstantBuffer();

		CEngine::GetAPI()->SetVertexShader(myEffect->GetVertexShader()->vertexShader);
		context.VSSetConstantBuffers(0, 1, &myConstantBuffer);
		CEngine::GetAPI()->SetPixelShader(myEffect->GetPixelShader()->pixelShader);
		ID3D11ShaderResourceView* srv = myTexture;
		context.PSSetShaderResources(0, 1, &srv);

		context.DrawIndexed(6, 0, 0);

		srv = nullptr;
		context.PSSetShaderResources(0, 1, &srv);

		CEngine::GetAPI()->SetBlendState(eBlendStates::NO_BLEND);
#endif
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

	void CSpriteModel::SetTexture(ID3D11ShaderResourceView* srv)
	{
		myTexture = srv;
	}

	void CSpriteModel::UpdateConstantBuffer()
	{
#ifdef SNOWBLIND_DX11
		myConstantStruct->scale = mySize;


		DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");

		D3D11_MAPPED_SUBRESOURCE msr;
		CEngine::GetAPI()->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SSpriteConstantBuffer* ptr = (SSpriteConstantBuffer*)msr.pData;
			memcpy(ptr, &myConstantStruct->world.myMatrix[0], sizeof(SSpriteConstantBuffer));
		}

		CEngine::GetAPI()->GetContext()->Unmap(myConstantBuffer, 0);
#endif
	}

	void CSpriteModel::InitiateVertexBuffer()
	{
#ifdef SNOWBLIND_DX11
		HRESULT hr;

		hr = CEngine::GetAPI()->GetDevice()->
			CreateInputLayout(&myVertexFormat[0]
				, myVertexFormat.Size()
				, myEffect->GetVertexShader()->compiledShader
				, myEffect->GetVertexShader()->shaderSize
				, &myVertexLayout);
		CEngine::GetAPI()->SetDebugName(myVertexLayout, "SpriteModel Vertex Layout");
		CEngine::GetAPI()->HandleErrors(hr, "Failed to create VertexLayout");

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = myVertexData->mySize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = static_cast<void*>(myVertexData->myVertexData);

		hr = CEngine::GetAPI()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to Create VertexBuffer!");

		myVertexBuffer->myStride = myVertexData->myStride;
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNrOfBuffers = 1;
#endif
	}

	void CSpriteModel::InitiateIndexBuffer()
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
		HRESULT hr = CEngine::GetAPI()->GetDevice()->CreateBuffer(&indexDesc, &indexData, &myIndexBuffer->myIndexBuffer);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to Create IndexBuffer");

		myIndexBuffer->myIndexBufferFormat = myIndexData->myFormat;
		myIndexBuffer->myByteOffset = 0;
#endif
	}

	void CSpriteModel::InitConstantBuffer()
	{
#ifdef SNOWBLIND_DX11
		myConstantStruct = new SSpriteConstantBuffer;

		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SSpriteConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = CEngine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
		CEngine::GetAPI()->SetDebugName(myConstantBuffer, "Font Constant Buffer");
		CEngine::GetAPI()->HandleErrors(hr, "[Font] : Failed to Create Constant Buffer, ");
#endif
	}

	void CSpriteModel::ConvertToNormalSpace()
	{
		myPosition.x /= myWindowSize.myWidth;
		myPosition.y /= myWindowSize.myHeight;
	}

	void CSpriteModel::SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix)
	{
		myConstantStruct->world = anOrientation;
		myConstantStruct->invertedView = a2DCameraOrientation;
		myConstantStruct->projection = anOrthogonalProjectionMatrix;
	}

};