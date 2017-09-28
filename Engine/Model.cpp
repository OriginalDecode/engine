#include "stdafx.h"
#include "Model.h"

#include "Camera.h"
#include "DirectX11.h"
#include <DL_Debug.h>
#include "Engine.h"
#include "Surface.h"

#include <Engine/RenderContext.h>
#include <Engine/IGraphicsContext.h>

Model::~Model()
{
	m_Surfaces.DeleteAll();
	m_Children.DeleteAll();

	Engine::GetAPI()->ReleasePtr(m_ConstantBuffer);

}

void Model::Initiate(const std::string& filename)
{
	m_Orientations.Init(250);
	m_ConstantBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(VertexBaseStruct));
	for (Model* child : m_Children)
	{
		child->Initiate(filename);
	}
}

void Model::Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Blue);
	for (Model* child : m_Children)
	{
		child->Render(camera_orientation, camera_projection, render_context);
	}

	if (m_IsRoot || m_Surfaces.Empty())
		return;


	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
	render_context.GetContext().PSSetSamplerState(0, 1, render_context.GetEngine().GetCurrentSampler());

	PROFILE_BLOCK("Model : DrawIndexed", profiler::colors::Blue100);
	for (Surface* surface : m_Surfaces)
	{
		surface->Activate(render_context);
		render_context.GetContext().DrawIndexed(this);
		surface->Deactivate();
	}
	PROFILE_BLOCK_END;

}

void Model::RenderCube(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
// 	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
// 	
// // 	render_context.m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
// // 	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);
// 
// 	m_Effect->Use();
// 	PROFILE_BLOCK("Model : DrawIndexedInstanced", profiler::colors::Amber100);
// 	//render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);
// 	PROFILE_BLOCK_END;
// 	m_Effect->Clear();
}

void Model::RenderInstanced(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Amber);
	for (Model* child : m_Children)
	{
		child->RenderInstanced(camera_orientation, camera_projection, render_context);
	}

	if (m_IsRoot || m_Surfaces.Empty() || m_Orientations.Empty())
	{
		RemoveOrientation();
		return;
	}

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
	render_context.GetContext().PSSetSamplerState(0, 1, render_context.GetEngine().GetCurrentSampler());
	
	PROFILE_BLOCK("Model : DrawIndexedInstanced", profiler::colors::Amber100);
	for (Surface* surface : m_Surfaces)
	{
		surface->Activate(render_context);
		render_context.GetContext().DrawIndexedInstanced(this);
		surface->Deactivate();
	}
	PROFILE_BLOCK_END;

	RemoveOrientation(); 

}

void Model::ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Amber100);
	for (Model* child : m_Children)
	{
		child->ShadowRender(camera_orientation, camera_projection, render_context);
	}
 
	if (m_IsRoot)
		return;
 
	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
	render_context.GetContext().PSSetSamplerState(0, 1, render_context.GetEngine().GetCurrentSampler());
	render_context.GetContext().DrawIndexed(this);
}

void Model::ShadowRenderInstanced(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Amber100);
	for (Model* child : m_Children)
	{
		child->ShadowRenderInstanced(camera_orientation, camera_projection, render_context);
	}

	if (m_IsRoot || m_Orientations.Empty())
	{
		RemoveOrientation();
		return;
	}
	
	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
	render_context.GetContext().PSSetSamplerState(0, 1, render_context.GetEngine().GetCurrentSampler());

	render_context.GetContext().DrawIndexedInstanced(this);
}

void Model::SetPosition(const CU::Vector3f& aPosition)
{
	m_Orientation.SetPosition(aPosition);
	for each (Model* child in m_Children)
	{
		child->SetPosition(aPosition);
	}
}

CU::Matrix44f& Model::GetOrientation()
{
	return m_Orientation;
}

void Model::SetOrientation(CU::Matrix44f orientation)
{
	m_Orientation = orientation;
	for (Model* child : m_Children)
	{
		child->SetOrientation(m_Orientation);
	}
}

void Model::SetMaxPoint(CU::Vector3f max_point)
{
	m_MaxPoint = max_point;
}

void Model::SetMinPoint(CU::Vector3f min_point)
{
	m_MinPoint = min_point;
}

