#include "stdafx.h"
#include "AtmosphereModel.h"
#include <Engine/IGraphicsContext.h>


AtmosphereModel::~AtmosphereModel()
{
	m_Surfaces.DeleteAll();
	m_Children.DeleteAll();
	Engine::GetAPI()->ReleasePtr(m_ConstantBuffer);
}

void AtmosphereModel::Initiate(const std::string& filename)
{
	m_ConstantBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(cbVertex));
	for ( AtmosphereModel* child : m_Children )
	{
		child->Initiate(filename);
	}
}

void AtmosphereModel::Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	for (AtmosphereModel* child : m_Children)
	{
		child->Render(camera_orientation, camera_projection, render_context);
	}

	if (m_IsRoot || m_Surfaces.Empty())
		return;

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);

	auto& ctx = render_context.GetContext();
	ctx.PSSetSamplerState(0, 1, Engine::GetInstance()->GetActiveSampler());
	ctx.DrawIndexed(this, m_Effect);
}

void AtmosphereModel::AddChild(AtmosphereModel* child)
{
	m_Children.Add(child);
}

void AtmosphereModel::SetOrientation(const CU::Matrix44f& orientation)
{
	m_Orientations[0] = orientation;
	for ( AtmosphereModel* child : m_Children )
	{
		child->SetOrientation(m_Orientations[0]);
	}
}
