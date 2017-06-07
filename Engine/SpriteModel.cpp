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
	myWindowSize = Engine::GetInstance()->GetWindowSize();

	myTexturePath = aTexturePath;
	mySize = aSize;
	myPosition = aPosition;
	Texture* text = Engine::GetInstance()->GetTexture(myTexturePath);
	myTexture = text->GetShaderView();
	myEffect = Engine::GetInstance()->GetEffect("Shaders/T_Sprite.json");

	myVertexFormat.Add(VertexLayoutPosUV[0]);
	myVertexFormat.Add(VertexLayoutPosUV[1]);

	CU::GrowingArray<VertexTypePosUV> vertices;
	CU::GrowingArray<int> indices;
	float halfWidth = mySize.x * 0.5f;
	float halfHeight = mySize.y * 0.5f;
	VertexTypePosUV v;
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

	myVertexBuffer = new VertexBufferWrapper;
	myVertexData = new VertexDataWrapper;
	myIndexBuffer = new IndexBufferWrapper;
	myIndexData = new IndexDataWrapper;

	myVertexData->myNrOfVertexes = vertices.Size();
	myVertexData->myStride = sizeof(VertexTypePosUV);
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
	myWindowSize = Engine::GetInstance()->GetWindowSize();

	mySize = aSize;
	myPosition = aPosition;
	myEffect = Engine::GetInstance()->GetEffect("Shaders/T_Sprite.json");
	myTexture = aShaderResource;
	//myEffect->SetAlbedo(aShaderResource);

	myVertexFormat.Init(2);
	myVertexFormat.Add(VertexLayoutPosUV[0]);
	myVertexFormat.Add(VertexLayoutPosUV[1]);

	CU::GrowingArray<VertexTypePosUV> vertices;
	CU::GrowingArray<int> indices;
	float halfWidth = mySize.x * 0.5f;
	float halfHeight = mySize.y * 0.5f;
	VertexTypePosUV v;
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

	myVertexBuffer = new VertexBufferWrapper;
	myVertexData = new VertexDataWrapper;
	myIndexBuffer = new IndexBufferWrapper;
	myIndexData = new IndexDataWrapper;

	myVertexData->myNrOfVertexes = vertices.Size();
	myVertexData->myStride = sizeof(VertexTypePosUV);
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
	Engine::GetAPI()->SetBlendState(eBlendStates::BLEND_FALSE);
	if (!myEffect)
		return;
	Engine::GetAPI()->SetSamplerState(eSamplerStates::LINEAR_CLAMP);

	SetMatrices(anOrientation, a2DCameraOrientation, anOrthogonalProjectionMatrix);

	ID3D11DeviceContext& context = *Engine::GetAPI()->GetContext();
	context.IASetInputLayout(myVertexLayout);
	context.IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNrOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
	context.IASetIndexBuffer(myIndexBuffer->myIndexBuffer, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);
	context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UpdateConstantBuffer();

	Engine::GetAPI()->SetVertexShader(myEffect->GetVertexShader()->m_Shader);
	context.VSSetConstantBuffers(0, 1, &myConstantBuffer);
	Engine::GetAPI()->SetPixelShader(myEffect->GetPixelShader()->m_Shader);
	ID3D11ShaderResourceView* srv = myTexture;
	context.PSSetShaderResources(0, 1, &srv);

	context.DrawIndexed(6, 0, 0);

	srv = nullptr;
	context.PSSetShaderResources(0, 1, &srv);

	Engine::GetAPI()->SetBlendState(eBlendStates::NO_BLEND);
}

Effect* CSpriteModel::GetEffect()
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
	myConstantStruct->scale = mySize;


	DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");

	D3D11_MAPPED_SUBRESOURCE msr;
	Engine::GetAPI()->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData != nullptr)
	{
		SSpriteConstantBuffer* ptr = (SSpriteConstantBuffer*)msr.pData;
		memcpy(ptr, &myConstantStruct->world.myMatrix[0], sizeof(SSpriteConstantBuffer));
	}

	Engine::GetAPI()->GetContext()->Unmap(myConstantBuffer, 0);
}

void CSpriteModel::InitiateVertexBuffer()
{
	HRESULT hr;

	hr = Engine::GetAPI()->GetDevice()->
		CreateInputLayout(&myVertexFormat[0]
			, myVertexFormat.Size()
			, myEffect->GetVertexShader()->compiledShader
			, myEffect->GetVertexShader()->shaderSize
			, &myVertexLayout);
	Engine::GetAPI()->SetDebugName(myVertexLayout, "SpriteModel Vertex Layout");
	Engine::GetAPI()->HandleErrors(hr, "Failed to create VertexLayout");

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = myVertexData->mySize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = static_cast<void*>(myVertexData->myVertexData);

	hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer);
	Engine::GetAPI()->HandleErrors(hr, "Failed to Create VertexBuffer!");

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
	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&indexDesc, &indexData, &myIndexBuffer->myIndexBuffer);
	Engine::GetAPI()->HandleErrors(hr, "Failed to Create IndexBuffer");

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

	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
	Engine::GetAPI()->SetDebugName(myConstantBuffer, "Font Constant Buffer");
	Engine::GetAPI()->HandleErrors(hr, "[Font] : Failed to Create Constant Buffer, ");
}

void CSpriteModel::ConvertToNormalSpace()
{
	myPosition.x /= myWindowSize.m_Width;
	myPosition.y /= myWindowSize.m_Height;
}

void CSpriteModel::SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix)
{
	myConstantStruct->world = anOrientation;
	myConstantStruct->invertedView = a2DCameraOrientation;
	myConstantStruct->projection = anOrthogonalProjectionMatrix;
}