void Model::AddTexture(const std::string& path, Effect::TextureSlot slot)
{
	for (Model* pChild : m_Children)
	{
		pChild->AddTexture(path, slot);
	}

	for (Surface* pSurface : m_Surfaces)
	{
		pSurface->AddTexture(path, slot);
	}
}

void Model::AddOrientation(CU::Matrix44f orientation)
{
	DL_ASSERT_EXP(m_Orientations.Size() >= m_InstanceWrapper.GetInstanceCount(), "Too many instances");
	for (Model* child : m_Children)
	{
		child->AddOrientation(orientation);
	}
	m_Orientations.Add(orientation);
}

void Model::RemoveOrientation()
{
	for (Model* child : m_Children)
	{
		child->RemoveOrientation();
	}
	m_Orientations.RemoveAll();
}

void Model::UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& rc)
{
	if (m_IsRoot)
		return;

	m_ConstantStruct.m_World = m_Orientation;
	m_ConstantStruct.m_InvertedView = CU::Math::Inverse(camera_orientation);
	m_ConstantStruct.m_Projection = camera_projection;

	graphics::IGraphicsContext& ctx = rc.GetContext();

	ctx.UpdateConstantBuffer(m_ConstantBuffer, &m_ConstantStruct, sizeof(m_ConstantStruct));
	ctx.UpdateConstantBuffer(m_InstanceWrapper.GetInstanceBuffer(), &m_Orientation, m_Orientations.Size() * sizeof(CU::Matrix44f));

	ctx.VSSetConstantBuffer(0, 1, m_ConstantBuffer);

}

void Model::AddChild(Model* aChild)
{
	m_Children.Add(aChild);
}

