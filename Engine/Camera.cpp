#include "stdafx.h"
#include "Camera.h"
#include "../Input/ControllerInput.h"

static constexpr float s_pi = 3.1415926535f;

void Camera::SetIsShadowCamera(bool is_shadow_camera)
{
	m_IsShadowCamera = is_shadow_camera;
}

bool Camera::IsShadowCamera() const
{
	return m_IsShadowCamera;
}

void Camera::CreatePerspectiveProjection(float width, float height, float near_plane, float far_plane, float fov)
{
	ASSERT(!m_ProjectionCreated, "Projection already created. Can't have two 3D projection matrices on same camera!");
	m_ProjectionCreated = true;
	m_CurrentFoV = fov;
	m_ProjectionMatrix = CU::Matrix44f::CreateProjectionMatrixLH(near_plane, far_plane, height / width, cl::DegreeToRad(m_CurrentFoV));
}

void Camera::CreateOrthographicProjection(float width, float height, float near_plane, float far_plane)
{
	ASSERT(!m_ProjectionCreated, "Projection already created. Can't have two 3D projection matrices on same camera!");
	m_ProjectionCreated = true;
	m_ProjectionMatrix = CU::Matrix44f::CreateOrthographicMatrixLH(width, height, near_plane, far_plane);
}

void Camera::CreateOrthogonalProjection(float width, float height, float near_plane, float far_plane)
{
	m_OrthogonalMatrix = CU::Matrix44f::CreateOrthogonalMatrixLH(width, height, near_plane, far_plane);
}


CU::Matrix44f& Camera::GetPerspective()
{
	return m_ProjectionMatrix;
}

const CU::Matrix44f& Camera::GetPerspective() const
{
	return m_ProjectionMatrix;
}

CU::Matrix44f& Camera::GetInvProjection()
{
	return m_InvProjectionMatrix;
}

const CU::Matrix44f& Camera::GetInvProjection() const
{
	return m_InvProjectionMatrix;
}


const CU::Matrix44f& Camera::GetOrthogonal() const
{
	return m_OrthogonalMatrix;
}

CU::Matrix44f& Camera::GetOrthogonal()
{
	return m_OrthogonalMatrix;
}

const CU::Matrix44f& Camera::GetOrthographic() const
{
	return m_ProjectionMatrix;
}

CU::Vector3f Camera::RotateAroundPoint(const CU::Vector3f& position)
{
	CU::Vector3f original_pos = GetPosition();
	SetPosition(original_pos - position);
	float dt = 0.016f;
	m_Orientation = m_Orientation * CU::Matrix44f::CreateRotateAroundZ(cl::DegreeToRad(45.f) * dt);
	//m_Orientation = m_Orientation * CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(45.f) * dt);

	SetPosition(GetPosition() + position);


	CU::Vector4f new_forward = position - original_pos;
	CU::Math::Normalize(new_forward);
	m_Orientation.SetForward(new_forward);

	return CU::Vector3f(position - original_pos);
}

CU::Vector4f& Camera::GetPos()
{
	return m_Orientation2.rows[3];
}

CU::Matrix44f& Camera::GetViewProjection()
{
	return m_ViewProj;
}

CU::Matrix44f& Camera::Get2DViewProjection()
{
	return m_OrthogonalViewProj;
}

CU::Matrix44f& Camera::GetOrientation()
{
	return m_Orientation2;
}

CU::Matrix44f& Camera::GetCurrentOrientation()
{
	return m_Orientation;
}

CU::Vector3f Camera::GetPosition() const
{
	return m_Orientation2.GetPosition();
}

CU::Matrix44f& Camera::Get2DOrientation()
{
	my2DOrientation = CU::Math::Inverse(my2DOrientation);
	return my2DOrientation;
}

const CU::Vector4f& Camera::GetAt() const
{
	return (m_Rotation2 * m_Orientation2).GetForward();
}

