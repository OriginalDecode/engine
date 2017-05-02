#include "stdafx.h"
#include "AtmosphereModel.h"


bool AtmosphereModel::CleanUp()
{
	__super::CleanUp();
	return true;
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
