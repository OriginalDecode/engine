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

		SAFE_RELEASE(myVertexLayout);
		SAFE_RELEASE(myConstantBuffer);
	}

	void CSpriteModel::Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
	{
		myWindowSize = CEngine::GetInstance()->GetWindowSize();

		myTexturePath = aTexturePath;
		mySize = aSize;
		myPosition = aPosition;
		myTexture = CEngine::GetInstance()->GetTexture(myTexturePath)->GetShaderView();
		myEffect = CEngine::GetInstance()->GetEffect("Data/Shaders/T_Sprite.json");

		myVertexFormat.Init(2);
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
	}

	void CSpriteModel::Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
	{
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
	}

	void CSpriteModel::Render(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix)
	{
		CEngine::GetDirectX()->SetBlendState(eBlendStates::ALPHA_BLEND);
		if (!myEffect)
			return;
		CEngine::GetDirectX()->SetSamplerState(eSamplerStates::LINEAR_CLAMP);

		SetMatrices(anOrientation, a2DCameraOrientation, anOrthogonalProjectionMatrix);

		ID3D11DeviceContext& context = *CEngine::GetDirectX()->GetContext();
		context.IASetInputLayout(myVertexLayout);
		context.IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNrOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		context.IASetIndexBuffer(myIndexBuffer->myIndexBuffer, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		UpdateConstantBuffer();

		CEngine::GetDirectX()->SetVertexShader(myEffect->GetVertexShader()->vertexShader);
		context.VSSetConstantBuffers(0, 1, &myConstantBuffer);
		CEngine::GetDirectX()->SetPixelShader(myEffect->GetPixelShader()->pixelShader);
		ID3D11ShaderResourceView* srv = myTexture;
		context.PSSetShaderResources(0, 1, &srv);

		context.DrawIndexed(6, 0, 0);

		srv = nullptr;
		context.PSSetShaderResources(0, 1, &srv);

		CEngine::GetDirectX()->SetBlendState(eBlendStates::NO_BLEND);

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

	void CSpriteModel::UpdateConstantBuffer()
	{
		myConstantStruct->scale = mySize;


		DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");

		D3D11_MAPPED_SUBRESOURCE msr;
		CEngine::GetDirectX()->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SSpriteConstantBuffer* ptr = (SSpriteConstantBuffer*)msr.pData;
			memcpy(ptr, &myConstantStruct->world.myMatrix[0], sizeof(SSpriteConstantBuffer));
		}

		CEngine::GetDirectX()->GetContext()->Unmap(myConstantBuffer, 0);
	}

	void CSpriteModel::InitiateVertexBuffer()
	{
		HRESULT hr;

		hr = CEngine::GetDirectX()->GetDevice()->
			CreateInputLayout(&myVertexFormat[0]
				, myVertexFormat.Size()
				, myEffect->GetVertexShader()->compiledShader
				, myEffect->GetVertexShader()->shaderSize
				, &myVertexLayout);
		CEngine::GetDirectX()->SetDebugName(myVertexLayout, "SpriteModel Vertex Layout");
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to create VertexLayout");

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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create VertexBuffer!");

		myVertexBuffer->myStride = myVertexData->myStride;
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNrOfBuffers = 1;
	}

	void CSpriteModel::InitiateIndexBuffer()
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

	void CSpriteModel::InitConstantBuffer()
	{
		myConstantStruct = new SSpriteConstantBuffer;

		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SSpriteConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
		CEngine::GetDirectX()->SetDebugName(myConstantBuffer, "Font Constant Buffer");
		CEngine::GetDirectX()->HandleErrors(hr, "[Font] : Failed to Create Constant Buffer, ");
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