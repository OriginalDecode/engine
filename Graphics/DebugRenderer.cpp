#include "DebugRenderer.h"
#include <DL_Debug/DL_Debug.h>
#include <Engine/Synchronizer.h>
#include <Engine/engine_shared.h>
#include <CommonLib/Utilities.h>

DebugRenderer* DebugRenderer::m_Instance = nullptr;

constexpr int _360Degrees = 360;

static const float c0		= cosf(cl::DegreeToRad(0.f));
static const float c90		= cosf(cl::DegreeToRad(90.f));
static const float c180		= cosf(cl::DegreeToRad(180.f));
static const float c270		= cosf(cl::DegreeToRad(270.f));

static const float s0		= sinf(cl::DegreeToRad(0.f));
static const float s90		= sinf(cl::DegreeToRad(90.f));
static const float s180		= sinf(cl::DegreeToRad(180.f));
static const float s270		= sinf(cl::DegreeToRad(270.f));

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
	ASSERT(!m_Instance, "Instance already created!");
}

void DebugRenderer::Destroy()
{
	delete m_Instance;
	m_Instance = nullptr;
}

void DebugRenderer::DrawCone(CU::Matrix44f orientation, const CU::Vector4f color, float range, float angle, float sides)
{
	//Should probably cache the cones and then just scale?

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


	orientation = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(90.f)) * orientation;

	_0deg.position = _0deg.position		* orientation;
	_90deg.position = _90deg.position	* orientation;
	_180deg.position = _180deg.position * orientation;
	_270deg.position = _270deg.position * orientation;

	sync->AddRenderCommand(LineCommand(origo, _0deg, true));
	sync->AddRenderCommand(LineCommand(origo, _90deg, true));
	sync->AddRenderCommand(LineCommand(origo, _180deg, true));
	sync->AddRenderCommand(LineCommand(origo, _270deg, true));


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

		sync->AddRenderCommand(LineCommand(p0, p1, true));
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

