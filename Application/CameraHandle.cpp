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

void CameraHandle::Initiate()
{
	m_Camera = Engine::GetInstance()->GetCamera();
	m_Frustum.Initiate(0.001f, 25.f, 90.f, &m_Camera->GetOrientation());
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
