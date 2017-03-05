#include "stdafx.h"
#include "GizmoBase.h"
#include "Surface.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include <Math/Vector/Vector.h>
void GizmoBase::CreateGizmoHandle(GizmoHandle& gizmo_handle, const std::string& model_key, const std::string& texture_path, GizmoHandle::eDirection direction)
{
	gizmo_handle.m_Key = Engine::GetInstance()->LoadModel(model_key, "Data/Shaders/T_Movement_Arrow.json");
	gizmo_handle.m_DirColor = Engine::GetInstance()->GetTexture(texture_path);
	gizmo_handle.m_Model = Engine::GetInstance()->GetModel(gizmo_handle.m_Key);

	CU::GrowingArray<CModel*> children = gizmo_handle.m_Model->GetChildModels();
	for (CModel* child : children)
	{
		CU::GrowingArray<CSurface*>& surfaces = child->GetSurfaces();
		for (CSurface* surface : surfaces)
		{
			surface->ClearTextures();
			surface->AddTexture(gizmo_handle.m_DirColor->GetShaderView());
		}
	}
	gizmo_handle.direction = direction;
}

void GizmoBase::Render()
{
	Engine* engine = Engine::GetInstance();
	CU::Vector4f camera_position = engine->GetCamera()->GetPosition();
	float distance = CU::Math::Length(m_Orientation.GetTranslation() - camera_position) * 0.5f;
	CU::Vector4f scale = CU::Vector4f(distance, distance, distance, 1);
	//CU::Matrix44f scale_matrix = CU::Matrix44f::CreateScaleMatrix(scale);
	CU::Matrix44f t = m_Right.m_Orientation;
	//t = CU::Matrix44f::CreateScaleMatrix(scale) * t;
	Synchronizer* sync = engine->GetSynchronizer();
	sync->AddRenderCommand(RenderCommand(eType::MODEL_NO_DEFERRED, m_Right.m_Key, t));

	//__________________________

	t = m_Forward.m_Orientation;
	//t = CU::Matrix44f::CreateScaleMatrix(scale) * t;
	sync->AddRenderCommand(RenderCommand(eType::MODEL_NO_DEFERRED, m_Forward.m_Key, t));

	//__________________________

	t = m_Up.m_Orientation;
	//t = CU::Matrix44f::CreateScaleMatrix(scale) * t;
	sync->AddRenderCommand(RenderCommand(eType::MODEL_NO_DEFERRED, m_Up.m_Key, t));

	//Update();
}

void GizmoBase::RenderBoxes()
{
	m_Up.RenderBox();
	m_Right.RenderBox();
	m_Forward.RenderBox();
}
