#include "stdafx.h"
#include "LightModel.h"
#include <Engine/IGraphicsContext.h>


LightModel::LightModel()
{
	m_ConstantBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(cbVertex), "LightModel - ConstantBuffer");
}

LightModel::~LightModel()
{
	m_Surfaces.DeleteAll();
	m_Children.DeleteAll();

	Engine::GetAPI()->ReleasePtr(m_ConstantBuffer);
	m_ConstantBuffer = nullptr;
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

	UpdateConstantBuffer(rc);
	rc.GetContext().PSSetSamplerState(0, 1, rc.GetEngine().GetActiveSampler());
	rc.GetContext().Draw(this);
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
