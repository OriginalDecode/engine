#include "stdafx.h"
#include "Frustum.h"

void Frustum::Initiate(float near_plane, float far_plane, float fov, const CU::Matrix44f* orientation)
{
	m_Orientation = orientation;
	m_FarPlane = far_plane;
	m_NearPlane = near_plane;

	const float rotation = sin(fov / 2.f) ;

	m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, near_plane), CU::Vector3f(0, 0, -1))); //near
	m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, far_plane), CU::Vector3f(0, 0, 1))); //far
	m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(rotation, 0, -rotation))); //right
	m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(-rotation, 0, -rotation))); //left
	m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(0, rotation, -rotation))); //up
	m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(0, -rotation, -rotation))); //down

	m_UpLeft = CU::Vector4f(-far_plane, +far_plane, +far_plane, 1.f);
	m_UpRight = CU::Vector4f(+far_plane, +far_plane, +far_plane, 1.f);
	m_DownLeft = CU::Vector4f(-far_plane, -far_plane, +far_plane, 1.f);
	m_DownRight = CU::Vector4f(+far_plane, -far_plane, +far_plane, 1.f);

}

void Frustum::Update()
{
	m_InvertedOrientation = CU::Math::Inverse(*m_Orientation);
	CalcCorners();
}

bool Frustum::Inside(const CU::Vector3f& position, float ) const
{
	const CU::Vector3f collision_check_position = CU::Vector3f(position) * m_InvertedOrientation;
	return m_Volume.Inside(collision_check_position);
}

bool Frustum::InsideAABB(const CU::Vector3f& position) const
{
	if ( position > m_Orientation->GetPosition() - m_FarPlane && position < m_Orientation->GetPosition() + m_FarPlane )
		return true;

	return false;
}

void Frustum::OnResize(float new_fov)
{
	const float rotation = sin(new_fov / 2.f);

	m_Volume.m_Planes[0] = ( CU::Plane<float>(CU::Vector3f(0, 0, m_NearPlane), CU::Vector3f(0, 0, -1)) ); //near
	m_Volume.m_Planes[1] = ( CU::Plane<float>(CU::Vector3f(0, 0, m_FarPlane), CU::Vector3f(0, 0, 1)) ); //far
	m_Volume.m_Planes[2] = ( CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(rotation, 0, -rotation)) ); //right
	m_Volume.m_Planes[3] = ( CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(-rotation, 0, -rotation)) ); //left
	m_Volume.m_Planes[4] = ( CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(0, rotation, -rotation)) ); //up
	m_Volume.m_Planes[5] = ( CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(0, -rotation, -rotation)) ); //down

	m_UpLeft = CU::Vector4f(-m_FarPlane, +m_FarPlane, +m_FarPlane, 1.f);
	m_UpRight = CU::Vector4f(+m_FarPlane, +m_FarPlane, +m_FarPlane, 1.f);
	m_DownLeft = CU::Vector4f(-m_FarPlane, -m_FarPlane, +m_FarPlane, 1.f);
	m_DownRight = CU::Vector4f(+m_FarPlane, -m_FarPlane, +m_FarPlane, 1.f);
}

void Frustum::CalcCorners()
{
	CU::Vector4f result = m_Orientation->GetTranslation();

	result.x = fminf(result.x, ( m_UpLeft * *m_Orientation ).x);
	result.y = fminf(result.y, ( m_UpLeft * *m_Orientation ).y);
	result.z = fminf(result.z, ( m_UpLeft * *m_Orientation ).z);

	result.x = fminf(result.x, ( m_UpRight * *m_Orientation ).x);
	result.y = fminf(result.y, ( m_UpRight * *m_Orientation ).y);
	result.z = fminf(result.z, ( m_UpRight * *m_Orientation ).z);

	result.x = fminf(result.x, ( m_DownLeft * *m_Orientation ).x);
	result.y = fminf(result.y, ( m_DownLeft * *m_Orientation ).y);
	result.z = fminf(result.z, ( m_DownLeft * *m_Orientation ).z);

	result.x = fminf(result.x, ( m_DownRight * *m_Orientation ).x);
	result.y = fminf(result.y, ( m_DownRight * *m_Orientation ).y);
	result.z = fminf(result.z, ( m_DownRight * *m_Orientation ).z);

	m_MinPos.x = result.x;
	m_MinPos.y = result.y;
	m_MinPos.z = result.z;

	result.x = fmaxf(result.x, ( m_UpLeft * *m_Orientation ).x);
	result.y = fmaxf(result.y, ( m_UpLeft * *m_Orientation ).y);
	result.z = fmaxf(result.z, ( m_UpLeft * *m_Orientation ).z);

	result.x = fmaxf(result.x, ( m_UpRight * *m_Orientation ).x);
	result.y = fmaxf(result.y, ( m_UpRight * *m_Orientation ).y);
	result.z = fmaxf(result.z, ( m_UpRight * *m_Orientation ).z);

	result.x = fmaxf(result.x, ( m_DownLeft * *m_Orientation ).x);
	result.y = fmaxf(result.y, ( m_DownLeft * *m_Orientation ).y);
	result.z = fmaxf(result.z, ( m_DownLeft * *m_Orientation ).z);

	result.x = fmaxf(result.x, ( m_DownRight * *m_Orientation ).x);
	result.y = fmaxf(result.y, ( m_DownRight * *m_Orientation ).y);
	result.z = fmaxf(result.z, ( m_DownRight * *m_Orientation ).z);

	m_MaxPos.x = result.x;
	m_MaxPos.y = result.y;
	m_MaxPos.z = result.z;


}

