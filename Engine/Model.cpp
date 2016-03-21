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
#include "TextureContainer.h"

namespace Snowblind
{
	CModel::CModel(CCamera* aCamera)
		: myCamera(aCamera)
	{
		myAPI = CEngine::GetInstance()->GetAPI();
		myIsTextured = false;
	}

	CModel::~CModel()
	{
		myCamera = nullptr;
		mySurfaces.DeleteAll();
		SAFE_DELETE(myVertexData->myVertexData);
		SAFE_DELETE(myIndexData->myIndexData);
		SAFE_DELETE(myVertexBuffer);
		SAFE_DELETE(myVertexData);
		SAFE_DELETE(myIndexBuffer);
		SAFE_DELETE(myIndexData);
	}

	void CModel::CreateTriangle(const std::string& anEffectPath)
	{
		myEffect = CEffectContainer::GetInstance()->GetEffect(anEffectPath);
		myVertexFormat.Init(2);
		myVertexFormat.Add(VertexLayoutPosCol[0]);
		myVertexFormat.Add(VertexLayoutPosCol[1]);
		SVertexTypePosCol v;
		v.myPosition = { 0.f, 0.5f, 0.f };
		v.myColor = { 1.f, 0.f, 0.f, 1.f };
		myVertices.Add(v);

		v.myPosition = { 0.5f, -0.5f, 0.f };
		v.myColor = { 0.f, 0.f, 1.f, 1.f };
		myVertices.Add(v);

		v.myPosition = { -0.5f, -0.5f, 0.f };
		v.myColor = { 0.f, 1.f, 0.f, 1.f };
		myVertices.Add(v);

		myVertexBuffer = new SVertexBufferWrapper;
		myVertexData = new SVertexDataWrapper;

		myVertexData->myNrOfVertexes = myVertices.Size();
		myVertexData->myStride = sizeof(SVertexTypePosCol);
		myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		myVertexData->myVertexData = new char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &myVertices[0], myVertexData->mySize);


		InitVertexBuffer();
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
		tempVertex.myColor = { 0.0f, 1.0f, 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, -halfHeight, halfDepth };
		tempVertex.myColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		vertices.Add(tempVertex);
#pragma endregion

#pragma region Index

		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(0);

		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(2);

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
		myIndexData->myIndexCount = 6 * 6;
		myIndexData->mySize = myIndexData->myIndexCount * 4;

		myIndexData->myIndexData = new char[myIndexData->mySize];
		memcpy(myIndexData->myIndexData, &indexes[0], myIndexData->mySize);

		InitVertexBuffer();
		InitIndexBuffer();
	}

	void CModel::CreateTexturedCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth)
	{
		myIsTextured = true;
		float halfWidth = aWidth * 0.5f;
		float halfDepth = aDepth * 0.5f;
		float halfHeight = aHeight * 0.5f;

		CU::GrowingArray<SVertexTypePosNormUV> vertices;
		CU::GrowingArray<int> indexes;

		myVertexFormat.Init(3);
		myVertexFormat.Add(VertexLayoutPosNormUV[0]);
		myVertexFormat.Add(VertexLayoutPosNormUV[1]);
		myVertexFormat.Add(VertexLayoutPosNormUV[2]);


		myEffect = CEffectContainer::GetInstance()->GetEffect(anEffectPath);

#pragma region Vertex
		SVertexTypePosNormUV tempVertex;

		//Top
		#pragma region Top
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
#pragma endregion
		//Bottom
		#pragma region Bottom
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
#pragma endregion
		//Left
		#pragma region Left
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
#pragma endregion
		//Right
		#pragma region Right
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
#pragma endregion
		//Front
		#pragma region Front
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
#pragma endregion
		//Back
		#pragma region Back
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

#pragma endregion

#pragma region Index
		//Top
		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(0);

		indexes.Add(2);
		indexes.Add(1);
		indexes.Add(3);

		//Bottom
		indexes.Add(6);
		indexes.Add(4);
		indexes.Add(5);

		indexes.Add(7);
		indexes.Add(4);
		indexes.Add(6);

		//Left
		indexes.Add(11);
		indexes.Add(9);
		indexes.Add(8);

		indexes.Add(10);
		indexes.Add(9);
		indexes.Add(11);

		//Right
		indexes.Add(14);
		indexes.Add(12);
		indexes.Add(13);

		indexes.Add(15);
		indexes.Add(12);
		indexes.Add(14);

		//Front
		indexes.Add(19);
		indexes.Add(17);
		indexes.Add(16);

		indexes.Add(18);
		indexes.Add(17);
		indexes.Add(19);

		//Back
		indexes.Add(22);
		indexes.Add(20);
		indexes.Add(21);

		indexes.Add(23);
		indexes.Add(20);
		indexes.Add(22);
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

		/*	CSurface* tempSurface = new CSurface(myEffect);
			tempSurface->SetVertexStart(0);
			tempSurface->SetIndexCount(myIndexData->myIndexCount);
			tempSurface->SetPrimology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			tempSurface->SetTexture("AlbedoTexture", "Data/Textures/col.dds");
			mySurfaces.Add(tempSurface);*/

		myEffect->SetAlbedo(Snowblind::CTextureContainer::GetInstance()->GetTexture("Data/Textures/colors.dds"));
		InitVertexBuffer();
		InitIndexBuffer();
	}

	void CModel::Render()
	{
		if (!myEffect)
			return;

		ID3D11DeviceContext* context = myAPI->GetContext();

		context->IASetInputLayout(myVertexLayout);

		context->IASetVertexBuffers(0, 1, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		context->IASetIndexBuffer(myIndexBuffer->myIndexBuffer, DXGI_FORMAT_R32_UINT, myIndexBuffer->myByteOffset);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);

		if (myIsTextured)
		{
			/*for (int i = 0; i < mySurfaces.Size(); i++)
			{*/

			//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				HRESULT hr = myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, context);
				myAPI->HandleErrors(hr, "Failed to apply pass to context!");
				context->DrawIndexed(myIndexData->myIndexCount, 0, 0);
			}
			//}
		}
		else
		{
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				HRESULT hr = myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, context);
				myAPI->HandleErrors(hr, "Failed to apply pass to context!");
				context->DrawIndexed(myIndexData->myIndexCount, 0, 0);
			}
		}
	}

	void CModel::RenderPolygon()
	{

		myAPI->GetContext()->IASetVertexBuffers(0, 1, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myStride);

		myAPI->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, myAPI->GetContext());
			myAPI->GetContext()->Draw(3, 0);
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
		ZeroMemory(&indexDesc, sizeof(indexDesc));
		indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexDesc.ByteWidth = myIndexData->mySize;
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;
		indexDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexData, sizeof(indexData)),
		indexData.pSysMem = myIndexData->myIndexData;

		HRESULT hr = myAPI->GetDevice()->CreateBuffer(&indexDesc, &indexData, &myIndexBuffer->myIndexBuffer);
		myAPI->HandleErrors(hr, "Failed to Create IndexBuffer");

		myIndexBuffer->myIndexBufferFormat = myIndexData->myFormat;
		myIndexBuffer->myByteOffset = 0;

	}
}