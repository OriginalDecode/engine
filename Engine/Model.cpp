#include "Model.h"
#include "Camera.h"
#include <d3dx11effect.h>
#include "Engine.h"
#include "DirectX11.h"
#include "Effect.h"

namespace Snowblind
{
	CModel::CModel()
	{
		myAPI = reinterpret_cast<CDirectX11*>(CEngine::GetInstance()->GetAPI());
	}

	CModel::~CModel()
	{
	}

	void CModel::CreateCube(CEffect* anEffect, float aWidth, float aHeight, float aDepth)
	{
		aDepth;
		aWidth;
		aHeight;
		myEffect = anEffect;
		SVertexTypePosCol tempVertex;
#pragma region Vertex
		tempVertex.myPosition = { -1.0f, 1.0f, -1.0f };
		tempVertex.myColor = { 0.0f, 0.0f, 1.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, -1.0f };
		tempVertex.myColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, 1.0f };
		tempVertex.myColor = { 0.0f, 1.0f, 1.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, 1.0f };
		tempVertex.myColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, -1.0f, -1.0f };
		tempVertex.myColor = { 1.0f, 0.0f, 1.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, -1.0f };
		tempVertex.myColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, 1.0f };
		tempVertex.myColor = { .0f, 1.0f, 1.0f, 1.0f };
		myVertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, -1.0f, 1.0f };
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

		InitVertexBuffer();
		InitIndexBuffer();
	}

	void CModel::Render()
	{
		UINT stride = sizeof(myVertices[0]);
		UINT offset = 0;

		ID3D11DeviceContext* context = myAPI->GetContext();

		context->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p) //It's p because Passes
		{
			HRESULT hr = myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, context);
			assert(!FAILED(hr) && "Failed to apply pass to context!");
			context->DrawIndexed(myVertexIndices.Size(), 0, 0);
		}
	}

	void CModel::InitVertexBuffer()
	{
		HRESULT hr;
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"COLOR",	 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		UINT elementCount = ARRAYSIZE(layout);
		D3DX11_PASS_DESC passDesc;
		hr = myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		assert(!FAILED(hr) && "Failed to get Desc from effectpass!");
		//CDirectX11* api = reinterpret_cast<CDirectX11*>(CEngine::GetInstance()->GetAPI());
		hr = myAPI->GetDevice()->
			CreateInputLayout(layout, elementCount, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
		assert(!FAILED(hr) && "Failed to create VertexLayout");
		myAPI->GetContext()->IASetInputLayout(myVertexLayout);

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(SVertexTypePosCol) * myVertices.Size();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = &myVertices[0];

		hr = myAPI->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);
		assert(!FAILED(hr) && "Failed to Create VertexBuffer!");
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
		indexData.pSysMem = &myVertexIndices[0];

		//CDirectX11* api = reinterpret_cast<CDirectX11*>(CEngine::GetInstance()->GetAPI());
		HRESULT hr = myAPI->GetDevice()->
			CreateBuffer(&indexDesc, &indexData, &myIndexBuffer);

		assert(!FAILED(hr) && "Couldn't create IndexBuffer!");

	}

}