#include "stdafx.h"
#include "MoveArrowModel.h"
#include "Texture.h"
#include "Engine.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "Surface.h"
#include "Model.h"

MoveArrowModel::MoveArrowModel()
{
}

void MoveArrowModel::Initiate()
{

	m_Forward.m_Key = Engine::GetInstance()->LoadModel("Data/Model/blue_arrow.fbx", "Data/Shaders/T_Movement_Arrow.json");

	m_Forward.m_DirColor = Engine::GetInstance()->GetTexture("Data/Textures/blue.dds");

	m_Forward.m_Arrow = Engine::GetInstance()->GetModel(m_Forward.m_Key);
	CU::GrowingArray<CModel*> children = m_Forward.m_Arrow->GetChildModels();
	for (CModel* child : children)
	{
		CU::GrowingArray<CSurface*>& surfaces = child->GetSurfaces();
		for (CSurface* surface : surfaces)
		{
			surface->ClearTextures();
			surface->AddTexture(m_Forward.m_DirColor->GetShaderView());
		}

	}


	m_Forward.m_Orientation = CU::Matrix44f::CreateRotateAroundX(CL::DegreeToRad(90.f) * 1) * m_Forward.m_Orientation;


	m_Up.m_Key = Engine::GetInstance()->LoadModel("Data/Model/green_arrow.fbx", "Data/Shaders/T_Movement_Arrow.json");

	m_Up.m_DirColor = Engine::GetInstance()->GetTexture("Data/Textures/green.dds");

	m_Up.m_Arrow = Engine::GetInstance()->GetModel(m_Up.m_Key);

	children = m_Up.m_Arrow->GetChildModels();
	for (CModel* child : children)
	{
		CU::GrowingArray<CSurface*>& surfaces = child->GetSurfaces();
		for (CSurface* surface : surfaces)
		{
			surface->ClearTextures();
			surface->AddTexture(m_Up.m_DirColor->GetShaderView());
		}

	}

	m_Right.m_Key = Engine::GetInstance()->LoadModel("Data/Model/red_arrow.fbx", "Data/Shaders/T_Movement_Arrow.json");

	m_Right.m_DirColor = Engine::GetInstance()->GetTexture("Data/Textures/red.dds");

	m_Right.m_Arrow = Engine::GetInstance()->GetModel(m_Right.m_Key);
	children = m_Right.m_Arrow->GetChildModels();
	for (CModel* child : children)
	{
		CU::GrowingArray<CSurface*>& surfaces = child->GetSurfaces();
		for (CSurface* surface : surfaces)
		{
			surface->ClearTextures();
			surface->AddTexture(m_Right.m_DirColor->GetShaderView());
		}

	}
	m_Right.m_Orientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(90.f) * -1) * m_Right.m_Orientation;

	m_Right.m_Arrow->SetOrientation(m_Right.m_Orientation);
	m_Forward.m_Arrow->SetOrientation(m_Forward.m_Orientation);
	m_Up.m_Arrow->SetOrientation(m_Up.m_Orientation);


}

void MoveArrowModel::CleanUp()
{

}

void MoveArrowModel::Render()
{
	
	CU::Vector4f camera_position = Engine::GetInstance()->GetCamera()->GetPosition();
	float distance = CU::Math::Length(m_Orientation.GetTranslation() - camera_position) * 0.5f;
	CU::Vector4f scale = CU::Vector4f(distance, distance, distance, 1);
	//CU::Matrix44f scale_matrix = CU::Matrix44f::CreateScaleMatrix(scale);
	CU::Matrix44f t = m_Right.m_Orientation;
	t = CU::Matrix44f::CreateScaleMatrix(scale) * t;
	Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(RenderCommand(eType::MODEL_NO_DEFERRED, m_Right.m_Key, t));

	//__________________________
	
	t = m_Up.m_Orientation;
	t = CU::Matrix44f::CreateScaleMatrix(scale) * t;
	Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(RenderCommand(eType::MODEL_NO_DEFERRED, m_Up.m_Key, t));

	//__________________________
	
	t = m_Forward.m_Orientation;
	t = CU::Matrix44f::CreateScaleMatrix(scale) * t;
	Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(RenderCommand(eType::MODEL_NO_DEFERRED, m_Forward.m_Key, t));

}

void MoveArrowModel::Update()
{
	/**m_Forward.m_Orientation.SetPosition(m_Orientation.GetPosition());
	m_Up.m_Orientation.SetPosition(m_Orientation.GetPosition());
	m_Right.m_Orientation.SetPosition(m_Orientation.GetPosition());/**/
}

void MoveArrowModel::SetPosition(const CU::Vector3f& pos)
{
	m_Orientation.SetPosition(pos);
	m_Forward.m_Orientation.SetPosition(pos);
	m_Up.m_Orientation.SetPosition(pos);
	m_Right.m_Orientation.SetPosition(pos);

}

DirectionalArrow::~DirectionalArrow()
{
}
