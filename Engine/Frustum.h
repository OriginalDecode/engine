#pragma once
#include <Math/Matrix/Matrix.h>
#include <Math/Plane/PlaneVolume.h>

class Frustum
{
public:
	Frustum() = default;

	void Initiate(float near_plane, float far_plane, float fov, const CU::Matrix44f* orientation);

	void Update();

	bool Inside(const CU::Vector3f& position, float radius) const;
	bool InsideAABB(const CU::Vector3f& position) const;
	bool InsideAABB(const CU::Vector4f& position) const;
	void OnResize(float new_fov);
private:
	void DrawFrustum();
	void UpdateOBB();
	const CU::Matrix44f* m_Orientation;
	CU::Matrix44f m_InvertedOrientation;

	CU::Vector4f m_UpLeft;
	CU::Vector4f m_UpRight;
	CU::Vector4f m_DownLeft;
	CU::Vector4f m_DownRight;

	CU::Vector3f m_MaxPos;
	CU::Vector3f m_MinPos;

	CU::PlaneVolume<float> m_Volume;

	float m_FOV = 0.f;
	float m_NearPlane = 0.f;
	float m_FarPlane = 0.f;

	void CalcCorners();

};

