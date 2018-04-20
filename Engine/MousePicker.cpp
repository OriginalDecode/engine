#include "stdafx.h"
#include "MousePicker.h"
#include "Camera.h"
#include "Engine.h"

CMousePicker::CMousePicker()
{
	myEngine = Engine::GetInstance();
	myCamera = myEngine->GetCamera();
}

CMousePicker::~CMousePicker()
{

}

const CU::Vector3f& CMousePicker::GetCurrentRay(float x, float y)
{
	CalculateCurrentRay(x, y);
	return myCurrentRay;
}

const CU::Vector3f& CMousePicker::GetCurrentRay(CU::Vector2f pos)
{
	CalculateCurrentRay(pos.x, pos.y);
	return myCurrentRay;
}

void CMousePicker::CalculateCurrentRay(float x, float y)
{
	CU::Vector2f cursorPos;
	cursorPos.x = x;
	cursorPos.y = y;

	cursorPos = NormalizedSpace(cursorPos.x, cursorPos.y);
	CU::Vector4f clip = CU::Vector4f(cursorPos.x, cursorPos.y, 1.f, 0.f); //Clip Space
	
	CU::Vector4f eyeCoord = ToEyeCoords(clip);
	CU::Vector3f ray = ToWorldCoords(eyeCoord);
	m_RayStart = ray;
	CU::Math::Normalize(ray);
	myCurrentRay = ray;
}

CU::Vector4f CMousePicker::ToEyeCoords(const CU::Vector4f& clipCoords)
{
	CU::Matrix44f invertedProjection = ~myCamera->GetPerspective(); //CU::Math::InverseReal(myCamera->GetPerspective());
	CU::Vector4f rayOrigin =  clipCoords * invertedProjection;
	return CU::Vector4f(rayOrigin.x, rayOrigin.y, 1.f, 0.f);
}

CU::Vector3f CMousePicker::ToWorldCoords(const CU::Vector4f& eyeCoords)
{
	CU::Matrix44f view = CU::Math::Inverse(myCamera->GetOrientation()); //doesn't account for the offset ???6
	CU::Vector4f rayEnd = eyeCoords * view;
	return CU::Vector3f(rayEnd.x, rayEnd.y, rayEnd.z);
}

CU::Vector2f CMousePicker::NormalizedSpace(float x, float y)
{
	x = x * 2.f / myEngine->GetInnerSize().m_Width - 1.f;
	y = 1.f - (y * 2.f) / myEngine->GetInnerSize().m_Height;
	return CU::Vector2f(x, y);
}
