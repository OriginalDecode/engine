#include "stdafx.h"
#include "LightModel.h"
#include <Engine/IGraphicsContext.h>


LightModel::LightModel()
{
}

LightModel::~LightModel()
{
	m_Surfaces.DeleteAll();
	m_Children.DeleteAll();
}


void LightModel::Initiate(const std::string& filename)
{
	//m_Filename = cl::substr(filename, "/", false, 0);
	std::string dbg(filename.c_str());
	for ( LightModel* child : m_Children )
	{
		child->Initiate(filename);
	}
}

void LightModel::Render(const graphics::RenderContext& rc)
{
	for (LightModel* child : m_Children)
	{
		child->Render(rc);
	}

	if (m_IsRoot)
		return;

	//UpdateConstantBuffer(rc);
	//graphics::IGraphicsContext& ctx = rc.GetContext();
	//ISamplerState* pSampler = rc.GetEngine().GetActiveSampler();
	//ctx.PSSetSamplerState(0, 1, &pSampler);
	//ctx.SetBlendState(rc.GetAPI().GetBlendState(graphics::LIGHT_BLEND)); //alpha blend does not work
	//ctx.Draw(this, m_Effect);
}

void LightModel::AddChild(LightModel* aChild)
{
	m_Children.Add(aChild);
}

void LightModel::SetOrientation(const CU::Matrix44f& orientation)
{
	m_Orientation = orientation;
	for ( LightModel* child : m_Children )
	{
		child->SetOrientation(orientation);
	}
}
