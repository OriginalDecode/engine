#pragma once
#include <Frustum.h>
class Camera;
class CameraHandle
{
public:

	static CameraHandle* GetInstance();
	static void Create();
	static void Destroy();

	void Initiate();
	void Update();
	Camera* GetCamera();
	const Frustum& GetFrustum();
private:
	CameraHandle() = default;

	static CameraHandle* m_Instance;

	Camera* m_Camera = nullptr;
	Frustum m_Frustum;
};

