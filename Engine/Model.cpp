#include "Model.h"

#include "Camera.h"
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
#include "Surface.h"

namespace Snowblind
{
	CModel::CModel(CCamera* aCamera)
		: myCamera(aCamera)
	{
		myAPI = reinterpret_cast<CDirectX11*>(CEngine::GetInstance()->GetAPI());
		myIsTextured = false;
	}

	CModel::~CModel()
	{
		mySurfaces.DeleteAll();
		SAFE_DELETE(myVertexData->myVertexData);
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
		CU::GrowingArray<SVertexTypePosCol> vertices;
		CU::GrowingArray<int> indexes;
		myVertexFormat.Init(2);
		myVertexFormat.Add(VertexLayoutPosCol[0]);
		myVertexFormat.Add(VertexLayoutPosCol[1]);

		myEffect = CEffectContainer::GetInstance()->GetEffect(anEffectPath);

		SVertexTypePosCol tempVertex;

#pragma region Vertex
		tempVertex.myPosition = { -halfWidth, halfHeight, -halfDepth };
		tempVertex.myColor = { 0.0f, 0.0f, 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, halfHeight, -halfDepth };
		tempVertex.myColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, halfHeight, halfDepth };
		tempVertex.myColor = { 0.0f, 1.0f, 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, halfHeight, halfDepth };
		tempVertex.myColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, -halfHeight, -halfDepth };
		tempVertex.myColor = { 1.0f, 0.0f, 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, -halfHeight, -halfDepth };
		tempVertex.myColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, -halfHeight, halfDepth };
		tempVertex.myColor = { .0f, 1.0f, 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, -halfHeight, halfDepth };
		tempVertex.myColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		vertices.Add(tempVertex);
#pragma endregion

#pragma region Index

		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(0);
		indexes.Add(2);
		indexes.Add(1);
		indexes.Add(3);
		indexes.Add(0);
		indexes.Add(5);
		indexes.Add(4);
		indexes.Add(1);
		indexes.Add(5);
		indexes.Add(0);
		indexes.Add(3);
		indexes.Add(4);
		indexes.Add(7);
		indexes.Add(0);
		indexes.Add(4);
		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(6);
		indexes.Add(5);
		indexes.Add(2);
		indexes.Add(6);
		indexes.Add(1);
		indexes.Add(2);
		indexes.Add(7);
		indexes.Add(6);
		indexes.Add(3);
		indexes.Add(7);
		indexes.Add(2);
		indexes.Add(6);
		indexes.Add(4);
		indexes.Add(5);
		indexes.Add(7);
		indexes.Add(4);
		indexes.Add(6);
