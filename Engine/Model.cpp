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
	for (Model* children : m_Children)
	{
		children->CleanUp();
	}
	m_Children.DeleteAll();

	SAFE_RELEASE(m_ConstantBuffer);
	DL_ASSERT_EXP(!m_ConstantBuffer, "Failed to release constant buffer!");

	SAFE_RELEASE(m_VertexLayout);
	SAFE_RELEASE(m_InstanceInputLayout);
	SAFE_RELEASE(m_InstanceBuffer);
}


void Model::CleanUp()
{
	m_Surfaces.DeleteAll();
	for (Model* children : m_Children)
	{
		children->CleanUp();
	}
	m_Children.DeleteAll();

	SAFE_RELEASE(m_ConstantBuffer);
	DL_ASSERT_EXP(!m_ConstantBuffer, "Failed to release constant buffer!");

	SAFE_RELEASE(m_VertexLayout);
	SAFE_RELEASE(m_InstanceInputLayout);
	SAFE_RELEASE(m_InstanceBuffer);
}

void Model::Initiate(const std::string& filename)
{
	//m_Filename = cl::substr(filename, "/", false, 0);
	m_Orientations.Init(250);


// 	for (const D3D11_INPUT_ELEMENT_DESC& el : myVertexFormat) //This is the next thing to go
// 	{
// 		m_InputLayoutDesc.Add(el);
// 	}
//  
// 	D3D11_INPUT_ELEMENT_DESC instance_info[4] = {
// 
// 		{ "INSTANCE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0 , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
// 		{ "INSTANCE", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
// 		{ "INSTANCE", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
// 		{ "INSTANCE", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
// 	};


	// 
	// 	m_InputLayoutDesc.Add(instance_info[0]);
	// 	m_InputLayoutDesc.Add(instance_info[1]);
	// 	m_InputLayoutDesc.Add(instance_info[2]);
	// 	m_InputLayoutDesc.Add(instance_info[3]);


// 	if (m_IsRoot == false)
// 	{
// 		InitVertexBuffer();
// 		InitInputLayout();
// 		InitIndexBuffer();
// 		InitConstantBuffer();
// 		//InitInstanceBuffer();
// 	}

	for (Model* child : m_Children)
	{
		child->Initiate(filename);
	}
}

void Model::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const graphics::RenderContext& render_context)
{
	if (m_Filename.find("default_cube") != m_Filename.npos)
	{
		RenderCube(aCameraOrientation, aCameraProjection);
		return;
	}

#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Blue);
#endif
	for (Model* child : m_Children)
	{
		child->Render(aCameraOrientation, aCameraProjection);
	}

	if (!m_VertexLayout || m_IsRoot || m_Surfaces.Empty())
		return;

	SetupLayoutsAndBuffers(); //depending

	UpdateConstantBuffer(aCameraOrientation, aCameraProjection); //depending
	render_context.m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer); //depending

	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP); //depending on dx

	for (Surface* surface : m_Surfaces)
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

void Model::RenderCube(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
	
// 	render_context.m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
// 	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);

	m_Effect->Use();
#ifdef _PROFILE
	EASY_BLOCK("Model : DrawIndexedInstanced", profiler::colors::Amber100);
#endif
	//render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif
	m_Effect->Clear();
}

void Model::RenderInstanced(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Amber);
#endif
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
	
	//Try to define this list before hand? Pass it to the activate of the surface?
	ISamplerState* samplers[] = {
		render_context.GetAPI().GetSamplerState(graphics::MSAA_x16),
	};

	render_context.GetContext().PSSetSamplerState(0, ARRSIZE(samplers), samplers);
	
#ifdef _PROFILE
	EASY_BLOCK("Model : DrawIndexedInstanced", profiler::colors::Amber100);
#endif
	for (Surface* surface : m_Surfaces)
	{
		surface->Activate(render_context);
		render_context.GetContext().DrawIndexedInstanced(this);
		surface->Deactivate();
	}
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif

	RemoveOrientation(); 

}

