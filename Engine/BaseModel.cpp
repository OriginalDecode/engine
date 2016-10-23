#include "stdafx.h"
#include "BaseModel.h"

namespace Snowblind
{
	CBaseModel::CBaseModel()
	{
		myEngine = CEngine::GetInstance();
#ifdef SNOWBLIND_DX11
		myAPI = CEngine::GetAPI();
		myContext = myAPI->GetContext();
#endif
	}


	CBaseModel::~CBaseModel()
	{
#ifdef SNOWBLIND_DX11
		SAFE_DELETE(myVertexBuffer);
		SAFE_DELETE(myVertexData);
		SAFE_DELETE(myIndexBuffer);
		SAFE_DELETE(myIndexData);
		SAFE_RELEASE(myVertexLayout);
		SAFE_RELEASE(myConstantBuffer);
#endif
	}

	void CBaseModel::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
#ifdef SNOWBLIND_DX11
		myContext->IASetInputLayout(myVertexLayout);
		myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		myContext->IASetVertexBuffers(0, 1, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);

		if (myIndexBuffer)
			myContext->IASetIndexBuffer(myIndexBuffer->myIndexBuffer, DXGI_FORMAT_R32_UINT, myIndexBuffer->myByteOffset);

		myAPI->SetVertexShader(myEffect->GetVertexShader() ? myEffect->GetVertexShader()->vertexShader : nullptr);
		myAPI->SetPixelShader(myEffect->GetPixelShader() ? myEffect->GetPixelShader()->pixelShader : nullptr);
		SetMatrices(aCameraOrientation, aCameraProjection);
#endif
	}

	void CBaseModel::SetEffect(CEffect* anEffect)
	{
#ifdef SNOWBLIND_DX11
		myEffect = anEffect;
#endif
	}

	void CBaseModel::InitVertexBuffer()
	{
#ifdef SNOWBLIND_DX11
		myVertexBuffer = new SVertexBufferWrapper();
		HRESULT hr;
		void* shader = myEffect->GetVertexShader()->compiledShader;
		int size = myEffect->GetVertexShader()->shaderSize;

		hr = myAPI->GetDevice()->
			CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size()
				, shader
				, size
				, &myVertexLayout);

		myAPI->SetDebugName(myVertexLayout, "Model Vertex Layout");
		myAPI->HandleErrors(hr, "[BaseModel] : Failed to create VertexLayout");

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = myVertexData->mySize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = static_cast<void*>(myVertexData->myVertexData);

		hr = myAPI->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer);
		myAPI->SetDebugName(myVertexBuffer->myVertexBuffer, "Model vb");
		myAPI->HandleErrors(hr, "[BaseModel] : Failed to Create VertexBuffer!");

		myVertexBuffer->myStride = myVertexData->myStride;
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNrOfBuffers = 1;
#endif
	}

	void CBaseModel::InitIndexBuffer()
	{
#ifdef SNOWBLIND_DX11

		myIndexBuffer = new SIndexBufferWrapper;

		D3D11_BUFFER_DESC indexDesc;
		ZeroMemory(&indexDesc, sizeof(indexDesc));
		indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexDesc.ByteWidth = myIndexData->mySize;
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;
		indexDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexData, sizeof(indexData));
		indexData.pSysMem = static_cast<void*>(myIndexData->myIndexData);

		HRESULT hr = myAPI->GetDevice()->CreateBuffer(&indexDesc, &indexData, &myIndexBuffer->myIndexBuffer);
		myAPI->SetDebugName(myIndexBuffer->myIndexBuffer, "Model ib");
		myAPI->HandleErrors(hr, "[BaseModel] : Failed to Create IndexBuffer");

		myIndexBuffer->myIndexBufferFormat = myIndexData->myFormat;
		myIndexBuffer->myByteOffset = 0;
#endif
	}
};