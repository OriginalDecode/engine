#pragma once
#include "snowblind_shared.h"

class Texture;
class Camera;
class Effect;
#include <Engine/GizmoBase.h>
class ShadowSpotlight
{
public:
	ShadowSpotlight() = default;

	bool Initiate(const CU::Vector3f& position, const CU::Vector3f& direction, float buffer_size);
	void Initiate(float buffer_size);
	bool CleanUp();

	Camera* GetCamera() { return m_Camera; }
	Camera* GetCamera() const { return m_Camera; }
	void SetViewport();

	Texture* GetDepthStencil() { return m_DepthStencil; }
	Texture* GetDepthTexture() { return m_Depth; }

	void ClearTexture();
	void SetTargets();

	void SetOrientation(const CU::Matrix44f& orientation);

	void SetAngle(float angle);

	const CU::Matrix44f& GetOrientation();
	const CU::Matrix44f& GetMVP();

	float GetBufferSize() const { return m_BufferSize; }

#ifdef _DEBUG
	void Copy();
	Texture* GetHolder() { return m_Holder; }
#endif


private:
	Effect* m_ShadowEffect	= nullptr;
	Camera* m_Camera		= nullptr;
	Viewport* m_Viewport	= nullptr;

	IDevContext* m_Context	= nullptr;

	Texture* m_Depth		= nullptr;
	Texture* m_DepthStencil = nullptr;

#ifdef _DEBUG
	Texture* m_Holder		= nullptr;
#endif

	float m_BufferSize		= 0.f;
	CU::Vector3f m_Position;
	CU::Vector3f m_Direction;


};
