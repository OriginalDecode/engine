#include "stdafx.h"
#include "PositionGizmo.h"
#include "Texture.h"
#include "Surface.h"
#include "Model.h"

#include "Engine.h"
#include "Synchronizer.h"
#include "RenderCommand.h"

//void PositionGizmo::Initiate()
//{
//	CreateGizmoHandle(m_Forward, "Data/Model/blue_arrow.fbx", "Data/Textures/blue.dds", GizmoHandle::eDirection::FORWARD);
//	m_Forward.m_Orientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(90.f) * -1) * m_Forward.m_Orientation;
//	OffsetGizmoHandle(m_Forward);
//	m_Forward.Initiate();
//
//	//________________________________________________
//
//	CreateGizmoHandle(m_Up, "Data/Model/green_arrow.fbx", "Data/Textures/green.dds", GizmoHandle::eDirection::UP);
//	m_Up.m_Orientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(90.f) * 1) * m_Up.m_Orientation;
//	OffsetGizmoHandle(m_Up);
//	m_Up.Initiate();
//
//	//________________________________________________
//
//	CreateGizmoHandle(m_Right, "Data/Model/red_arrow.fbx", "Data/Textures/red.dds", GizmoHandle::eDirection::RIGHT);
//	OffsetGizmoHandle(m_Right);
//	m_Right.Initiate();
//
//	//________________________________________________
//
//}
//
//void PositionGizmo::OffsetGizmoHandle(GizmoHandle& gizmo_handle)
//{
//	CU::Vector4f right = gizmo_handle.m_Orientation.GetRight();
//	CU::Vector4f position = gizmo_handle.m_Orientation.GetTranslation();
//	position = position + right * 1.5f;
//	gizmo_handle.m_Orientation.SetPosition(position);
//}
//
//
//void PositionGizmo::SetPosition(const CU::Vector3f& pos)
//{
//	m_Orientation.SetPosition(pos);
//
//	m_Forward.m_Orientation.SetPosition(pos);
//	OffsetGizmoHandle(m_Forward);
//
//	m_Up.m_Orientation.SetPosition(pos);
//	OffsetGizmoHandle(m_Up);
//
//	m_Right.m_Orientation.SetPosition(pos);
//	OffsetGizmoHandle(m_Right);
//}