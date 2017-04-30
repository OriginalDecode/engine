#include "stdafx.h"
#include "Model.h"

#include "Camera.h"
#include "DirectX11.h"
#include <DL_Debug.h>
#include "Engine.h"
#include "Surface.h"

void CModel::CleanUp()
{
	mySurfaces.DeleteAll();
	
	for (CModel* children : myChildren)
	{
		children->CleanUp();
	}

	myChildren.DeleteAll();

	SAFE_RELEASE(myConstantBuffer);
	DL_ASSERT_EXP(!myConstantBuffer, "Failed to release constant buffer!");
		
	SAFE_RELEASE(m_VertexLayout);
	DL_ASSERT_EXP(!m_VertexLayout, "Failed to release vertex layout");
}

void CModel::Initiate(const std::string& filename)
{
	m_Filename = CL::substr(filename, "/", false, 0);
	if (m_IsRoot == false)
	{
		InitVertexBuffer();
		InitIndexBuffer();
		InitConstantBuffer();
	}

	for (CModel* child : myChildren)
	{
		child->Initiate(filename);
	}

}

void CModel::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, bool render_shadows)
{
	for (CModel* child : myChildren)
	{
		child->Render(aCameraOrientation, aCameraProjection, render_shadows);
	}

	if (m_IsRoot)
		return;
	if (mySurfaces.Empty())
		return;
	SetupLayoutsAndBuffers();

	IDevContext* ctx = Engine::GetAPI()->GetContext();
	UpdateConstantBuffer(aCameraOrientation, aCameraProjection);
	ctx->VSSetConstantBuffers(0, 1, &myConstantBuffer);

	for (s32 i = 0; i < mySurfaces.Size(); i++)
	{
		Engine::GetAPI()->SetSamplerState(eSamplerStates::LINEAR_WRAP);

		myEffect->Activate();
		mySurfaces[i]->Activate(); 
		ctx->DrawIndexed(mySurfaces[i]->GetIndexCount(), 0, 0);
		mySurfaces[i]->Deactivate();
	}
}

void CModel::ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection)
{
	for (CModel* child : myChildren)
	{
		child->ShadowRender(camera_orientation, camera_orientation);
	}

	if (m_IsRoot)
		return;

	SetupLayoutsAndBuffers();
	IDevContext* ctx = Engine::GetAPI()->GetContext();
	UpdateConstantBuffer(camera_orientation, camera_projection);
	ctx->VSSetConstantBuffers(0, 1, &myConstantBuffer);
	ctx->DrawIndexed(m_IndexData.myIndexCount, 0, 0);

}

void CModel::SetPosition(const CU::Vector3f& aPosition)
{
	myOrientation.SetPosition(aPosition);
	for each (CModel* child in myChildren)
	{
		child->SetPosition(aPosition);
	}
}

CU::Matrix44f& CModel::GetOrientation()
{
	return myOrientation;
}

void CModel::SetOrientation(CU::Matrix44f orientation)
{
	myOrientation = orientation;
	for (CModel* child : myChildren)
	{
		child->SetOrientation(myOrientation);
	}
}

void CModel::SetMaxPoint(CU::Vector3f max_point)
{
	m_MaxPoint = max_point;
}

void CModel::SetMinPoint(CU::Vector3f min_point)
{
	m_MinPoint = min_point;
}

std::vector<float> CModel::GetVertices()
{
	std::vector<float> to_return;

	for (CModel* child : myChildren)
	{
		std::vector<float> child_verts = child->GetVertices();
		for (const float& vert : child_verts)
		{
			to_return.push_back(vert);
		}
	}

	for (const SVertexTypePosCol& vert : myVertices)
	{
		to_return.push_back(vert.myPosition.x);
		to_return.push_back(vert.myPosition.y);
		to_return.push_back(vert.myPosition.z);
	}
	return to_return;
}

std::vector<s32> CModel::GetIndices()
{
	std::vector<s32> to_return;

	for (CModel* child : myChildren)
	{
		std::vector<s32> child_verts = child->GetIndices();
		for (const s32& indice : child_verts)
		{
			to_return.push_back(indice);
		}
	}


	for (s32 index : m_Indices)
	{
		to_return.push_back(index);
	}
	return to_return;
}


void CModel::SetupLayoutsAndBuffers()
{
	IDevContext* ctx = Engine::GetAPI()->GetContext();
	ctx->IASetInputLayout(m_VertexLayout);
	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ctx->IASetVertexBuffers(0, 1, &m_VertexBuffer.myVertexBuffer, &m_VertexBuffer.myStride, &m_VertexBuffer.myByteOffset);
	ctx->IASetIndexBuffer(m_IndexBuffer.myIndexBuffer, DXGI_FORMAT_R32_UINT, m_IndexBuffer.myByteOffset);
}

void CModel::UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
{
	if (m_IsRoot == false)
	{
		if (!myConstantBuffer)
		{
			InitConstantBuffer();
		}

		m_ConstantStruct.m_World = myOrientation;
		m_ConstantStruct.m_InvertedView = CU::Math::Inverse(aCameraOrientation);
		m_ConstantStruct.m_Projection = aCameraProjection;

		Engine::GetAPI()->UpdateConstantBuffer(myConstantBuffer, &m_ConstantStruct);

	}
}

void CModel::InitVertexBuffer()
{
	DirectX11* api = Engine::GetAPI();
	void* shader = myEffect->GetVertexShader()->compiledShader;
	s32 size = myEffect->GetVertexShader()->shaderSize;
	m_VertexLayout = api->CreateInputLayout(shader, size, &myVertexFormat[0], myVertexFormat.Size());

#ifdef _DEBUG
	api->SetDebugName(m_VertexLayout, "Model Vertex Layout : " + m_Filename);
#endif
	m_VertexBuffer.myVertexBuffer = api->CreateBuffer(m_VertexData.mySize, m_VertexData.myVertexData);
#ifdef _DEBUG

	api->SetDebugName(m_VertexBuffer.myVertexBuffer, "Model Vertex Buffer " + m_Filename);
#endif

	m_VertexBuffer.myStride = m_VertexData.myStride;
	m_VertexBuffer.myByteOffset = 0;
	m_VertexBuffer.myStartSlot = 0;
	m_VertexBuffer.myNrOfBuffers = 1;
}

void CModel::InitIndexBuffer()
{
	DirectX11* api = Engine::GetAPI();
	m_IndexBuffer.myIndexBuffer = api->CreateBuffer(m_IndexData.mySize, m_IndexData.myIndexData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER);

	api->SetDebugName(m_IndexBuffer.myIndexBuffer, "Model Index Buffer " + m_Filename);
	m_IndexBuffer.myIndexBufferFormat = m_IndexData.myFormat;
	m_IndexBuffer.myByteOffset = 0;
}

void CModel::AddChild(CModel* aChild)
{
	myChildren.Add(aChild);
}

void CModel::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(VertexBaseStruct);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	DirectX11* api = Engine::GetAPI();
	HRESULT hr = api->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);

	api->SetDebugName(myConstantBuffer, "Model Constant Buffer : " + m_Filename);
	api->HandleErrors(hr, "[BaseModel] : Failed to Create Constant Buffer, ");
}
