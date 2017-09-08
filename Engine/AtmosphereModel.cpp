#include "stdafx.h"
#include "AtmosphereModel.h"


AtmosphereModel::~AtmosphereModel()
{
	mySurfaces.DeleteAll();
	for (AtmosphereModel* children : myChildren)
	{
		children->CleanUp();
	}
	myChildren.DeleteAll();

	Engine::GetAPI()->ReleasePtr(myConstantBuffer);
	Engine::GetAPI()->ReleasePtr(m_VertexLayout);
}

void AtmosphereModel::Initiate(const std::string& filename)
{
	//m_Filename = cl::substr(filename, "/", false, 0);
	if ( m_IsRoot == false )
	{
		InitVertexBuffer();
		InitInputLayout();
		InitIndexBuffer();
		InitConstantBuffer();
	}

	for ( AtmosphereModel* child : myChildren )
	{
		child->Initiate(filename);
	}
}

void AtmosphereModel::CleanUp()
{
		
}

void AtmosphereModel::Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection)
{
	for (AtmosphereModel* child : myChildren)
	{
		child->Render(camera_orientation, camera_projection);
	}

	if (m_IsRoot)
		return;

	if (mySurfaces.Empty())
		return;

	SetupLayoutsAndBuffers();

	UpdateConstantBuffer(camera_orientation, camera_projection);

	m_Effect->Use();

	render_context.m_Context->VSSetConstantBuffers(0, 1, &myConstantBuffer);
	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);
	render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);

	m_Effect->Clear();
}

void AtmosphereModel::AddChild(AtmosphereModel* child)
{
	myChildren.Add(child);
}

void AtmosphereModel::SetOrientation(const CU::Matrix44f& orientation)
{
	myOrientation = orientation;
	for ( AtmosphereModel* child : myChildren )
	{
		child->SetOrientation(myOrientation);
	}
}
