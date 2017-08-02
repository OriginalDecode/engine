#include "stdafx.h"
#include "Camera.h"
//#include <SimpleMath.h>
#include "../Input/ControllerInput.h"

static constexpr float s_pi = 3.1415926535f;

void Camera::CreatePerspectiveProjection(float width, float height, float near_plane, float far_plane, float fov)
{
	DL_ASSERT_EXP(!m_ProjectionCreated, "Projection already created. Can't have two 3D projection matrices on same camera!");
	m_ProjectionCreated = true;
	m_CurrentFoV = fov;
	m_ProjectionMatrix = CU::Matrix44f::CreateProjectionMatrixLH(near_plane, far_plane, height / width, cl::DegreeToRad(m_CurrentFoV));
}

void Camera::CreateOrthogonalProjection(float width, float height, float near_plane, float far_plane)
{
	m_OrthogonalMatrix = CU::Matrix44f::CreateOrthogonalMatrixLH(width, height, near_plane, far_plane);
}

void Camera::CreateOrthographicProjection(float width, float height, float near_plane, float far_plane)
{
	DL_ASSERT_EXP(!m_ProjectionCreated, "Projection already created. Can't have two 3D projection matrices on same camera!");
	m_ProjectionCreated = true;
	m_ProjectionMatrix = CU::Matrix44f::CreateOrthographicMatrixLH(width, height, near_plane, far_plane);
}

CU::Vector3f Camera::RotateAroundPoint(const CU::Vector3f& position)
{
	CU::Vector3f original_pos = GetPosition();
	SetPosition(original_pos - position);
	float dt = Engine::GetInstance()->GetDeltaTime();
	m_Orientation = m_Orientation * CU::Matrix44f::CreateRotateAroundZ(cl::DegreeToRad(45.f) * dt);
	//m_Orientation = m_Orientation * CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(45.f) * dt);

	SetPosition(GetPosition() + position);


	CU::Vector4f new_forward = position - original_pos;
	CU::Math::Normalize(new_forward);
	m_Orientation.SetForward(new_forward);

	return CU::Vector3f(position - original_pos);
}

void Camera::SetPosition(const CU::Vector3f& position)
{
	m_Orientation2 = m_Orientation;
	m_Orientation.SetPosition(position);
}

void Camera::SetPosition2(const CU::Vector3f& position)
{
	m_Orientation.SetPosition(position);
	m_Orientation2.SetPosition(position);
}

void Camera::SetTranslation(const CU::Vector4f& translation)
{
	m_Orientation2 = m_Orientation;
	m_Orientation.SetTranslation(translation);
}

void Camera::Update(const ControllerState& controller_state)
{

	float x_value = ( float ) controller_state.m_ThumbRX;
	float y_value = ( float ) controller_state.m_ThumbRY;

	float magnitude = sqrt(x_value * x_value + y_value * y_value); //Do something to skip the sqrt?
	float normalized = 0.f;
	const float r_thumb_deadzone = 8689.f;

	if ( magnitude > r_thumb_deadzone )
	{
		if ( magnitude > SHRT_MAX )
			magnitude = SHRT_MAX;

		magnitude -= r_thumb_deadzone;

		normalized = magnitude / ( SHRT_MAX - r_thumb_deadzone );

	}
	else
	{
		x_value = 0.f;
		y_value = 0.f;
	}


	if ( normalized < -0.5f || normalized > 0.5f )
	{
		x_value /= 2.f;
		y_value /= 2.f;
	}

	m_CenterPoint.x += ( x_value / SHRT_MAX ) * m_LookSpeedModifier;
	m_CenterPoint.y -= ( y_value / SHRT_MAX ) * m_LookSpeedModifier;
	m_CenterPoint.y = fmaxf(fminf(1.57f, m_CenterPoint.y), -1.57f);

	OrientCamera();
}

void Camera::Update(const CU::Vector2f& cursor_pos)
{
	float x_value = cursor_pos.x;
	float y_value = cursor_pos.y;

	m_CenterPoint.x += x_value * m_LookSpeedModifier;
	m_CenterPoint.y += y_value * m_LookSpeedModifier;
	m_CenterPoint.y = fmaxf(fminf(1.57f, m_CenterPoint.y), -1.57f);

	OrientCamera();
}

void Camera::SetOrientation(const CU::Matrix44f& matrix)
{
	m_Orientation2 = m_Orientation;
	m_Orientation = matrix;
}

void Camera::RotateAroundX(float rad)
{
	m_Orientation = CU::Matrix44f::CreateRotateAroundX(rad) * m_Orientation;
}

void Camera::RotateAroundY(float rad)
{
	m_Orientation = CU::Matrix44f::CreateRotateAroundY(rad) * m_Orientation;
}

void Camera::RotateAroundZ(float rad)
{
	m_Orientation = CU::Matrix44f::CreateRotateAroundZ(rad) * m_Orientation;
}

