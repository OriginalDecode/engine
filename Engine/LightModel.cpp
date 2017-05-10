#include "stdafx.h"
#include "LightModel.h"


void LightModel::CleanUp()
{
}

void LightModel::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const RenderContext& render_context)
{
	for (LightModel* child : myChildren)
	{
		child->Render(aCameraOrientation, aCameraProjection, render_context);
	}

	if (m_IsRoot)
		return;

	SetupLayoutsAndBuffers();

	myEffect->Activate();
	
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
