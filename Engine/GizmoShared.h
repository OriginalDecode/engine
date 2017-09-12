#pragma once
#include <Math/Matrix/Matrix44.h>
#include <Math/Plane/PlaneVolume.h>
#include <CommonLib/HashString.h>
class Model;
class Texture;

struct GizmoHandle
{
	enum class eDirection
	{
		//POSITION & SCALE
		RIGHT,
		UP,
		FORWARD,
		//ROTATION
		X,
		Y,
		Z,
	};
	eDirection direction;

	void Initiate();

	~GizmoHandle();
	cl::CHashString<128> m_Key;

	Model* m_Model = nullptr;
	Texture* m_DirColor = nullptr;
	CU::Matrix44f m_Orientation;
	float m_Offset = 0.f;
	
	CU::PlaneVolume<float> m_OBB;
	bool Inside(const CU::Vector3f& position);
	void SetPosition(const CU::Vector3f& position);
	void RenderBox();
	void Update();
};