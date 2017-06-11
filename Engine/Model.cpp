#include "stdafx.h"
#include "Model.h"

#include "Camera.h"
#include "DirectX11.h"
#include <DL_Debug.h>
#include "Engine.h"
#include "Surface.h"

void Model::CleanUp()
{
	mySurfaces.DeleteAll();
	for (Model* children : myChildren)
	{
		children->CleanUp();
	}
	myChildren.DeleteAll();

	SAFE_RELEASE(myConstantBuffer);
	DL_ASSERT_EXP(!myConstantBuffer, "Failed to release constant buffer!");
		
	SAFE_RELEASE(m_VertexLayout);
}

void Model::Initiate(const std::string& filename)
{
	m_Filename = CL::substr(filename, "/", false, 0);
	if (m_IsRoot == false)
	{
		InitVertexBuffer();
		InitIndexBuffer();
		InitConstantBuffer();
	}

	for (Model* child : myChildren)
	{
		child->Initiate(filename);
	}
}

void Model::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const RenderContext& render_context)
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Blue);
#endif
	for (Model* child : myChildren)
	{
		child->Render(aCameraOrientation, aCameraProjection, render_context);
	}

	if (!m_VertexLayout || m_IsRoot || mySurfaces.Empty())
		return;

	SetupLayoutsAndBuffers(); //depending

	UpdateConstantBuffer(aCameraOrientation, aCameraProjection, render_context); //depending
	render_context.m_Context->VSSetConstantBuffers(0, 1, &myConstantBuffer); //depending

	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP); //depending on dx

	for (Surface* surface : mySurfaces)
	{
		surface->Activate(render_context);
#ifdef _PROFILE
		EASY_BLOCK("Model : DrawIndexed", profiler::colors::Blue100);
#endif
		render_context.m_Context->DrawIndexed(surface->GetIndexCount(), 0, 0); //depending on dx
#ifdef _PROFILE
		EASY_END_BLOCK;
#endif
		surface->Deactivate();
	}

}

void Model::RenderInstanced(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context)
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Green);
#endif
	for (Model* child : myChildren)
	{
		child->Render(camera_orientation, camera_projection, render_context);
	}

	if (!m_VertexLayout || m_IsRoot || mySurfaces.Empty())
		return;

	render_context.m_Context->IASetInputLayout(m_VertexLayout);
	render_context.m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	render_context.m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.myVertexBuffer, &m_VertexBuffer.myStride, &m_VertexBuffer.myByteOffset);// change this
	render_context.m_Context->IASetIndexBuffer(m_IndexBuffer.myIndexBuffer, DXGI_FORMAT_R32_UINT, m_IndexBuffer.myByteOffset);

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context); 
	render_context.m_Context->VSSetConstantBuffers(0, 1, &myConstantBuffer); 

	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP); 
	for (Surface* surface : mySurfaces)
	{
		surface->Activate(render_context);
		//render_context.m_Context->DrawIndexed(surface->GetIndexCount(), 0, 0); //depending on dx
#ifdef _PROFILE
		EASY_BLOCK("Model : DrawIndexedInstanced", profiler::colors::Green100);
#endif
		render_context.m_Context->DrawIndexedInstanced(surface->GetIndexCount(), 250, 0, surface->GetStartVertex(), 0);
#ifdef _PROFILE
		EASY_END_BLOCK;
#endif
		surface->Deactivate();
	}
}

void Model::ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context)
{
	for (Model* child : myChildren)
	{
		child->ShadowRender(camera_orientation, camera_projection,render_context);
	}

	if (m_IsRoot)
		return;

	if ( !m_VertexLayout )
		return;

	if (mySurfaces.Empty())
		return;

	SetupLayoutsAndBuffers();

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);

	render_context.m_Context->VSSetConstantBuffers(0, 1, &myConstantBuffer);
	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);
	
	render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);
}

void Model::SetPosition(const CU::Vector3f& aPosition)
{
	myOrientation.SetPosition(aPosition);
	for each (Model* child in myChildren)
	{
		child->SetPosition(aPosition);
	}
}

CU::Matrix44f& Model::GetOrientation()
{
	return myOrientation;
}

void Model::SetOrientation(CU::Matrix44f orientation)
{
	myOrientation = orientation;
	for (Model* child : myChildren)
	{
		child->SetOrientation(myOrientation);
	}
}

void Model::SetWHD(CU::Vector3f whd)
{
	m_WHD = whd;
}

void Model::SetMaxPoint(CU::Vector3f max_point)
{
	m_MaxPoint = max_point;
}

void Model::SetMinPoint(CU::Vector3f min_point)
{
	m_MinPoint = min_point;
}

std::vector<float> Model::GetVertices()
{
	std::vector<float> to_return;

	for (Model* child : myChildren)
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

std::vector<s32> Model::GetIndices()
{
	std::vector<s32> to_return;

	for (Model* child : myChildren)
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


void Model::AddOrientation(CU::Matrix44f orientation)
{
	m_Orientations.Add(orientation);
}

void Model::UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const RenderContext& render_context)
{
	if ( m_IsRoot )
		return;

	if (!myConstantBuffer)
	{
		InitConstantBuffer();
	}

	m_ConstantStruct.m_World = myOrientation;
	m_ConstantStruct.m_InvertedView = CU::Math::Inverse(aCameraOrientation);
	m_ConstantStruct.m_Projection = aCameraProjection;

	render_context.m_API->UpdateConstantBuffer(myConstantBuffer, &m_ConstantStruct);
}

void Model::AddChild(Model* aChild)
{
	myChildren.Add(aChild);
}

void Model::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(VertexBaseStruct);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);

	Engine::GetAPI()->SetDebugName(myConstantBuffer, "Model Constant Buffer : " + m_Filename);
	Engine::GetAPI()->HandleErrors(hr, "[BaseModel] : Failed to Create Constant Buffer, ");
}
