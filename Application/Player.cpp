#include "Player.h"

#include <Engine.h>
#include <Camera.h>

#include <Synchronizer.h>
#include <RenderCommand.h>
#include <Input/InputHandle.h>
#include <Input/ControllerInput.h>

void Player::Initiate()
{
	m_Engine = Hex::Engine::GetInstance();
	m_Camera = m_Engine->GetCamera();
	m_Synchronizer = m_Engine->GetSynchronizer();
	m_ModelKey = m_Engine->LoadModel("Data/Model/cube.fbx", "Data/Shaders/T_Cube.json");
}

void Player::Update(float dt)
{
	InputHandle * handle = m_Engine->GetInputHandle();
	const ControllerState& state = handle->GetController(0)->GetState();

	UpdateOrientation(state);

	m_Camera->SetOrientation(m_Orientation);

	CU::Vector4f forward = m_Orientation.GetForward();
	CU::Vector4f position = m_Orientation.GetPosition();

	position = position - (forward * 5.f);
	m_Camera->SetPosition(CU::Vector3f(position.x, position.y, position.z));

	m_Synchronizer->AddRenderCommand(RenderCommand(eType::MODEL, m_ModelKey, m_Orientation, CU::Vector4f()));

}

void Player::UpdateOrientation(const ControllerState& state)
{
	float x_value = (float)state.m_ThumbRX / SHRT_MAX;
	float y_value = (float)state.m_ThumbRY / SHRT_MAX;
	if (0.5f < x_value && 0.5f < y_value ||
		x_value < -0.5f && y_value < -0.5f ||
		x_value < -0.5f && 0.5f < y_value ||
		0.5f < x_value && y_value < -0.5f)
	{
		x_value /= 2.f;
		y_value /= 2.f;
	}

	m_CenterPoint.x += x_value * m_LookSpeedModifier;
	m_CenterPoint.y -= y_value * m_LookSpeedModifier;
	m_CenterPoint.y = fmaxf(fminf(1.57f, m_CenterPoint.y), -1.57f);

	m_Pitch = CU::Quaternion(CU::Vector3f(1.f, 0, 0), m_CenterPoint.y);
	m_Yaw = CU::Quaternion(CU::Vector3f(0, 1.f, 0), m_CenterPoint.x);

	CU::Vector3f axisX(1.f, 0, 0);
	CU::Vector3f axisY(0, 1.f, 0);
	CU::Vector3f axisZ(0, 0, 1.f);

	axisX = m_Yaw * m_Pitch * axisX;
	axisY = m_Yaw * m_Pitch * axisY;
	axisZ = m_Yaw * m_Pitch * axisZ;

	m_Orientation.myMatrix[0] = axisX.x;
	m_Orientation.myMatrix[1] = axisX.y;
	m_Orientation.myMatrix[2] = axisX.z;
	m_Orientation.myMatrix[4] = axisY.x;
	m_Orientation.myMatrix[5] = axisY.y;
	m_Orientation.myMatrix[6] = axisY.z;
	m_Orientation.myMatrix[8] = axisZ.x;
	m_Orientation.myMatrix[9] = axisZ.y;
	m_Orientation.myMatrix[10] = axisZ.z;
}
