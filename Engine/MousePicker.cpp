#include "stdafx.h"
#include "MousePicker.h"
#include "Camera.h"
#include "Engine.h"
namespace Snowblind
{
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
		CalculateCurrentRay(x,y);
		return myCurrentRay;
	}

	void CMousePicker::CalculateCurrentRay(float x, float y)
	{
		CU::Vector2f cursorPos;
		cursorPos.x = x;//CU::Input::InputWrapper::GetInstance()->GetX();
		cursorPos.y = y;//CU::Input::InputWrapper::GetInstance()->GetY();

		cursorPos = NormalizedSpace(cursorPos.x, cursorPos.y);
		CU::Vector4f clip = CU::Vector4f(cursorPos.x, cursorPos.y, 1.f, 0.f); //Clip Space
		CU::Vector4f eyeCoord = ToEyeCoords(clip);
		CU::Vector3f ray = ToWorldCoords(eyeCoord);

		CU::Math::Normalize(ray);
		myCurrentRay = ray;
	}

	CU::Vector4f CMousePicker::ToEyeCoords(const CU::Vector4f& clipCoords)
	{
		CU::Matrix44f invertedProjection = CU::Math::InverseReal(myCamera->GetProjection());
		CU::Vector4f rayOrigin = invertedProjection * clipCoords;
		return CU::Vector4f(rayOrigin.x, rayOrigin.y, 1.f, 0.f);
	}

	CU::Vector3f CMousePicker::ToWorldCoords(const CU::Vector4f& eyeCoords)
	{
		CU::Matrix44f view = CU::Math::Inverse(myCamera->GetOrientation());
		CU::Vector4f rayEnd = view * eyeCoords;
		return CU::Vector3f(rayEnd.x, rayEnd.y, rayEnd.z);
	}

	CU::Vector2f CMousePicker::NormalizedSpace(float x, float y)
	{
		x = x * 2.f / myEngine->GetWindowSize().myWidth - 1.f;
		y = 1.f - (y * 2.f) / myEngine->GetWindowSize().myHeight;
		return CU::Vector2f(x, y);
	}
};