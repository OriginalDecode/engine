#include "CameraHandle.h"
#include <Engine.h>
#include <Camera.h>

CameraHandle* CameraHandle::m_Instance = nullptr;

CameraHandle* CameraHandle::GetInstance()
{
	return m_Instance;
}
	
void CameraHandle::Create()
{
	m_Instance = new CameraHandle;
}

void CameraHandle::Destroy()
{
	delete m_Instance;
	m_Instance = nullptr;
}

void CameraHandle::Initiate(CU::Matrix44f* orientation)
{
	float far_plane = 100.f;
	float near_plane = 0.1f;

	if ( !orientation )
	{
		m_Camera = Engine::GetInstance()->GetCamera();
		m_Frustum.Initiate(near_plane, far_plane, m_Camera->GetFOV(), &m_Camera->GetOrientation());
	}
	else
	{
		m_Frustum.Initiate(near_plane, far_plane, 90.f, orientation);
	}
}

void CameraHandle::Update()
{
	m_Frustum.Update();
}

Camera* CameraHandle::GetCamera()
{
	return m_Camera;
}

const Frustum& CameraHandle::GetFrustum() const
{
	return m_Frustum;
}

Frustum& CameraHandle::GetFrustum()
{
	return m_Frustum;
}