void Model::ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	for (Model* child : m_Children)
	{
		child->ShadowRender(camera_orientation, camera_projection, render_context);
	}

	if (m_IsRoot || !m_VertexLayout || m_Surfaces.Empty())
		return;

	SetupLayoutsAndBuffers();

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);

	render_context.m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);

	render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);
}

void Model::ShadowRenderInstanced(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	for (Model* child : m_Children)
	{
		child->ShadowRenderInstanced(camera_orientation, camera_projection, render_context);
	}

	if (!m_InstanceInputLayout || m_IsRoot || m_Surfaces.Empty() || m_Orientations.Empty())
	{
		RemoveOrientation();
		return;
	}

	render_context.m_Context->IASetInputLayout(m_InstanceInputLayout);
	render_context.m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	u32 offsets[] = {
		m_VertexBuffer.myByteOffset,
		0
	};

	u32 strides[] = {
		m_VertexBuffer.myStride,
		sizeof(CU::Matrix44f)
	};


	IBuffer* buffers[] = {
		m_VertexBuffer.myVertexBuffer,
		m_InstanceBuffer
	};

	render_context.m_Context->IASetVertexBuffers(0, ARRAYSIZE(buffers), buffers, strides, offsets);
	render_context.m_Context->IASetIndexBuffer(m_IndexBuffer.myIndexBuffer, DXGI_FORMAT_R32_UINT, m_IndexBuffer.myByteOffset);

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);

	render_context.m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_CLAMP);

	render_context.m_Context->DrawIndexedInstanced(m_IndexData.myIndexCount, m_Orientations.Size(), 0, 0, 0);
}

void Model::SetPosition(const CU::Vector3f& aPosition)
{
	myOrientation.SetPosition(aPosition);
	for each (Model* child in m_Children)
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
	for (Model* child : m_Children)
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

	for (Model* child : m_Children)
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

	for (Model* child : m_Children)
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

	if (!m_ConstantBuffer)
		InitConstantBuffer();

	m_ConstantStruct.m_World = m_Orientations[0];
	m_ConstantStruct.m_InvertedView = CU::Math::Inverse(camera_orientation);
	m_ConstantStruct.m_Projection = camera_projection;

	graphics::IGraphicsContext& ctx = rc.GetContext();

	ctx.UpdateConstantBuffer(m_ConstantBuffer, &m_ConstantStruct, sizeof(m_ConstantStruct));
	ctx.UpdateConstantBuffer(m_InstanceWrapper.GetInstanceBuffer(), &m_Orientations[0], m_Orientations.Size() * sizeof(CU::Matrix44f));

	ctx.VSSetConstantBuffer(0, 1, m_ConstantBuffer);

}

void Model::AddChild(Model* aChild)
{
	m_Children.Add(aChild);
}

/*
void Model::InitInstanceBuffer()
{
	D3D11_BUFFER_DESC ibdesc;
	ZeroMemory(&ibdesc, sizeof(ibdesc));
	ibdesc.ByteWidth = sizeof(CU::Matrix44f) * m_InstanceCount;
	ibdesc.Usage = D3D11_USAGE_DYNAMIC;
	ibdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibdesc.MiscFlags = 0;
	ibdesc.StructureByteStride = 0;

	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&ibdesc, nullptr, &m_InstanceBuffer);
	if (FAILED(hr))
		DL_ASSERT("Failed to create instance buffer!");

	void* shader = myEffect->GetVertexShader()->compiledShader;
	s32 size = myEffect->GetVertexShader()->shaderSize;
	m_InstanceInputLayout = Engine::GetAPI()->CreateInputLayout(shader, size, &m_InputLayoutDesc[0], m_InputLayoutDesc.Size());
	Engine::GetAPI()->SetDebugName(m_InstanceInputLayout, "Model Instancing InputLayout");


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

	myConstantBuffer = Engine::GetAPI()->CreateBuffer(cbDesc);
	Engine::GetAPI()->SetDebugName(myConstantBuffer, "Model Constant Buffer : " + m_Filename);

	//HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);

	//Engine::GetAPI()->HandleErrors(hr, "[BaseModel] : Failed to Create Constant Buffer, ");
}*/

