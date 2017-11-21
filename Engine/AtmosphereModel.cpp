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
	//m_ConstantBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(cbVertex), "AtmosphereModel ConstantBuffer");
	for ( AtmosphereModel* child : m_Children )
	{
		child->Initiate(filename);
	}
}

void AtmosphereModel::Render(const graphics::RenderContext& rc)
{
	for (AtmosphereModel* child : m_Children)
	{
		child->Render(rc);
	}

	if (m_IsRoot)
		return;

	UpdateConstantBuffer(rc);

	auto& ctx = rc.GetContext();
	ctx.PSSetSamplerState(0, 1, Engine::GetInstance()->GetActiveSampler());
	ctx.DrawIndexed(this, m_Effect);
}

void AtmosphereModel::AddChild(AtmosphereModel* child)
{
	m_Children.Add(child);
}

void AtmosphereModel::SetOrientation(const CU::Matrix44f& orientation)
{
	m_Orientation = orientation;
	for ( AtmosphereModel* child : m_Children )
	{
		child->SetOrientation(m_Orientation);
	}
}
