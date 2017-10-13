#include "stdafx.h"
#include "Frustum.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "Engine.h"

void Frustum::Initiate(float near_plane, float far_plane, float fov, const CU::Matrix44f* orientation)
{
	m_FOV = fov;
	m_Orientation = orientation;
	m_FarPlane = far_plane;
	m_NearPlane = near_plane;

	const float rotation = cl::DegreeToRad(fov / 2.f) ;

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
	//UpdateOBB();
	DrawFrustum();
}

bool Frustum::Inside(const CU::Vector3f& position, float radius) const
{
	const CU::Vector3f collision_check_position = (position) * m_InvertedOrientation;
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

void Frustum::DrawFrustum()
{
	LinePoint p1, p2, p3, p4, p5, p6, p7, p8;
	p1.color = CU::Vector4f(0, 1, 0, 1);
	p2.color = p1.color;
	p3.color = p1.color;
	p4.color = p1.color;
	p5.color = p1.color;
	p6.color = p1.color;
	p7.color = p1.color;
	p8.color = p1.color;

	p1.position = m_Orientation->GetTranslation(); // translation.myOrientation.GetTranslation();
	p2.position = p1.position;
	p3.position = p1.position;
	p4.position = p1.position;
	p5.position = p1.position;
	p6.position = p1.position;
	p7.position = p1.position;
	p8.position = p1.position;

	const CU::Vector4f up = m_Orientation->GetUp();
	const CU::Vector4f down = ( up - ( up * 2.f ) );
	const CU::Vector4f right = m_Orientation->GetRight();
	const CU::Vector4f left = ( right - ( right * 2.f ) );
	const CU::Vector4f forward = m_Orientation->GetForward();
	const CU::Vector4f position = m_Orientation->GetPosition();
	const CU::Vector4f far_plane = position + forward * m_FarPlane;
	const float half_width = m_FarPlane ;
	p5.position = far_plane;

	p5.position += ( left * half_width );
	p5.position += ( down * half_width );


	p6.position = far_plane;
	p6.position += ( left * half_width );
	p6.position += ( up * half_width );

	p7.position = far_plane;
	p7.position += ( right * half_width );
	p7.position += ( down * half_width );


	p8.position = far_plane;
	p8.position += ( right * half_width );
	p8.position += ( up * half_width );



	Synchronizer* sync = Engine::GetInstance()->GetSynchronizer();

	sync->AddRenderCommand(LineCommand(p1, p2, true));
	sync->AddRenderCommand(LineCommand(p2, p3, true));
	sync->AddRenderCommand(LineCommand(p3, p4, true));
	sync->AddRenderCommand(LineCommand(p4, p1, true));
	sync->AddRenderCommand(LineCommand(p1, p5, true));
	sync->AddRenderCommand(LineCommand(p5, p6, true));
	sync->AddRenderCommand(LineCommand(p6, p8, true));
	sync->AddRenderCommand(LineCommand(p8, p7, true));
	sync->AddRenderCommand(LineCommand(p7, p5, true));
	sync->AddRenderCommand(LineCommand(p6, p4, true));
	sync->AddRenderCommand(LineCommand(p7, p2, true));
	sync->AddRenderCommand(LineCommand(p8, p3, true));

	p1.color = CU::Vector4f(0, 1, 1, 1);
	p2.color = p1.color;
	p3.color = p1.color;
	p4.color = p1.color;
	p5.color = p1.color;
	p6.color = p1.color;
	p7.color = p1.color;
	p8.color = p1.color;

	p1.position = m_Orientation->GetTranslation(); // translation.myOrientation.GetTranslation();
	p2.position = p1.position;
	p3.position = p1.position;
	p4.position = p1.position;
	p5.position = p1.position;
	p6.position = p1.position;
	p7.position = p1.position;
	p8.position = p1.position;


	p1.position = MIN(m_MinPos, m_MaxPos);
	p8.position = MAX(m_MinPos, m_MaxPos);

	p2.position = p1.position;
	p2.position.x = MAX(m_MinPos.x, m_MaxPos.x);

	p3.position = p8.position;
	p3.position.y = MIN(m_MinPos.y, m_MaxPos.y);

	p4.position = p1.position;
	p4.position.z = MAX(m_MinPos.z, m_MaxPos.z);

	p5.position = p1.position;
	p5.position.y = MAX(m_MinPos.y, m_MaxPos.y);

	p6.position = p8.position;
	p6.position.x = MIN(m_MinPos.x, m_MaxPos.x);

	p7.position = p8.position;
	p7.position.z = MIN(m_MinPos.z, m_MaxPos.z);



	sync->AddRenderCommand(LineCommand(p1, p2, true));
	sync->AddRenderCommand(LineCommand(p2, p3, true));
	sync->AddRenderCommand(LineCommand(p3, p4, true));
	sync->AddRenderCommand(LineCommand(p4, p1, true));
	sync->AddRenderCommand(LineCommand(p1, p5, true));
	sync->AddRenderCommand(LineCommand(p5, p6, true));
	sync->AddRenderCommand(LineCommand(p6, p8, true));
	sync->AddRenderCommand(LineCommand(p8, p7, true));
	sync->AddRenderCommand(LineCommand(p7, p5, true));
	sync->AddRenderCommand(LineCommand(p6, p4, true));
	sync->AddRenderCommand(LineCommand(p7, p2, true));
	sync->AddRenderCommand(LineCommand(p8, p3, true));

}

void Frustum::UpdateOBB()
{
	//TranslationComponent& translation = GetComponent<TranslationComponent>(e);
	CU::Vector4f up = m_Orientation->GetUp();
	CU::Vector4f right = m_Orientation->GetRight();
	CU::Vector4f forward = m_Orientation->GetForward();

	const float rotation = sin(m_FOV / 2.f);

	//m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, 0), )); //right
	//m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(-rotation, 0, -rotation))); //left

	//m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(0, rotation, -rotation))); //up
	//m_Volume.AddPlane(CU::Plane<float>(CU::Vector3f(0, 0, 0), CU::Vector3f(0, -rotation, -rotation))); //down

	CU::Vector4f position;

	//z
	position = m_Orientation->GetTranslation();
	position += forward * m_NearPlane;
	forward -= ( forward * 2.f );
	m_Volume.m_Planes[0].InitWithPointAndNormal(position, forward);

	position = m_Orientation->GetTranslation();
	position += forward * m_FarPlane;
	m_Volume.m_Planes[1].InitWithPointAndNormal(position, forward);

	//x
	position = m_Orientation->GetTranslation();
	position += right * m_MaxPos.x;
	m_Volume.m_Planes[2].InitWithPointAndNormal(position, CU::Vector3f(rotation, 0, -rotation));

	position = m_Orientation->GetTranslation();
	position += right * m_MinPos.x;
	right -= ( right * 2.f );
	m_Volume.m_Planes[3].InitWithPointAndNormal(position, CU::Vector3f(-rotation, 0, -rotation));

	//y
	position = m_Orientation->GetTranslation();
	position += up * m_MaxPos.y;
	m_Volume.m_Planes[4].InitWithPointAndNormal(position, CU::Vector3f(0, rotation, -rotation));

	position = m_Orientation->GetTranslation();
	position += up * m_MinPos.y;
	up -= ( up * 2.f );
	m_Volume.m_Planes[5].InitWithPointAndNormal(position, CU::Vector3f(0, -rotation, -rotation));



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

	result = CU::Vector4f();
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

