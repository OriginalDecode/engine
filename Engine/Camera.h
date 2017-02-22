#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include "../CommonLib/Math/Quaternion/Quaternion.h"

struct ControllerState;
namespace Hex
{
	enum class eDirection
	{
		FORWARD,
		BACK,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum class eRotation
	{
		X_AXIS,
		Y_AXIS,
		Z_AXIS
	};


	class Camera
	{
	public:
		Camera() = default;
		/*Camera(float aWidth, float aHeight);
		Camera(float aWidth, float aHeight, const CU::Vector3f& aPosition);
		Camera(float aWidth, float aHeight, float far_plane, float near_plane);
		Camera(float aWidth, float aHeight, float near_plane, float far_plane, float fov);*/

		void CreatePerspectiveProjection(float width, float height, float near_plane, float far_plane, float fov);
		const CU::Matrix44f& GetPerspective() const { return m_ProjectionMatrix; }

		void CreateOrthogonalProjection(float width, float height, float near_plane, float far_plane);
		const CU::Matrix44f& GetOrthogonal() const { return m_OrthogonalMatrix; }

		void CreateOrthographicProjection(float width, float height, float near_plane, float far_plane);
		const CU::Matrix44f& GetOrthographic() const { return m_ProjectionMatrix; }

		CU::Vector3f RotateAroundPoint(const CU::Vector3f& position);

		CU::Matrix44f& GetOrientation() { return m_Orientation; }
		const CU::Vector3f& GetPosition() const { return m_Orientation.GetPosition(); }
		CU::Matrix44f& Get2DOrientation() { return my2DOrientation; }

		const CU::Vector4f& GetAt() const { return m_Orientation.GetForward(); }

		void Move(eDirection aDirection, float aSpeed);
		void SetPosition(const CU::Vector3f& position);
		void SetTranslation(const CU::Vector4f& translation);

		void Update(const ControllerState& controller_state);
		void Update(const CU::Vector2f& cursor_pos );

		void SetOrientation(const CU::Matrix44f& matrix);

		void RotateAroundX(float rad);
		void RotateAroundY(float rad);
		void RotateAroundZ(float rad);

		void SetFOV(float field_of_view);
	
		/*	void IncreaseLookModifier() { m_LookSpeedModifier += 0.0001f; }
		void DecreaseLookModifier() { m_LookSpeedModifier -= 0.0001f; }*/


		//void ToggleFreefly() { m_Controller = !m_Controller; }
		//bool GetCanFreeFly() { return m_Controller; }
	private:
		float m_LookSpeedModifier = 0.005f;
		void operator=(Camera&) = delete;
		void MoveForwardAndBack(CU::Vector4f& aPosition, float aSpeed);
		void MoveUpAndDown(CU::Vector4f& aPosition, float aSpeed);
		void MoveLeftAndRight(CU::Vector4f& aPosition, float aSpeed);


		void OrientCamera();

		CU::Matrix44f m_Orientation;
		CU::Matrix44f my2DOrientation;

		bool m_ProjectionCreated = false;
		CU::Matrix44f m_ProjectionMatrix;
		CU::Matrix44f m_OrthogonalMatrix;
		float m_FOV = 90.f;

		CU::Vector2f m_CenterPoint;
		CU::Quaternion m_Pitch;
		CU::Quaternion m_Yaw;
		CU::Quaternion m_Roll;

	};
}