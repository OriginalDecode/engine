#include "stdafx.h"
#include "GizmoBase.h"
#include "Surface.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include <Math/Vector/Vector.h>
void GizmoBase::CreateGizmoHandle(GizmoHandle& gizmo_handle, std::string model_key, const std::string& texture_path, GizmoHandle::eDirection direction)
{
	gizmo_handle.m_Key = Engine::GetInstance()->LoadModel(model_key, "Shaders/T_Movement_Arrow.json", true);
	gizmo_handle.m_DirColor = Engine::GetInstance()->GetTexture(texture_path);
	gizmo_handle.m_Model = Engine::GetInstance()->GetModel(gizmo_handle.m_Key);

	CU::GrowingArray<Model*> children = gizmo_handle.m_Model->GetChildModels();
	for (Model* child : children)
	{
		CU::GrowingArray<CSurface*>& surfaces = child->GetSurfaces();
		for (CSurface* surface : surfaces)
		{
			surface->ClearTextures();
			surface->AddTexture(gizmo_handle.m_DirColor->GetShaderView());
		}
	}
	gizmo_handle.direction = direction;
	gizmo_handle.Initiate();
}

void GizmoBase::Render()
{
	if (!m_Active)
		return;

	RenderBoxes();

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

void GizmoBase::Update()
{
	m_Up.Update();
	m_Right.Update();
	m_Forward.Update();
}

void GizmoBase::SetPosition(const CU::Vector3f& position)
{
	m_Orientation.SetPosition(position);

	m_Forward.m_Orientation.SetPosition(position);
	OffsetGizmoHandle(m_Forward);

	m_Up.m_Orientation.SetPosition(position);
	OffsetGizmoHandle(m_Up);

	m_Right.m_Orientation.SetPosition(position);
	OffsetGizmoHandle(m_Right);
}

bool GizmoBase::Inside(const CU::Vector3f& position, GizmoHandle* result)
{
	if (m_Forward.Inside(position))
	{
		result = &m_Forward;
		return true;
	}
	else if (m_Up.Inside(position))
	{
		result = &m_Up;
		return true;
	}
	else if (m_Right.Inside(position))
	{
		result = &m_Right; 
		return true;
	}

	result = nullptr;
	return false;
}

void GizmoBase::OffsetGizmoHandle(GizmoHandle& gizmo_handle)
{
	CU::Vector4f right = gizmo_handle.m_Orientation.GetRight();
	CU::Vector4f position = gizmo_handle.m_Orientation.GetTranslation();
	position = position + right * gizmo_handle.m_Offset;
	gizmo_handle.m_Orientation.SetPosition(position);
}
