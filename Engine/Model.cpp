#include "Model.h"

#include <d3dx11effect.h>
#include "DirectX11.h"
#include <DL_Debug.h>
#include "Effect.h"
#include "EffectContainer.h"
#include "Engine.h"
#include "EngineDefines.h"
#include "IndexWrapper.h"
#include "VertexWrapper.h"
#include "VertexTypes.h"

namespace Snowblind
{
	CModel::CModel()
	{
		myAPI = reinterpret_cast<CDirectX11*>(CEngine::GetInstance()->GetAPI());
	}

	CModel::~CModel()
	{
		SAFE_DELETE(myIndexData->myIndexData);
		SAFE_DELETE(myVertexBuffer);
		SAFE_DELETE(myVertexData);
		SAFE_DELETE(myIndexBuffer);
		SAFE_DELETE(myIndexData);
	}

	void CModel::CreateCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth)
	{
		float halfWidth = aWidth *0.5f;
		float halfDepth = aDepth *0.5f;
		float halfHeight = aHeight *0.5f;
	
		myVertexFormat.Init(2);
		myVertexFormat.Add(VertexLayoutPosCol[0]);

		myEffect = CEffectContainer::GetInstance()->GetEffect(anEffectPath);

		SVertexTypePosCol tempVertex;
#pragma region Vertex
		tempVertex.myPosition = { -halfWidth, halfHeight, -halfDepth};
		tempVertex.myColor = { 0.0f, 0.0f, 1.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, halfHeight, -halfDepth };
		tempVertex.myColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, halfHeight, halfDepth};
		tempVertex.myColor = { 0.0f, 1.0f, 1.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, halfHeight, halfDepth };
		tempVertex.myColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, -halfHeight, -halfDepth };
		tempVertex.myColor = { 1.0f, 0.0f, 1.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, -halfHeight, -halfDepth };
		tempVertex.myColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, -halfHeight, halfDepth };
		tempVertex.myColor = { .0f, 1.0f, 1.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, -halfHeight, halfDepth };
		tempVertex.myColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		myVertices.Add(tempVertex);
#pragma endregion

#pragma region Index

		myVertexIndices.Add(3);
		myVertexIndices.Add(1);
		myVertexIndices.Add(0);
		myVertexIndices.Add(2);
		myVertexIndices.Add(1);
		myVertexIndices.Add(3);
		myVertexIndices.Add(0);
		myVertexIndices.Add(5);
		myVertexIndices.Add(4);
		myVertexIndices.Add(1);
		myVertexIndices.Add(5);
		myVertexIndices.Add(0);
		myVertexIndices.Add(3);
		myVertexIndices.Add(4);
		myVertexIndices.Add(7);
		myVertexIndices.Add(0);
		myVertexIndices.Add(4);
		myVertexIndices.Add(3);
		myVertexIndices.Add(1);
		myVertexIndices.Add(6);
		myVertexIndices.Add(5);
		myVertexIndices.Add(2);
		myVertexIndices.Add(6);
		myVertexIndices.Add(1);
		myVertexIndices.Add(2);
		myVertexIndices.Add(7);
		myVertexIndices.Add(6);
		myVertexIndices.Add(3);
		myVertexIndices.Add(7);
		myVertexIndices.Add(2);
		myVertexIndices.Add(6);
		myVertexIndices.Add(4);
		myVertexIndices.Add(5);
		myVertexIndices.Add(7);
		myVertexIndices.Add(4);
		myVertexIndices.Add(6);
#pragma endregion
		myVertexBuffer = new SVertexBufferWrapper;
		myVertexData = new SVertexDataWrapper;
		myIndexBuffer = new SIndexBufferWrapper;
		myIndexData = new SVertexIndexWrapper;

		myVertexData->myNrOfVertexes = myVertices.Size();
		myVertexData->myStride = sizeof(SVertexTypePosCol);
		myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		myVertexData->myVertexData = new  char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &myVertices[0], myVertexData->mySize);

		myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
		myIndexData->myIndexCount = 6 * 6;
		myIndexData->mySize = myIndexData->myIndexCount * 4;

		myIndexData->myIndexData = new char[myIndexData->mySize];
		memcpy(myIndexData->myIndexData, &myVertexIndices[0], myIndexData->mySize);

		InitVertexBuffer();
		InitIndexBuffer();
	}

	void CModel::Render()
	{
		UINT stride = sizeof(myVertices[0]);
		UINT offset = 0;

		ID3D11DeviceContext* context = myAPI->GetContext();

		context->IASetVertexBuffers(0, 1, &myVertexBuffer->myVertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(myIndexBuffer->myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p) 
		{
			HRESULT hr = myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, context);
			myAPI->HandleErrors(hr, "Failed to apply pass to context!");
			context->DrawIndexed(myVertexIndices.Size(), 0, 0);
		}
	}

	void CModel::InitVertexBuffer()
	{
		HRESULT hr;
		
		UINT elementCount = ARRAYSIZE(VertexLayoutPosCol);
		D3DX11_PASS_DESC passDesc;
		hr = myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		myAPI->HandleErrors(hr, "Failed to get description from EffectPass!");
		hr = myAPI->GetDevice()->
			CreateInputLayout(VertexLayoutPosCol, elementCount, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
		myAPI->HandleErrors(hr, "Failed to create VertexLayout");
		myAPI->GetContext()->IASetInputLayout(myVertexLayout);

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = myVertexData->mySize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = myVertexData->myVertexData;

		hr = myAPI->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer);
		myAPI->HandleErrors(hr, "Failed to Create VertexBuffer!");

		myVertexBuffer->myStride = myVertexData->myStride;
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNrOfBuffers = 1;

	}

	void CModel::InitIndexBuffer()
	{

		D3D11_BUFFER_DESC indexDesc;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;
		indexDesc.ByteWidth = sizeof(SVertexTypePosCol) * myVertices.Size();
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = myIndexData->myIndexData;

		HRESULT hr = myAPI->GetDevice()->
			CreateBuffer(&indexDesc, &indexData, &myIndexBuffer->myIndexBuffer);

		myIndexBuffer->myByteOffset = 0;
		myIndexBuffer->myIndexBufferFormat = myIndexData->myFormat;

		myAPI->HandleErrors(hr, "Failed to Create IndexBuffer");

	}

}