void Model::CreateCube()
{
	assert(false&&"not implemented");
// 	myVertexFormat.Init(ARRAYSIZE(DeferredBaseLayout));
// 	for (s32 i = 0; i < ARRAYSIZE(DeferredBaseLayout); i++)
// 	{
// 		myVertexFormat.Add(DeferredBaseLayout[i]);
// 	}
// 
// 	CU::GrowingArray<VertexTypePosNormUV> vertices;
// 	vertices.Init(16);
// 
// #pragma region Vertex
// 	VertexTypePosNormUV temp;
// 	temp.myPosition = { -1.0f, 1.0f, -1.0f };
// 	temp.myNormal = { 0.0f, 1.0f, 0.0f };
// 	temp.myUV = { 0.0f, 0.0f };
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, 1.0f, -1.0f };
// 	temp.myNormal = { 0.0f, 1.0f, 0.0f };
// 	temp.myUV = { 1.0f, 0.0f };
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, 1.0f, 1.0f };
// 	temp.myNormal = { 0.0f, 1.0f, 0.0f };
// 	temp.myUV = { 1.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, 1.0f, 1.0f };
// 	temp.myNormal = { 0.0f, 1.0f, 0.0f };
// 	temp.myUV = { 0.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, -1.0f, -1.0f };
// 	temp.myNormal = { 0.0f, -1.0f, 0.0f };
// 	temp.myUV = { 0.0f, 0.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, -1.0f, -1.0f };
// 	temp.myNormal = { 0.0f, -1.0f, 0.0f };
// 	temp.myUV = { 1.0f, 0.0f };
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, -1.0f, 1.0f };
// 	temp.myNormal = { 0.0f, -1.0f, 0.0f };
// 	temp.myUV = { 1.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, -1.0f, 1.0f };
// 	temp.myNormal = { 0.0f, -1.0f, 0.0f };
// 	temp.myUV = { 0.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, -1.0f, 1.0f };
// 	temp.myNormal = { -1.0f, 0.0f, 0.0f };
// 	temp.myUV = { 0.0f, 0.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, -1.0f, -1.0f };
// 	temp.myNormal = { -1.0f, 0.0f, 0.0f };
// 	temp.myUV = { 1.0f, 0.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, 1.0f, -1.0f };
// 	temp.myNormal = { -1.0f, 0.0f, 0.0f };
// 	temp.myUV = { 1.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, 1.0f, 1.0f };
// 	temp.myNormal = { -1.0f, 0.0f, 0.0f };
// 	temp.myUV = { 0.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 
// 	temp.myPosition = { 1.0f, -1.0f, 1.0f };
// 	temp.myNormal = { 1.0f, 0.0f, 0.0f };
// 	temp.myUV = { 0.0f, 0.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, -1.0f, -1.0f };
// 	temp.myNormal = { 1.0f, 0.0f, 0.0f };
// 	temp.myUV = { 1.0f, 0.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, 1.0f, -1.0f };
// 	temp.myNormal = { 1.0f, 0.0f, 0.0f };
// 	temp.myUV = { 1.0f, 1.0f };
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, 1.0f, 1.0f };
// 	temp.myNormal = { 1.0f, 0.0f, 0.0f };
// 	temp.myUV = { 0.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, -1.0f, -1.0f };
// 	temp.myNormal = { 0.0f, 0.0f, -1.0f };
// 	temp.myUV = { 0.0f, 0.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, -1.0f, -1.0f };
// 	temp.myNormal = { 0.0f, 0.0f, -1.0f };
// 	temp.myUV = { 1.0f, 0.0f };
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, 1.0f, -1.0f };
// 	temp.myNormal = { 0.0f, 0.0f, -1.0f };
// 	temp.myUV = { 1.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, 1.0f, -1.0f };
// 	temp.myNormal = { 0.0f, 0.0f, -1.0f };
// 	temp.myUV = { 0.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 
// 	temp.myPosition = { -1.0f, -1.0f, 1.0f };
// 	temp.myNormal = { 0.0f, 0.0f, 1.0f };
// 	temp.myUV = { 0.0f, 0.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, -1.0f, 1.0f };
// 	temp.myNormal = { 0.0f, 0.0f, 1.0f };
// 	temp.myUV = { 1.0f, 0.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { 1.0f, 1.0f, 1.0f };
// 	temp.myNormal = { 0.0f, 0.0f, 1.0f };
// 	temp.myUV = { 1.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// 	temp.myPosition = { -1.0f, 1.0f, 1.0f };
// 	temp.myNormal = { 0.0f, 0.0f, 1.0f };
// 	temp.myUV = { 0.0f, 1.0f };
// 
// 	vertices.Add(temp);
// 
// #pragma endregion
// 
// 	m_VertexData.myNrOfVertexes = vertices.Size();
// 	m_VertexData.myStride = sizeof(VertexTypePosNormUV);
// 	m_VertexData.mySize = m_VertexData.myNrOfVertexes*m_VertexData.myStride;
// 	//m_VertexData.myType = VertexTypePosNormUV;
// 	m_VertexData.myVertexData = new char[m_VertexData.mySize]();
// 	memcpy(m_VertexData.myVertexData, &vertices[0], m_VertexData.mySize);
// 
// 	m_IndexData.myFormat = DXGI_FORMAT_R32_UINT;
// 	m_IndexData.myIndexCount = 6 * 6;
// 	m_IndexData.mySize = m_IndexData.myIndexCount * 4;
// 
// 
// 	CU::GrowingArray<int> indices;
// 	indices.Init(32);
// 
// #pragma region Indices
// 
// 	indices.Add(3);
// 	indices.Add(1);
// 	indices.Add(0);
// 
// 	indices.Add(2);
// 	indices.Add(1);
// 	indices.Add(3);
// 
// 	indices.Add(6);
// 	indices.Add(4);
// 	indices.Add(5);
// 
// 	indices.Add(6);
// 	indices.Add(7);
// 	indices.Add(4);
// 
// 	indices.Add(11);
// 	indices.Add(9);
// 	indices.Add(8);
// 
// 	indices.Add(10);
// 	indices.Add(9);
// 	indices.Add(11);
// 
// 	indices.Add(14);
// 	indices.Add(12);
// 	indices.Add(13);
// 
// 	indices.Add(15);
// 	indices.Add(12);
// 	indices.Add(14);
// 
// 	indices.Add(19);
// 	indices.Add(17);
// 	indices.Add(16);
// 
// 	indices.Add(18);
// 	indices.Add(17);
// 	indices.Add(19);
// 
// 	indices.Add(22);
// 	indices.Add(20);
// 	indices.Add(21);
// 
// 	indices.Add(23);
// 	indices.Add(20);
// 	indices.Add(22);
// 
// #pragma endregion
// 
// 	m_IndexData.myIndexData = new char[m_IndexData.mySize]();
// 	memcpy(m_IndexData.myIndexData, &indices[0], m_IndexData.mySize);
// 	m_IsRoot = false;
// 
// 	Initiate("default_cube");

	//InitVertexBuffer();
	//InitIndexBuffer();
}
