#pragma once
#include <Math/Matrix/Matrix44.h>
#include <Math/Plane/PlaneVolume.h>
#include <string>
class CModel;
class Texture;

struct GizmoHandle
{
	enum class eDirection
	{
		RIGHT,
		UP,
		FORWARD,
	};
	eDirection direction;

	void Initiate();

	~GizmoHandle();
	std::string m_Key;

	CModel* m_Model = nullptr;
	Texture* m_DirColor = nullptr;
	CU::Matrix44f m_Orientation;

	CU::Vector3f m_MinPos;
	CU::Vector3f m_MaxPos;

	CU::PlaneVolume<float> m_OBB;
	bool Inside(const CU::Vector3f& position);
	void RenderBox();
	void Update();
};