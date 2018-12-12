#pragma once

#include <Engine/engine_shared.h>
#include "Engine/ConstantBuffer.h"
#include <CommonLib/EventManager/EventManager.h>
class Camera;
class Texture;
namespace graphics
{
	class Viewport;
};

class ShadowDirectional : public Subscriber
{
public:
	ShadowDirectional() = default;
	void Initiate(float buffer_size);
	void CleanUp();


	Camera* GetCamera() { return m_Camera; }
	Camera* GetCamera() const { return m_Camera; }

	void SetViewport();
	void ClearTexture();
	void SetTargets();

	void SetOrientation(const CU::Matrix44f& orientation);
	const CU::Matrix44f& GetOrientation() { return m_Camera->GetOrientation(); }


	void Update();

	CU::Matrix44f GetMVP();


	Texture* GetDepthTexture() { return m_ShadowDepth; }
	Texture* GetDepthStencilTexture() { return m_ShadowDepthStencil; }

	graphics::ConstantBuffer& GetConstBuffer() { return m_ConstBuffer; }
	void HandleEvent(uint64 event, void* pData) override;
private:
	graphics::ConstantBuffer m_ConstBuffer;
	graphics::Viewport*	m_Viewport		= nullptr;
	Camera*		m_Camera				= nullptr;
	CU::Matrix44f m_ViewMatrix;
	Texture*	m_ShadowDepth			= nullptr;
	Texture*	m_ShadowDepthStencil	= nullptr;
};