void Camera::SetFOV(float field_of_view)
{
	const WindowSize& window_size = Engine::GetInstance()->GetWindowSize();
	if ( field_of_view >= 60.f && field_of_view <= 120.f || m_IsShadowCamera)
		m_CurrentFoV = field_of_view;


	m_ProjectionMatrix.SetPerspectiveFOV(cl::DegreeToRad(m_CurrentFoV), window_size.m_Height / window_size.m_Width);
}

void Camera::RecalculatePerspective(float width, float height, float near_plane, float far_plane)
{
	m_ProjectionMatrix = CU::Matrix44f::CreateProjectionMatrixLH(near_plane, far_plane, height / width, cl::DegreeToRad(m_CurrentFoV));
}

void Camera::InvertPitch()
{
	m_Pitch = m_Pitch.Inverted();
	UpdateOrientation();
}

void Camera::InvertRoll()
{
	m_Roll = m_Roll.Inverted();
	UpdateOrientation();
}

void Camera::InvertYaw()
{
	m_Yaw = m_Yaw.Inverted();
	UpdateOrientation();
}

void Camera::InvertAll()
{
	InvertPitch();
	InvertRoll();
	InvertYaw();
}

void Camera::SetAt(const CU::Vector4f& at)
{
	m_Orientation2 = m_Orientation;
	m_Orientation.SetForward(at);
}

void Camera::Move(eDirection aDirection, float aSpeed)
{
	CU::Math::Vector4<float> position;
	m_Orientation2 = m_Orientation;

	position = m_Orientation.GetTranslation();
	switch ( aDirection )
	{
		case eDirection::FORWARD:
		MoveForwardAndBack(position, aSpeed);
		break;
		case eDirection::BACK:
		MoveForwardAndBack(position, aSpeed);
		break;
		case eDirection::UP:
		MoveUpAndDown(position, aSpeed);
		break;
		case eDirection::DOWN:
		MoveUpAndDown(position, aSpeed);
		break;
		case eDirection::LEFT:
		MoveLeftAndRight(position, aSpeed);
		break;
		case eDirection::RIGHT:
		MoveLeftAndRight(position, aSpeed);
		break;
	}
	m_Orientation.SetTranslation(position);
}

void Camera::UpdateOrientation()
{
	CU::Vector3f axisX(1.f, 0, 0);
	CU::Vector3f axisY(0, 1.f, 0);
	CU::Vector3f axisZ(0, 0, 1.f);

	axisX = m_Yaw * m_Pitch * axisX;
	axisY = m_Yaw * m_Pitch * axisY;
	axisZ = m_Yaw * m_Pitch * axisZ;

	m_Orientation2 = m_Orientation;

	m_Orientation[0] = axisX.x;
	m_Orientation[1] = axisX.y;
	m_Orientation[2] = axisX.z;

	m_Orientation[4] = axisY.x;
	m_Orientation[5] = axisY.y;
	m_Orientation[6] = axisY.z;

	m_Orientation[8] = axisZ.x;
	m_Orientation[9] = axisZ.y;
	m_Orientation[10] = axisZ.z;
}

void Camera::MoveForwardAndBack(CU::Vector4f& aPosition, float aSpeed)
{
	CU::Math::Vector4<float> forward = m_Orientation.GetForward();
	aPosition += forward * aSpeed;
}

void Camera::MoveUpAndDown(CU::Vector4f& aPosition, float aSpeed)
{
	CU::Math::Vector4<float> up = m_Orientation.GetUp();
	aPosition += up * aSpeed;
}

void Camera::MoveLeftAndRight(CU::Vector4f& aPosition, float aSpeed)
{
	CU::Math::Vector4<float> right = m_Orientation.GetRight();
	aPosition += right * aSpeed;
}


void Camera::OrientCamera()
{
	m_Pitch = CU::Quaternion(CU::Vector3f(1.f, 0, 0), m_CenterPoint.y);
	m_Yaw = CU::Quaternion(CU::Vector3f(0, 1.f, 0), m_CenterPoint.x);

	CU::Vector3f axisX(1.f, 0, 0);
	CU::Vector3f axisY(0, 1.f, 0);
	CU::Vector3f axisZ(0, 0, 1.f);

	axisX = m_Yaw * m_Pitch * axisX;
	axisY = m_Yaw * m_Pitch * axisY;
	axisZ = m_Yaw * m_Pitch * axisZ;

	m_Orientation2 = m_Orientation;

	m_Orientation[0] = axisX.x;
	m_Orientation[1] = axisX.y;
	m_Orientation[2] = axisX.z;

	m_Orientation[4] = axisY.x;
	m_Orientation[5] = axisY.y;
	m_Orientation[6] = axisY.z;

	m_Orientation[8] = axisZ.x;
	m_Orientation[9] = axisZ.y;
	m_Orientation[10] = axisZ.z;
}