void Camera::SetPosition(const CU::Vector3f& position)
{
	m_Orientation2 = m_Orientation;
	m_Orientation.SetPosition(position);
	m_TargetPosition = position;
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
#ifdef _DEBUG
	const float sensitivity = debug::DebugHandle::GetInstance()->m_ControllerLookSens;
#else
	const float sensitivity = 0.05f;
#endif
	float x_value = (float)controller_state.m_ThumbRX;
	float y_value = (float)controller_state.m_ThumbRY;

	float magnitude = (x_value * x_value + y_value * y_value);
	float normalized = 0.f;
	const float r_thumb_deadzone = 8689.f;

	if (magnitude > r_thumb_deadzone * r_thumb_deadzone)
	{
		if (magnitude > SHRT_MAX)
			magnitude = SHRT_MAX;

		magnitude -= r_thumb_deadzone;

		normalized = magnitude / (SHRT_MAX - r_thumb_deadzone);

	}
	else
	{
		x_value = 0.f;
		y_value = 0.f;
	}


	if (normalized < -0.5f || normalized > 0.5f)
	{
		x_value /= 2.f;
		y_value /= 2.f;
	}

	m_CenterPoint.x += (x_value / SHRT_MAX) * sensitivity;
	m_CenterPoint.y -= (y_value / SHRT_MAX) * sensitivity;
	m_CenterPoint.y = fmaxf(fminf(1.57f, m_CenterPoint.y), -1.57f);

	OrientCamera();
}

void Camera::Orient(const CU::Vector2f& cursor_pos)
{
#ifdef _DEBUG
	const float sensitivity = debug::DebugHandle::GetInstance()->m_MouseLookSense;
#else
	const float sensitivity = 0.005f;
#endif
	float x_value = cursor_pos.x;
	float y_value = cursor_pos.y;

	m_CenterPoint.x += x_value * sensitivity;
	m_CenterPoint.y += y_value * sensitivity;
	m_CenterPoint.y = fmaxf(fminf(1.57f, m_CenterPoint.y), -1.57f);

	OrientCamera();


}

void Camera::Update()
{
	m_Rotation2 = m_Rotation;
	m_Orientation2 = m_Orientation;
	m_PixelOrientation = m_Rotation2 * m_Orientation2;
	m_ViewProj = CU::Math::Inverse(m_PixelOrientation) * m_ProjectionMatrix;
	m_InvProjectionMatrix = CU::Math::InverseReal(m_ProjectionMatrix);
}

void Camera::SetOrientation(const CU::Matrix44f& matrix)
{
	m_Orientation2 = m_Orientation;
	m_Orientation = matrix;
}

void Camera::RotateAroundX(float rad)
{
	m_Rotation = CU::Matrix44f::CreateRotateAroundX(rad) * m_Rotation;
}

void Camera::RotateAroundY(float rad)
{
	m_Rotation = CU::Matrix44f::CreateRotateAroundY(rad) * m_Rotation;
}

void Camera::RotateAroundZ(float rad)
{
	m_Rotation = CU::Matrix44f::CreateRotateAroundZ(rad) * m_Rotation;
}

void Camera::SetRotationX(float rad)
{
	m_Rotation.SetRotation3dX(rad);
}

void Camera::SetRotationY(float rad)
{
	m_Rotation.SetRotation3dY(rad);
}

void Camera::SetRotationZ(float rad)
{
	m_Rotation.SetRotation3dZ(rad);
}

void Camera::SetRotation(const CU::Matrix44f& rot)
{
	m_Rotation2 = m_Rotation;
	m_Rotation = rot;
	OrientCamera();
}

void Camera::SetFOV(float field_of_view)
{
	const WindowSize& window_size = Engine::GetInstance()->GetInnerSize();
	if (field_of_view >= 60.f && field_of_view <= 120.f || m_IsShadowCamera)
		m_CurrentFoV = field_of_view;

	m_ProjectionMatrix.SetPerspectiveFOV(cl::DegreeToRad(m_CurrentFoV), window_size.m_Height / window_size.m_Width);
}

