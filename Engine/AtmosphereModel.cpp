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

	SAFE_RELEASE(myConstantBuffer);
	DL_ASSERT_EXP(!myConstantBuffer, "Failed to release constant buffer!");

	SAFE_RELEASE(m_VertexLayout);
}

void AtmosphereModel::Initiate(const std::string& filename)
{
	m_Filename = CL::substr(filename, "/", false, 0);
	if ( m_IsRoot == false )
	{
		InitVertexBuffer();
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

void AtmosphereModel::Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context)
{
	for (AtmosphereModel* child : myChildren)
	{
		child->Render(camera_orientation, camera_projection, render_context);
	}

	if (m_IsRoot)
		return;

	if (mySurfaces.Empty())
		return;

	SetupLayoutsAndBuffers();
	myEffect->Activate();

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);

	render_context.m_Context->VSSetConstantBuffers(0, 1, &myConstantBuffer);



	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);
	render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);

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
