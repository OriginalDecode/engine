#include "stdafx.h"
#include "LightModel.h"
#include <Engine/IGraphicsContext.h>


LightModel::~LightModel()
{
	m_Surfaces.DeleteAll();
	m_Children.DeleteAll();

	Engine::GetAPI()->ReleasePtr(m_ConstantBuffer);
	DL_ASSERT_EXP(!m_ConstantBuffer, "Failed to release constant buffer!");
}

void LightModel::Initiate(const std::string& filename)
{
	//m_Filename = cl::substr(filename, "/", false, 0);
	std::string dbg(filename.c_str());
	m_ConstantBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(cbVertex), dbg + " LightModel - ConstantBuffer");
	for ( LightModel* child : m_Children )
	{
		child->Initiate(filename);
	}
}

void LightModel::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const graphics::RenderContext& render_context)
{
	for (LightModel* child : m_Children)
	{
		child->Render(aCameraOrientation, aCameraProjection, render_context);
	}

	if (m_IsRoot)
		return;

	UpdateConstantBuffer(aCameraOrientation, aCameraProjection, render_context);
	render_context.GetContext().PSSetSamplerState(0, 1, render_context.GetEngine().GetActiveSampler());
	render_context.GetContext().Draw(this);
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
