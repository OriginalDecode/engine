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
	if ( !orientation )
	{
		m_Camera = Engine::GetInstance()->GetCamera();
		m_Frustum.Initiate(0.f, 50.f, m_Camera->GetFOV(), &m_Camera->GetOrientation());
	}
	else
	{
		m_Frustum.Initiate(0.f, 50.f, 90.f, orientation);
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

const Frustum& CameraHandle::GetFrustum()
{
	return m_Frustum;
}
