#include "stdafx.h"
#include "LightModel.h"


bool LightModel::CleanUp()
{

	return true;
}

void LightModel::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const RenderContext& render_context)
{
	for (Model* child : myChildren)
	{
		child->Render(aCameraOrientation, aCameraProjection, render_context);
	}

	if (m_IsRoot)
		return;

	SetupLayoutsAndBuffers();

	myEffect->Activate();
	
	UpdateConstantBuffer(aCameraOrientation, aCameraProjection);
	
	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);
	render_context.m_Context->Draw(m_VertexData.myNrOfVertexes, 0);
}
