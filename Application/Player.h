#pragma once
#include <Math/Matrix/Matrix.h>
#include <Math/Quaternion/Quaternion.h>
namespace Hex
{
	class Camera;
	class Engine;
	class Synchronizer;
};

struct ControllerState;

class Player
{
public:
	Player() = default;

	void Initiate();

	void Update(float dt);


private:
	
	void UpdateOrientation(const ControllerState& state);


	Hex::Engine* m_Engine = nullptr;
	Hex::Camera* m_Camera = nullptr;
	Hex::Synchronizer* m_Synchronizer = nullptr;
	CU::Matrix44f m_Orientation;
	std::string m_ModelKey;

	float m_LookSpeedModifier = 0.005f;
	CU::Vector2f m_CenterPoint;
	CU::Quaternion m_Pitch;
	CU::Quaternion m_Yaw;

};

