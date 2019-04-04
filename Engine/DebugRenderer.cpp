#include "stdafx.h"

#include "DebugRenderer.h"
#include <DL_Debug/DL_Debug.h>
#include <Engine/Synchronizer.h>
#include <Engine/engine_shared.h>
#include <CommonLib/Utilities.h>
#include "Renderer.h"
#include "Line3D.h"
DebugRenderer* DebugRenderer::m_Instance = nullptr;

constexpr int _360Degrees = 360;




constexpr float c0 = 1.f;			//cosf(cl::DegreeToRad(0.f));
constexpr float c90 = 0.f;			//cosf(cl::DegreeToRad(90.f));
constexpr float c180 = -1.f;		//cosf(cl::DegreeToRad(180.f));
constexpr float c270 = 0.f;			//cosf(cl::DegreeToRad(270.f));

constexpr float s0 = 0.f;			//sinf(cl::DegreeToRad(0.f));
constexpr float s90 = 1.f;			//sinf(cl::DegreeToRad(90.f));
constexpr float s180 = 0.f;			//sinf(cl::DegreeToRad(180.f));
constexpr float s270 = -1.f;		//sinf(cl::DegreeToRad(270.f));

DebugRenderer::DebugRenderer()
{
}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::Create()
{
	if (!m_Instance)
		m_Instance = new DebugRenderer;
	ASSERT(m_Instance, "Instance already created!");
}

void DebugRenderer::Destroy()
{
	delete m_Instance;
	m_Instance = nullptr;
}

void DebugRenderer::DrawCone(const CU::Matrix44f& orientation, const CU::Vector4f& color, float range, float angle, float sides)
{
	Synchronizer* sync = Engine::GetInstance()->GetSynchronizer();
	const float theta = tan(angle);
	const float halfwidth = theta * range;
	const int max = _360Degrees / sides;

	LinePoint origo;
	origo.color = color;
	origo.position = orientation.GetPosition();

	LinePoint _0deg, _90deg, _180deg, _270deg;
	_0deg.color = _90deg.color = _180deg.color = _270deg.color = color;

	const float y_pos = range;

	_0deg.position = { c0 * halfwidth, y_pos, s0 * halfwidth, 1 };
	_90deg.position = { c90 * halfwidth, y_pos, s90 * halfwidth, 1 };
	_180deg.position = { c180 * halfwidth, y_pos, s180 * halfwidth, 1 };
	_270deg.position = { c270 * halfwidth, y_pos, s270 * halfwidth, 1 };

	CU::Matrix44f matrix = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(90.f)) * orientation;

	_0deg.position = _0deg.position		* matrix;
	_90deg.position = _90deg.position	* matrix;
	_180deg.position = _180deg.position * matrix;
	_270deg.position = _270deg.position * matrix;

	Renderer* renderer = Engine::GetInstance()->GetRenderer();

	renderer->m_Line->AddLine(origo, _0deg);
	renderer->m_Line->AddLine(origo, _90deg);
	renderer->m_Line->AddLine(origo, _180deg);
	renderer->m_Line->AddLine(origo, _270deg);

	//Draw circle at the base of cone
	for (int i = 0; i < _360Degrees; i += max)
	{
		LinePoint p0, p1;
		p0.color = p1.color = color;

		const float x = cl::DegreeToRad(static_cast<float>(i));
		const float y = cl::DegreeToRad(static_cast<float>(i + max));

		p0.position = { cosf(x) * halfwidth, y_pos, sinf(x) * halfwidth, 1.f };
		p1.position = { cosf(y) * halfwidth, y_pos , sinf(y) * halfwidth, 1.f };

		p0.position = p0.position * orientation;
		p1.position = p1.position * orientation;
		renderer->m_Line->AddLine(p0, p1);
	}
}

void DebugRenderer::DrawSphere()
{

}

void DebugRenderer::DrawLine()
{

}

void DebugRenderer::DrawCube()
{

}

void DebugRenderer::DrawPosition(CU::Vector3f position)
{

#if !defined(_PROFILE) && !defined(_FINAL)
	Renderer* renderer = Engine::GetInstance()->GetRenderer();

	LinePoint points[2];

	points[0].color = { 1.f, 0.f, 0.f, 1.f };
	points[1].color = { 1.f, 0.f, 0.f, 1.f };

	points[0].position = position;
	points[0].position.y += 1.f;
	points[1].position = position;
	points[1].position.y -= 1.f;
	renderer->m_Line->AddLine(points);

	points[0].position = position;
	points[0].position.x += 1.f;
	points[1].position = position;
	points[1].position.x -= 1.f;
	renderer->m_Line->AddLine(points);

	points[0].position = position;
	points[0].position.z += 1.f;
	points[1].position = position;
	points[1].position.z -= 1.f;
	renderer->m_Line->AddLine(points);
#endif
}