#pragma endregion

		myVertexBuffer = new SVertexBufferWrapper;
		myVertexData = new SVertexDataWrapper;
		myIndexBuffer = new SIndexBufferWrapper;
		myIndexData = new SVertexIndexWrapper;

		myVertexData->myNrOfVertexes = vertices.Size();
		myVertexData->myStride = sizeof(SVertexTypePosCol);
		myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		myVertexData->myVertexData = new char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);

		myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
		myIndexData->myIndexCount = 6*6;
		myIndexData->mySize = myIndexData->myIndexCount * 4;

		myIndexData->myIndexData = new char[myIndexData->mySize];
		memcpy(myIndexData->myIndexData, &indexes[0], myIndexData->mySize);

		InitVertexBuffer();
		InitIndexBuffer();
	}

	void CModel::CreateTexturedCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth)
	{
		myIsTextured = true;
		float halfWidth = aWidth *0.5f;
		float halfDepth = aDepth *0.5f;
		float halfHeight = aHeight *0.5f;

		CU::GrowingArray<SVertexTypePosNormUV> vertices;
		CU::GrowingArray<int> indexes;

		myVertexFormat.Init(3);
		myVertexFormat.Add(VertexLayoutPosNormUV[0]);
		myVertexFormat.Add(VertexLayoutPosNormUV[1]);
		myVertexFormat.Add(VertexLayoutPosNormUV[2]);


		myEffect = CEffectContainer::GetInstance()->GetEffect(anEffectPath);

		SVertexTypePosNormUV tempVertex;
#pragma region Vertex
		tempVertex.myPosition = { -1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 1.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 1.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 1.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 1.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 1.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 1.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, -1.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 0.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, -1.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, -1.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 1.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, -1.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 1.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { -1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 0.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { -1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 0.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { -1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 1.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { -1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 1.0f };

		vertices.Add(tempVertex);


		tempVertex.myPosition = { 1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 0.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 0.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 1.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, -1.0f };
		tempVertex.myUV = { 0.0f, 0.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, -1.0f };
		tempVertex.myUV = { 1.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, -1.0f };
		tempVertex.myUV = { 1.0f, 1.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, -1.0f };
		tempVertex.myUV = { 0.0f, 1.0f };

		vertices.Add(tempVertex);


		tempVertex.myPosition = { -1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, 1.0f };
		tempVertex.myUV = { 0.0f, 0.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, 1.0f };
		tempVertex.myUV = { 1.0f, 0.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, 1.0f };
		tempVertex.myUV = { 1.0f, 1.0f };

		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, 1.0f };
		tempVertex.myUV = { 0.0f, 1.0f };

		vertices.Add(tempVertex);
#pragma endregion

#pragma region Index

		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(0);
		indexes.Add(2);
		indexes.Add(1);
		indexes.Add(3);
		indexes.Add(0);
		indexes.Add(5);
		indexes.Add(4);
		indexes.Add(1);
		indexes.Add(5);
		indexes.Add(0);
		indexes.Add(3);
		indexes.Add(4);
		indexes.Add(7);
		indexes.Add(0);
		indexes.Add(4);
		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(6);
		indexes.Add(5);
		indexes.Add(2);
		indexes.Add(6);
		indexes.Add(1);
		indexes.Add(2);
		indexes.Add(7);
		indexes.Add(6);
		indexes.Add(3);
		indexes.Add(7);
		indexes.Add(2);
		indexes.Add(6);
		indexes.Add(4);
		indexes.Add(5);
		indexes.Add(7);
		indexes.Add(4);
		indexes.Add(6);
#pragma endregion
		myVertexBuffer = new SVertexBufferWrapper;
		myVertexData = new SVertexDataWrapper;
		myIndexBuffer = new SIndexBufferWrapper;
		myIndexData = new SVertexIndexWrapper;

		myVertexData->myNrOfVertexes = vertices.Size();
		myVertexData->myStride = sizeof(SVertexTypePosNormUV);
		myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		myVertexData->myVertexData = new  char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);

		myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
		myIndexData->myIndexCount = indexes.Size();
		myIndexData->mySize = myIndexData->myIndexCount * 4;

		myIndexData->myIndexData = new char[myIndexData->mySize];
		memcpy(myIndexData->myIndexData, &indexes[0], myIndexData->mySize);

		CSurface* tempSurface = new CSurface(myEffect);
		tempSurface->SetVertexStart(0);
		tempSurface->SetIndexCount(myIndexData->myIndexCount);
		tempSurface->SetPrimology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		tempSurface->SetTexture("AlbedoTexture", "Data/Textures/colors.dds");
		mySurfaces.Add(tempSurface);

		InitVertexBuffer();
		InitIndexBuffer();
	}

	void CModel::Render()
	{
		ID3D11DeviceContext* context = myAPI->GetContext();
		context->IASetVertexBuffers(0, 1, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		context->IASetIndexBuffer(myIndexBuffer->myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		myEffect->SetMatrices(myOrientation, myCamera->GetOrientation(), myCamera->GetProjection());

		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);
		if (myIsTextured)
		{
			for (int i = 0; i < mySurfaces.Size(); i++)
			{
				mySurfaces[i]->Activate();
				for (UINT p = 0; p < techDesc.Passes; ++p)
				{
					HRESULT hr = myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, context);
					myAPI->HandleErrors(hr, "Failed to apply pass to context!");
					context->DrawIndexed(mySurfaces[i]->GetIndexCount(), mySurfaces[i]->GetStartVertex(), 0);
				}
			}
		}
		else
		{
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				HRESULT hr = myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, context);
				myAPI->HandleErrors(hr, "Failed to apply pass to context!");
				context->DrawIndexed(myIndexData->myIndexCount, 0, 0);
			}
		}
	}

	void CModel::InitVertexBuffer()
	{
		HRESULT hr;

		D3DX11_PASS_DESC passDesc;
		hr = myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		myAPI->HandleErrors(hr, "Failed to get description from EffectPass!");

		hr = myAPI->GetDevice()->
			CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size(), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
		myAPI->HandleErrors(hr, "Failed to create VertexLayout");
		myAPI->GetContext()->IASetInputLayout(myVertexLayout);

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
		indexDesc.ByteWidth = myIndexData->mySize;
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