float Camera::GetFOV()
{
	return m_CurrentFoV;
}

float* Camera::GetFOVRef()
{
	return &m_CurrentFoV;
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


void Camera::LookAt(const CU::Vector3f& eye, const CU::Vector3f& target, const CU::Vector3f& up)
{
	m_Rotation2 = m_Rotation;
	m_Rotation.LookAt(eye, target, up);
}


void Camera::SetAt(const CU::Vector4f& at)
{
	m_Orientation2 = m_Orientation;
	m_Orientation.SetForward(at);
}

void Camera::Move(eDirection aDirection, float acceleration)
{
	CU::Vector4f position, prevpos;
	m_Orientation2 = m_Orientation;

	position = m_Orientation.GetTranslation();
	prevpos = position;

	switch (aDirection)
	{
	case eDirection::FORWARD:
		Move(position, m_Rotation.GetForward(), acceleration);
		m_Direction += cl::AsVector3(m_Rotation.GetForward());
		break;
	case eDirection::BACK:
		Move(position, m_Rotation.GetForward(), acceleration);
		m_Direction -= cl::AsVector3(m_Rotation.GetForward());
		break;
	case eDirection::UP:
		Move(position, m_Rotation.GetUp(), acceleration);
		m_Direction += cl::AsVector3(m_Rotation.GetUp());
		break;
	case eDirection::DOWN:
		Move(position, m_Rotation.GetUp(), acceleration);
		m_Direction -= cl::AsVector3(m_Rotation.GetUp());
		break;
	case eDirection::RIGHT:
		Move(position, m_Rotation.GetRight(), acceleration);
		m_Direction += cl::AsVector3(m_Rotation.GetRight());
		break;
	case eDirection::LEFT:
		Move(position, m_Rotation.GetRight(), acceleration);
		m_Direction -= cl::AsVector3(m_Rotation.GetRight());
		break;
	}
	CU::Math::Normalize(m_Direction);
	
	m_TargetPosition = cl::AsVector3(position) + (m_Direction * 10.f);
	m_Orientation.SetTranslation(position);
}

void Camera::Move(CU::Vector4f& position, const CU::Vector4f& dir, float acceleration)
{
	position += dir * acceleration;
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
	m_Rotation2 = m_Rotation;

	m_Rotation[0] = axisX.x;
	m_Rotation[1] = axisX.y;
	m_Rotation[2] = axisX.z;

	m_Rotation[4] = axisY.x;
	m_Rotation[5] = axisY.y;
	m_Rotation[6] = axisY.z;

	m_Rotation[8] = axisZ.x;
	m_Rotation[9] = axisZ.y;
	m_Rotation[10] = axisZ.z;
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

	m_Rotation2 = m_Rotation;

	m_Rotation[0] = axisX.x;
	m_Rotation[1] = axisX.y;
	m_Rotation[2] = axisX.z;

	m_Rotation[4] = axisY.x;
	m_Rotation[5] = axisY.y;
	m_Rotation[6] = axisY.z;

	m_Rotation[8] = axisZ.x;
	m_Rotation[9] = axisZ.y;
	m_Rotation[10] = axisZ.z;

}

void Camera::UpdateOrthographicProjection(const Frustum& view_frustum)
{
	UpdateOrthographicProjection(view_frustum.GetMin(), view_frustum.GetMax(), view_frustum.GetNearPlane(), view_frustum.GetFarPlane());
}

void Camera::UpdateOrthographicProjection(CU::Vector3f min, CU::Vector3f max, float near_plane, float far_plane)
{
	const float width = (max.x - min.x);
	const float height = (max.y - min.y);

	m_ProjectionMatrix[0] = 2.f / width;
	m_ProjectionMatrix[5] = 2.f / height;
	m_ProjectionMatrix[10] = -1.f / (far_plane - near_plane);
	m_ProjectionMatrix[14] = (near_plane / (near_plane - far_plane));
	m_ProjectionMatrix[15] = 1.f;
}
