#include "stdafx.h"
#include "LightModel.h"
#include <Engine/IGraphicsContext.h>


LightModel::~LightModel()
{
	m_Surfaces.DeleteAll();
	for (LightModel* child : m_Children)
	{
		child->CleanUp();
		delete child;
		child = nullptr;
	}

	SAFE_RELEASE(m_ConstantBuffer);
	DL_ASSERT_EXP(!m_ConstantBuffer, "Failed to release constant buffer!");

	SAFE_RELEASE(m_VertexLayout);
}

void LightModel::Initiate(const std::string& filename)
{
	//m_Filename = cl::substr(filename, "/", false, 0);
	if ( !m_IsRoot  )
	{
		InitVertexBuffer();
		InitInputLayout();
		InitIndexBuffer();
		InitConstantBuffer();
	}

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

	//SetupLayoutsAndBuffers();

	//myEffect->Activate();
	
	UpdateConstantBuffer(aCameraOrientation, aCameraProjection, render_context);
	
	//render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);


	render_context.GetContext().Draw(this);

	//render_context.m_Context->Draw(m_VertexData.myNrOfVertexes, 0);
}

void LightModel::AddChild(LightModel* aChild)
{
	m_Children.Add(aChild);
}

void LightModel::SetOrientation(const CU::Matrix44f& orientation)
{
	m_Orientations[0] = orientation;
	for ( LightModel* child : m_Children )
	{
		child->SetOrientation(orientation);
	}
}
