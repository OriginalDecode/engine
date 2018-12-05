#pragma once

#include <CommonLib/Math/Matrix/Matrix.h>

class DebugRenderer
{
public:


	static void Create();
	static void Destroy();
	static DebugRenderer* GetInstance() { return m_Instance; }

	void DrawCone(CU::Matrix44f orientation, const CU::Vector4f color, float range, float angle, float sides = 32);
	void DrawSphere();
	void DrawLine();
	void DrawCube();

	//draws a * in world space of the given position
	void DrawPosition(CU::Vector3f position);


private:
	static DebugRenderer* m_Instance;
	DebugRenderer() ;
	~DebugRenderer();
};
