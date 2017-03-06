#include "stdafx.h"
#include "GizmoShared.h"

#include "Engine.h"
#include "Synchronizer.h"
#include "RenderCommand.h"

GizmoHandle::~GizmoHandle()
{
}

void GizmoHandle::Initiate()
{
	CU::Plane<float> plane0;

	CU::Vector4f up = m_Orientation.GetUp();
	CU::Vector4f right = m_Orientation.GetRight();
	CU::Vector4f forward = m_Orientation.GetForward();


	CU::Vector4f position;
	CU::Vector3f whd = m_Model->GetWHD();
	//x

	position = m_Orientation.GetTranslation();
	position += right * whd.x;
	plane0.InitWithPointAndNormal(position, right);
	m_OBB.AddPlane(plane0);

	position = m_Orientation.GetTranslation();
	position -= right * whd.x;
	right -= (right * 2.f);
	plane0.InitWithPointAndNormal(position, right);
	m_OBB.AddPlane(plane0);

	//y
	position = m_Orientation.GetTranslation();
	position += up * whd.y;
	plane0.InitWithPointAndNormal(position, up);
	m_OBB.AddPlane(plane0);

	position = m_Orientation.GetTranslation();
	position -= up * whd.y;
	up -= (up * 2.f);
	plane0.InitWithPointAndNormal(position, up);
	m_OBB.AddPlane(plane0);

	//z
	position = m_Orientation.GetTranslation();
	position += forward * whd.z;
	plane0.InitWithPointAndNormal(position, forward);
	m_OBB.AddPlane(plane0);


	position = m_Orientation.GetTranslation();
	position -= forward * whd.z;
	forward -= (forward * 2.f);
	plane0.InitWithPointAndNormal(position, forward);
	m_OBB.AddPlane(plane0);
}


bool GizmoHandle::Inside(const CU::Vector3f& position)
{
	return m_OBB.Inside(position);
}

void GizmoHandle::SetPosition(const CU::Vector3f& position)
{
	m_Orientation.SetPosition(position);
}

void GizmoHandle::RenderBox()
{
	CU::Vector4f camera_position = Engine::GetInstance()->GetCamera()->GetPosition();
	float distance = CU::Math::Length(m_Orientation.GetTranslation() - camera_position) * 0.5f;
	SLinePoint p1, p2, p3, p4, p5, p6, p7, p8;
	p1.color = CU::Vector4f(1, 1, 1, 1);
	p2.color = p1.color;
	p3.color = p1.color;
	p4.color = p1.color;
	p5.color = p1.color;
	p6.color = p1.color;
	p7.color = p1.color;
	p8.color = p1.color;

	p1.position = m_Orientation.GetTranslation(); // translation.myOrientation.GetTranslation();
	p2.position = p1.position;
	p3.position = p1.position;
	p4.position = p1.position;
	p5.position = p1.position;
	p6.position = p1.position;
	p7.position = p1.position;
	p8.position = p1.position;
	CU::Vector3f whd = m_Model->GetWHD() /** distance*/;
	p1.position -= m_Orientation.GetRight() * whd.x;
	p1.position -= m_Orientation.GetUp() * whd.y;
	p1.position -= m_Orientation.GetForward() * whd.z;

	p2.position += m_Orientation.GetRight() * whd.x;
	p2.position -= m_Orientation.GetUp() * whd.y;
	p2.position -= m_Orientation.GetForward() * whd.z;

	p3.position += m_Orientation.GetRight() * whd.x;
	p3.position -= m_Orientation.GetUp() * whd.y;
	p3.position += m_Orientation.GetForward() * whd.z;

	p4.position -= m_Orientation.GetRight() * whd.x;
	p4.position -= m_Orientation.GetUp() * whd.y;
	p4.position += m_Orientation.GetForward() * whd.z;

	p5.position -= m_Orientation.GetRight() * whd.x;
	p5.position += m_Orientation.GetUp() * whd.y;
	p5.position -= m_Orientation.GetForward() * whd.z;

	p6.position -= m_Orientation.GetRight() * whd.x;
	p6.position += m_Orientation.GetUp() * whd.y;
	p6.position += m_Orientation.GetForward() * whd.z;

	p7.position += m_Orientation.GetRight() * whd.x;
	p7.position += m_Orientation.GetUp() * whd.y;
	p7.position -= m_Orientation.GetForward() * whd.z;

	p8.position += m_Orientation.GetRight() * whd.x;
	p8.position += m_Orientation.GetUp() * whd.y;
	p8.position += m_Orientation.GetForward() * whd.z;

	Engine* engine = Engine::GetInstance();
	Synchronizer* synch = engine->GetSynchronizer();
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p1, p2));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p2, p3));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p3, p4));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p4, p1));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p1, p5));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p5, p6));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p6, p8));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p8, p7));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p7, p5));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p6, p4));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p7, p2));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p8, p3));
}

void GizmoHandle::Update()
{
	CU::Vector4f camera_position = Engine::GetInstance()->GetCamera()->GetPosition();
	float distance = CU::Math::Length(m_Orientation.GetTranslation() - camera_position) * 0.5f;
	CU::Vector4f up = m_Orientation.GetUp();
	CU::Vector4f right = m_Orientation.GetRight();
	CU::Vector4f forward = m_Orientation.GetForward();


	CU::Vector4f position;
	CU::Vector3f whd = m_Model->GetWHD() /** distance*/;
	//x

	position = m_Orientation.GetTranslation();
	position += right * whd.x;
	m_OBB.m_Planes[0].InitWithPointAndNormal(position, right);

	position = m_Orientation.GetTranslation();
	position -= right * whd.x;
	right -= (right * 2.f);
	m_OBB.m_Planes[1].InitWithPointAndNormal(position, right);

	//y
	position = m_Orientation.GetTranslation();
	position += up * whd.y;
	m_OBB.m_Planes[2].InitWithPointAndNormal(position, up);

	position = m_Orientation.GetTranslation();
	position -= up * whd.y;
	up -= (up * 2.f);
	m_OBB.m_Planes[3].InitWithPointAndNormal(position, up);

	//z
	position = m_Orientation.GetTranslation();
	position += forward * whd.z;
	m_OBB.m_Planes[4].InitWithPointAndNormal(position, forward);


	position = m_Orientation.GetTranslation();
	position -= forward * whd.z;
	forward -= (forward * 2.f);
	m_OBB.m_Planes[5].InitWithPointAndNormal(position, forward);
}