void Model::CreateCube()
{

	myVertexFormat.Init(ARRAYSIZE(DeferredBaseLayout));
	for (s32 i = 0; i < ARRAYSIZE(DeferredBaseLayout); i++)
	{
		myVertexFormat.Add(DeferredBaseLayout[i]);
	}

	CU::GrowingArray<VertexTypePosNormUV> vertices;
	vertices.Init(16);

#pragma region Vertex
	VertexTypePosNormUV temp;
	temp.myPosition = { -1.0f, 1.0f, -1.0f };
	temp.myNormal = { 0.0f, 1.0f, 0.0f };
	temp.myUV = { 0.0f, 0.0f };
	vertices.Add(temp);

	temp.myPosition = { 1.0f, 1.0f, -1.0f };
	temp.myNormal = { 0.0f, 1.0f, 0.0f };
	temp.myUV = { 1.0f, 0.0f };
	vertices.Add(temp);

	temp.myPosition = { 1.0f, 1.0f, 1.0f };
	temp.myNormal = { 0.0f, 1.0f, 0.0f };
	temp.myUV = { 1.0f, 1.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, 1.0f, 1.0f };
	temp.myNormal = { 0.0f, 1.0f, 0.0f };
	temp.myUV = { 0.0f, 1.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, -1.0f, -1.0f };
	temp.myNormal = { 0.0f, -1.0f, 0.0f };
	temp.myUV = { 0.0f, 0.0f };

	vertices.Add(temp);

	temp.myPosition = { 1.0f, -1.0f, -1.0f };
	temp.myNormal = { 0.0f, -1.0f, 0.0f };
	temp.myUV = { 1.0f, 0.0f };
	vertices.Add(temp);

	temp.myPosition = { 1.0f, -1.0f, 1.0f };
	temp.myNormal = { 0.0f, -1.0f, 0.0f };
	temp.myUV = { 1.0f, 1.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, -1.0f, 1.0f };
	temp.myNormal = { 0.0f, -1.0f, 0.0f };
	temp.myUV = { 0.0f, 1.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, -1.0f, 1.0f };
	temp.myNormal = { -1.0f, 0.0f, 0.0f };
	temp.myUV = { 0.0f, 0.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, -1.0f, -1.0f };
	temp.myNormal = { -1.0f, 0.0f, 0.0f };
	temp.myUV = { 1.0f, 0.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, 1.0f, -1.0f };
	temp.myNormal = { -1.0f, 0.0f, 0.0f };
	temp.myUV = { 1.0f, 1.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, 1.0f, 1.0f };
	temp.myNormal = { -1.0f, 0.0f, 0.0f };
	temp.myUV = { 0.0f, 1.0f };

	vertices.Add(temp);


	temp.myPosition = { 1.0f, -1.0f, 1.0f };
	temp.myNormal = { 1.0f, 0.0f, 0.0f };
	temp.myUV = { 0.0f, 0.0f };

	vertices.Add(temp);

	temp.myPosition = { 1.0f, -1.0f, -1.0f };
	temp.myNormal = { 1.0f, 0.0f, 0.0f };
	temp.myUV = { 1.0f, 0.0f };

	vertices.Add(temp);

	temp.myPosition = { 1.0f, 1.0f, -1.0f };
	temp.myNormal = { 1.0f, 0.0f, 0.0f };
	temp.myUV = { 1.0f, 1.0f };
	vertices.Add(temp);

	temp.myPosition = { 1.0f, 1.0f, 1.0f };
	temp.myNormal = { 1.0f, 0.0f, 0.0f };
	temp.myUV = { 0.0f, 1.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, -1.0f, -1.0f };
	temp.myNormal = { 0.0f, 0.0f, -1.0f };
	temp.myUV = { 0.0f, 0.0f };

	vertices.Add(temp);

	temp.myPosition = { 1.0f, -1.0f, -1.0f };
	temp.myNormal = { 0.0f, 0.0f, -1.0f };
	temp.myUV = { 1.0f, 0.0f };
	vertices.Add(temp);

	temp.myPosition = { 1.0f, 1.0f, -1.0f };
	temp.myNormal = { 0.0f, 0.0f, -1.0f };
	temp.myUV = { 1.0f, 1.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, 1.0f, -1.0f };
	temp.myNormal = { 0.0f, 0.0f, -1.0f };
	temp.myUV = { 0.0f, 1.0f };

	vertices.Add(temp);


	temp.myPosition = { -1.0f, -1.0f, 1.0f };
	temp.myNormal = { 0.0f, 0.0f, 1.0f };
	temp.myUV = { 0.0f, 0.0f };

	vertices.Add(temp);

	temp.myPosition = { 1.0f, -1.0f, 1.0f };
	temp.myNormal = { 0.0f, 0.0f, 1.0f };
	temp.myUV = { 1.0f, 0.0f };

	vertices.Add(temp);

	temp.myPosition = { 1.0f, 1.0f, 1.0f };
	temp.myNormal = { 0.0f, 0.0f, 1.0f };
	temp.myUV = { 1.0f, 1.0f };

	vertices.Add(temp);

	temp.myPosition = { -1.0f, 1.0f, 1.0f };
	temp.myNormal = { 0.0f, 0.0f, 1.0f };
	temp.myUV = { 0.0f, 1.0f };

	vertices.Add(temp);

#pragma endregion

	m_VertexData.myNrOfVertexes = vertices.Size();
	m_VertexData.myStride = sizeof(VertexTypePosNormUV);
	m_VertexData.mySize = m_VertexData.myNrOfVertexes*m_VertexData.myStride;
	//m_VertexData.myType = VertexTypePosNormUV;
	m_VertexData.myVertexData = new char[m_VertexData.mySize]();
	memcpy(m_VertexData.myVertexData, &vertices[0], m_VertexData.mySize);

	m_IndexData.myFormat = DXGI_FORMAT_R32_UINT;
	m_IndexData.myIndexCount = 6 * 6;
	m_IndexData.mySize = m_IndexData.myIndexCount * 4;


	CU::GrowingArray<int> indices;
	indices.Init(32);

#pragma region Indices

	indices.Add(3);
	indices.Add(1);
	indices.Add(0);

	indices.Add(2);
	indices.Add(1);
	indices.Add(3);

	indices.Add(6);
	indices.Add(4);
	indices.Add(5);

	indices.Add(6);
	indices.Add(7);
	indices.Add(4);

	indices.Add(11);
	indices.Add(9);
	indices.Add(8);

	indices.Add(10);
	indices.Add(9);
	indices.Add(11);

	indices.Add(14);
	indices.Add(12);
	indices.Add(13);

	indices.Add(15);
	indices.Add(12);
	indices.Add(14);

	indices.Add(19);
	indices.Add(17);
	indices.Add(16);

	indices.Add(18);
	indices.Add(17);
	indices.Add(19);

	indices.Add(22);
	indices.Add(20);
	indices.Add(21);

	indices.Add(23);
	indices.Add(20);
	indices.Add(22);

#pragma endregion

	m_IndexData.myIndexData = new char[m_IndexData.mySize]();
	memcpy(m_IndexData.myIndexData, &indices[0], m_IndexData.mySize);
	m_IsRoot = false;

	Initiate("default_cube");

	//InitVertexBuffer();
	//InitIndexBuffer();
}

