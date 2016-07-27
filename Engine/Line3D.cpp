#include "stdafx.h"
#include "Line3D.h"
#include <DL_Debug.h>
#include "VertexStructs.h"

CLine3D::CLine3D()
	: myFirstPoint({ 0, 0, 0, 1 }, { 1, 1, 1, 1 })
	, mySecondPoint({ 0, 0, 0, 1 }, { 1, 1, 1, 1 })
{
}

CLine3D::~CLine3D()
{
	SAFE_RELEASE(myVertexLayout);
	SAFE_RELEASE(myConstantBuffer);
	SAFE_DELETE(myConstantStruct);
	SAFE_DELETE(myVertexBuffer);
}

void CLine3D::Initiate(int aLineAmount /*= 256*/)
{
	myLineAmount = aLineAmount;
	myAPI = Snowblind::CEngine::GetDirectX();
	myEffect =  Snowblind::CEngine::GetInstance()->GetEffect("Data/Shaders/T_Line3D.json");
	CreateVertexBuffer();
	CreateConstantBuffer();

}

void CLine3D::Update(const SLinePoint& firstPoint, const SLinePoint& secondPoint)
{
	myFirstPoint = firstPoint;
	mySecondPoint = secondPoint;
	myVertices.RemoveAll();
	myVertices.Add(myFirstPoint);
	myVertices.Add(mySecondPoint);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	myAPI->GetContext()->Map(myVertexBuffer->myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (mappedResource.pData != nullptr)
	{
		SLinePoint* data = (SLinePoint*)mappedResource.pData;

		bool isSafe = sizeof(SLinePoint) == sizeof(myVertices[0]);
		DL_ASSERT_EXP(isSafe, "[Line3DRenderer](UpdateVertexBuffer) : Not safe to copy.");
		memcpy(data, &myVertices[0], sizeof(SLinePoint) * myVertices.Size());
	}
	myAPI->GetContext()->Unmap(myVertexBuffer->myVertexBuffer, 0);

}

void CLine3D::Render(CU::Matrix44f& prevOrientation, CU::Matrix44f& projection)
{
	ID3D11DeviceContext* context = myAPI->GetContext();

	context->IASetInputLayout(myVertexLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	context->IASetVertexBuffers(0, 1, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	myAPI->SetVertexShader(myEffect->GetVertexShader() ? myEffect->GetVertexShader()->vertexShader : nullptr);
	myAPI->SetPixelShader(myEffect->GetPixelShader() ? myEffect->GetPixelShader()->pixelShader : nullptr);

	ID3D11ShaderResourceView* srv[1];
	srv[0] = nullptr;
	context->PSSetShaderResources(0, 1, &srv[0]);

	SetMatrices(prevOrientation, projection);
	context->VSSetConstantBuffers(0, 1, &myConstantBuffer);

	context->Draw(myVertices.Size(), 0);

}

void CLine3D::AddLine(const SLine& aLine)
{

}

void CLine3D::AddCube(const CU::Vector3f& min, const CU::Vector3f& max)
{
	float depth = max.z - min.z;
	float width = max.x - min.x;
	float height = max.y - min.y;

	CU::Vector3f point;
	CU::Vector3f nextPoint;

	SLinePoint p1;
	SLinePoint p2;

	point.x = min.x;
	point.y = min.y;
	point.z = min.z;

	point.x -= (width * 0.5f);
	point.y -= (height * 0.5f);
	point.z -= (depth * 0.5f);

	nextPoint = point;
	nextPoint.x += width;

	p1.position = point;
	p1.color = CU::Vector4f(1, 0, 1, 1);

	p2.position = nextPoint;
	p2.color = CU::Vector4f(1, 1, 1, 1);

	myVertices.Add(p2);
	myVertices.Add(p2);

	point = nextPoint;
	point.z += depth;

	nextPoint = point;
	nextPoint.x -= width;

	p1.position = point;
	p1.color = CU::Vector4f(0, 1, 1, 1);

	p2.position = nextPoint;
	p2.color = CU::Vector4f(0, 0, 1, 1);

	myVertices.Add(p2);
	myVertices.Add(p2);

	point = nextPoint;
	point.y += height;

	nextPoint = point;
	nextPoint.z -= depth;

	p1.position = point;
	p1.color = CU::Vector4f(1.f, 150.f/255.f, 0, 1);

	p2.position = nextPoint;
	p2.color = CU::Vector4f(1, 0, 0, 1);

	myVertices.Add(p2);
	myVertices.Add(p2);

	point = nextPoint;
	point.x += width;

	nextPoint = point;
	nextPoint.z += depth;


	p1.position = point;
	p1.color = CU::Vector4f(1.f, 1.f, 0, 1);

	p2.position = nextPoint;
	p2.color = CU::Vector4f(0, 1, 0, 1);

	myVertices.Add(p2);
	myVertices.Add(p2);

}

void CLine3D::CreateConstantBuffer()
{
	myConstantStruct = new SVertexBaseStruct;

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(SVertexBaseStruct);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = myAPI->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
	myAPI->SetDebugName(myConstantBuffer, "Line3D Constant Buffer");
	myAPI->HandleErrors(hr, "[Line3D] : Failed to Create Constant Buffer, ");
}

void CLine3D::CreateVertexBuffer()
{

	HRESULT hr;
	void* shader = myEffect->GetVertexShader()->compiledShader;
	int size = myEffect->GetVertexShader()->shaderSize;

	const D3D11_INPUT_ELEMENT_DESC Line3DRendererLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT asize = ARRAYSIZE(Line3DRendererLayout);

	hr = myAPI->GetDevice()->
		CreateInputLayout(&Line3DRendererLayout[0], asize
			, shader
			, size
			, &myVertexLayout);

	myAPI->SetDebugName(myVertexLayout, "Line3D Vertex Layout");
	myAPI->HandleErrors(hr, "Failed to create VertexLayout");

	myVertexBuffer = new Snowblind::SVertexBufferWrapper();
	myVertexBuffer->myStride = sizeof(SLinePoint);
	myVertexBuffer->myByteOffset = 0;
	myVertexBuffer->myStartSlot = 0;
	myVertexBuffer->myNrOfBuffers = 1;

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	if (myVertexBuffer->myVertexBuffer)
	{
		myVertexBuffer->myVertexBuffer->Release();
	}

	vertexBufferDesc.ByteWidth = sizeof(SLinePoint) * myLineAmount;



	hr = myAPI->GetDevice()->CreateBuffer(&vertexBufferDesc, nullptr, &myVertexBuffer->myVertexBuffer);
	myAPI->SetDebugName(myVertexBuffer->myVertexBuffer, "Line3D : Vertex Buffer");
	myAPI->HandleErrors(hr, "Failed to Create VertexBuffer!");

}

void CLine3D::SetMatrices(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection)
{
	DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");
	myOrientation.SetPosition(CU::Vector3f(1, 1, 1));
	myConstantStruct->world = myOrientation;
	myConstantStruct->invertedView = CU::Math::Inverse(aCameraOrientation);
	myConstantStruct->projection = aCameraProjection;

	D3D11_MAPPED_SUBRESOURCE msr;
	Snowblind::CEngine::GetDirectX()->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData != nullptr)
	{
		SVertexBaseStruct* ptr = (SVertexBaseStruct*)msr.pData;
		memcpy(ptr, &myConstantStruct->world.myMatrix[0], sizeof(SVertexBaseStruct));
	}

	Snowblind::CEngine::GetDirectX()->GetContext()->Unmap(myConstantBuffer, 0);
}

SLinePoint::SLinePoint(const CU::Vector4f& pos, const CU::Vector4f& col)
	: position(pos)
	, color(col)
{
}

SLinePoint::SLinePoint()
{
}
