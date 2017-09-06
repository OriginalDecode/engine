#include "stdafx.h"
#include "LightModel.h"


LightModel::~LightModel()
{
	mySurfaces.DeleteAll();
	for (LightModel* child : myChildren)
	{
		child->CleanUp();
		delete child;
		child = nullptr;
	}

	SAFE_RELEASE(myConstantBuffer);
	DL_ASSERT_EXP(!myConstantBuffer, "Failed to release constant buffer!");

	SAFE_RELEASE(m_VertexLayout);
}

void LightModel::Initiate(const std::string& filename)
{
	m_Filename = cl::substr(filename, "/", false, 0);
	if ( m_IsRoot == false )
	{
		InitVertexBuffer();
		InitInputLayout();
		InitIndexBuffer();
		InitConstantBuffer();
	}

	for ( LightModel* child : myChildren )
	{
		child->Initiate(filename);
	}
}

void LightModel::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
{
	for (LightModel* child : myChildren)
	{
		child->Render(aCameraOrientation, aCameraProjection, render_context);
	}

	if (m_IsRoot)
		return;

	SetupLayoutsAndBuffers();

	//myEffect->Activate();
	
	UpdateConstantBuffer(aCameraOrientation, aCameraProjection, render_context);
	
	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);
	render_context.m_Context->Draw(m_VertexData.myNrOfVertexes, 0);
}

void LightModel::AddChild(LightModel* aChild)
{
	myChildren.Add(aChild);
}

void LightModel::SetOrientation(const CU::Matrix44f& orientation)
{
	myOrientation = orientation;
	for ( LightModel* child : myChildren )
	{
		child->SetOrientation(orientation);
	}
}
